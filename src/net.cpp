#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <lvgl.h>

#include "app.h"
#include "net.h"
#include "secrets.h"

static const char *TOPIC_TEMPERATURE = "temperatury/wielicka";
static const char *TOPIC_PRESSURE = "temperatury/wielicka_p";
// History source shared with DisplayUnit (WeatherDisplay_CD): Node-RED answers a
// request on TOPIC_HISTORY_REQ with both series (not retained), to every subscriber.
static const char *TOPIC_HISTORY_REQ = "temperatury/DisplayUnit/history";
static const char *TOPIC_HIST_T = "temperatury/wielicka_hist_t";
static const char *TOPIC_HIST_P = "temperatury/wielicka_hist_p";
static const char *TOPIC_BATHROOM_FLOOR = "temperatury/bathroom_floor_t";
static const char *TOPIC_TOILET_FLOOR = "temperatury/toilet_floor_t";
static const char *TOPIC_BATHROOM_AIR = "temperatury/shower_t";
static const char *TOPIC_BATHROOM_HUMIDITY = "temperatury/shower_h";
static const char *TOPIC_STATUS = "color_controller/status";
static const char *TOPIC_BATHROOM_HEATING_ENABLE = "temperatury/BathroomDisplay/Button2";
static const char *TOPIC_TOILET_HEATING_ENABLE = "temperatury/BathroomDisplay/Button3";
static const char *TOPIC_BATHROOM_HEAT_TARGET = "color_controller/HeatTargetBathroom";
static const char *TOPIC_TOILET_HEAT_TARGET = "color_controller/HeatTargetToilet";
static const char *TOPIC_BATHROOM_HEATING = "shelly1pmminig3-3030f9ec8140/command/switch:0";
static const char *TOPIC_TOILET_HEATING = "temperatury/RelayUnit/rly4";

static constexpr uint32_t RETRY_INTERVAL_MS = 5000;
static constexpr uint32_t AUTH_RETRY_INTERVAL_MS = 60000;  // after the broker rejects the login

// connect() blocks loop() (and touch handling) for at most TCP + CONNACK timeouts.
static constexpr uint32_t TCP_TIMEOUT_S = 1;
static constexpr uint16_t MQTT_SOCKET_TIMEOUT_S = 2;

static constexpr uint16_t MQTT_BUFFER = 2048;  // one history series is ~300-400 B
static constexpr const char *HISTORY_HOURS = "24";
static constexpr uint32_t HISTORY_REFRESH_MS = 30 * 60 * 1000UL;  // = Node-RED averaging step
static constexpr uint32_t HISTORY_RETRY_MS = 5 * 60 * 1000UL;     // until both series have arrived
static constexpr uint16_t HISTORY_MAX = 64;

static WiFiClient wifi_client;
static PubSubClient mqtt(wifi_client);
static uint32_t last_attempt_ms;
static uint32_t retry_interval_ms = RETRY_INTERVAL_MS;
static bool enabled;
static uint32_t history_request_ms;
static bool history_requested;
static bool got_hist_t, got_hist_p;

// {"start":<UTC s>,"step":1800,"v":[13.6,null,...]} -> app; null/non-number -> NAN.
static bool on_history(const char *topic, const uint8_t *payload, unsigned int len) {
  JsonDocument doc;
  const DeserializationError err = deserializeJson(doc, payload, len);
  JsonArrayConst arr = doc["v"];
  const uint32_t step = doc["step"] | 0u;
  if (err || arr.size() < 2 || step == 0) {
    Serial.printf("net: bad history on %s (%s)\n", topic, err ? err.c_str() : "missing v/step");
    return false;
  }

  static float v[HISTORY_MAX];
  const size_t skip = arr.size() > HISTORY_MAX ? arr.size() - HISTORY_MAX : 0;  // keep the newest
  uint16_t n = 0;
  size_t i = 0;
  for (JsonVariantConst x : arr) {
    if (i++ < skip) continue;
    v[n++] = x.is<float>() ? x.as<float>() : NAN;
  }

  if (strcmp(topic, TOPIC_HIST_T) == 0) app_set_temperature_history(v, n, step);
  else app_set_pressure_history(v, n, step);
  Serial.printf("net: history %s, %u points\n", topic, n);
  return true;
}

// Every HISTORY_REFRESH_MS, or HISTORY_RETRY_MS while the last answer was incomplete
// (e.g. Node-RED flow not deployed).
static void request_history_if_due(void) {
  const uint32_t now = millis();
  const uint32_t interval = (got_hist_t && got_hist_p) ? HISTORY_REFRESH_MS : HISTORY_RETRY_MS;
  if (history_requested && now - history_request_ms < interval) return;
  if (!mqtt.publish(TOPIC_HISTORY_REQ, HISTORY_HOURS)) return;
  history_requested = true;
  history_request_ms = now;
  got_hist_t = got_hist_p = false;
}

static void on_message(char *topic, uint8_t *payload, unsigned int len) {
  if (strcmp(topic, TOPIC_HIST_T) == 0) {
    got_hist_t |= on_history(topic, payload, len);
    return;
  }
  if (strcmp(topic, TOPIC_HIST_P) == 0) {
    got_hist_p |= on_history(topic, payload, len);
    return;
  }

  char buf[16];
  len = min<unsigned int>(len, sizeof(buf) - 1);
  memcpy(buf, payload, len);
  buf[len] = '\0';

  if (strcmp(topic, TOPIC_TEMPERATURE) == 0) app_set_temperature(atof(buf));
  else if (strcmp(topic, TOPIC_PRESSURE) == 0) app_set_pressure((int)lroundf(atof(buf)));
  else if (strcmp(topic, TOPIC_BATHROOM_FLOOR) == 0) app_set_bathroom_temperature(atof(buf));
  else if (strcmp(topic, TOPIC_TOILET_FLOOR) == 0) app_set_toilet_temperature(atof(buf));
  else if (strcmp(topic, TOPIC_BATHROOM_AIR) == 0) app_set_bathroom_air_temperature(atof(buf));
  else if (strcmp(topic, TOPIC_BATHROOM_HUMIDITY) == 0) app_set_bathroom_humidity(atoi(buf));
  else if (strcmp(topic, TOPIC_BATHROOM_HEATING) == 0) app_set_bathroom_heating(strcmp(buf, "on") == 0);
  else if (strcmp(topic, TOPIC_TOILET_HEATING) == 0) app_set_toilet_heating(strcmp(buf, "true") == 0);
}

void net_init(void) {
  if (strlen(WIFI_SSID) == 0 || strlen(MQTT_HOST) == 0) {
    Serial.println("net: WIFI_SSID / MQTT_HOST empty in include/secrets.h - network disabled");
    app_set_net_status("Net: not configured");
    return;
  }
  enabled = true;
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.setSleep(false);  // modem sleep causes periodic bursts that disturb the RGB panel
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(on_message);
  mqtt.setSocketTimeout(MQTT_SOCKET_TIMEOUT_S);
  mqtt.setBufferSize(MQTT_BUFFER);
  wifi_client.setTimeout(TCP_TIMEOUT_S);
  app_set_net_status("WiFi: connecting...");
}

void net_loop(void) {
  if (!enabled) return;

  if (WiFi.status() != WL_CONNECTED) {
    app_set_net_status("WiFi: connecting...");
    return;
  }

  if (mqtt.connected()) {
    mqtt.loop();
    request_history_if_due();
    return;
  }

  const uint32_t now = millis();
  if (now - last_attempt_ms < retry_interval_ms && last_attempt_ms != 0) return;
  last_attempt_ms = now;

  app_set_net_status("MQTT: connecting...");
  lv_timer_handler();  // show the status before connect() blocks
  const String client_id = "color_controller-" + String((uint32_t)ESP.getEfuseMac(), HEX);
  const bool has_auth = strlen(MQTT_USER) > 0;
  const bool ok = mqtt.connect(client_id.c_str(), has_auth ? MQTT_USER : nullptr, has_auth ? MQTT_PASS : nullptr,
                               TOPIC_STATUS, 0, true, "offline");  // blocks up to TCP + socket timeout on failure
  retry_interval_ms = RETRY_INTERVAL_MS;
  if (ok) {
    mqtt.publish(TOPIC_STATUS, "online", true);
    mqtt.subscribe(TOPIC_TEMPERATURE);
    mqtt.subscribe(TOPIC_PRESSURE);
    mqtt.subscribe(TOPIC_HIST_T);
    mqtt.subscribe(TOPIC_HIST_P);
    mqtt.subscribe(TOPIC_BATHROOM_FLOOR);
    mqtt.subscribe(TOPIC_TOILET_FLOOR);
    mqtt.subscribe(TOPIC_BATHROOM_AIR);
    mqtt.subscribe(TOPIC_BATHROOM_HUMIDITY);
    mqtt.subscribe(TOPIC_BATHROOM_HEATING);
    mqtt.subscribe(TOPIC_TOILET_HEATING);
    net_publish_bathroom_heating_enable(app_get_bathroom_heating_enable());
    net_publish_toilet_heating_enable(app_get_toilet_heating_enable());
    history_requested = false;  // fresh history after every (re)connect
    request_history_if_due();
    app_set_net_status("MQTT: connected");
    Serial.printf("net: connected, IP %s\n", WiFi.localIP().toString().c_str());
  } else if (mqtt.state() == MQTT_CONNECT_BAD_CREDENTIALS || mqtt.state() == MQTT_CONNECT_UNAUTHORIZED) {
    // Retrying quickly will not help - back off until the credentials are fixed.
    retry_interval_ms = AUTH_RETRY_INTERVAL_MS;
    app_set_net_status("MQTT: bad login (check secrets.h)");
    Serial.printf("net: MQTT login rejected for user \"%s\", rc=%d - retry in %lu s\n", MQTT_USER, mqtt.state(),
                  (unsigned long)(AUTH_RETRY_INTERVAL_MS / 1000));
  } else {
    app_set_net_status("MQTT: retrying...");
    Serial.printf("net: MQTT connect failed, rc=%d\n", mqtt.state());
  }
}

void net_publish_bathroom_heating_enable(bool on) {
  if (!mqtt.connected()) return;
  mqtt.publish(TOPIC_BATHROOM_HEATING_ENABLE, on ? "1" : "0");
}

void net_publish_toilet_heating_enable(bool on) {
  if (!mqtt.connected()) return;
  mqtt.publish(TOPIC_TOILET_HEATING_ENABLE, on ? "1" : "0");
}

void net_publish_bathroom_heat_target(int celsius) {
  if (!mqtt.connected()) return;
  char buf[8];
  snprintf(buf, sizeof(buf), "%d", celsius);
  mqtt.publish(TOPIC_BATHROOM_HEAT_TARGET, buf);
}

void net_publish_toilet_heat_target(int celsius) {
  if (!mqtt.connected()) return;
  char buf[8];
  snprintf(buf, sizeof(buf), "%d", celsius);
  mqtt.publish(TOPIC_TOILET_HEAT_TARGET, buf);
}
