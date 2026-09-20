#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "app.h"
#include "net.h"
#include "secrets.h"

static const char *TOPIC_TEMPERATURE = "temperatury/wielicka";
static const char *TOPIC_HUMIDITY = "temperatury/wielicka_h";
static const char *TOPIC_BATHROOM_FLOOR = "temperatury/bathroom_floor_t";
static const char *TOPIC_TOILET_FLOOR = "temperatury/toilet_floor_t";
static const char *TOPIC_STATUS = "color_controller/status";

static constexpr uint32_t RETRY_INTERVAL_MS = 5000;

static WiFiClient wifi_client;
static PubSubClient mqtt(wifi_client);
static uint32_t last_attempt_ms;
static bool enabled;

static void on_message(char *topic, uint8_t *payload, unsigned int len) {
  char buf[16];
  len = min<unsigned int>(len, sizeof(buf) - 1);
  memcpy(buf, payload, len);
  buf[len] = '\0';

  if (strcmp(topic, TOPIC_TEMPERATURE) == 0) app_set_temperature(atof(buf));
  else if (strcmp(topic, TOPIC_HUMIDITY) == 0) app_set_humidity(atoi(buf));
  else if (strcmp(topic, TOPIC_BATHROOM_FLOOR) == 0) app_set_bathroom_temperature(atof(buf));
  else if (strcmp(topic, TOPIC_TOILET_FLOOR) == 0) app_set_toilet_temperature(atof(buf));
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
    return;
  }

  const uint32_t now = millis();
  if (now - last_attempt_ms < RETRY_INTERVAL_MS && last_attempt_ms != 0) return;
  last_attempt_ms = now;

  app_set_net_status("MQTT: connecting...");
  const String client_id = "color_controller-" + String((uint32_t)ESP.getEfuseMac(), HEX);
  const bool has_auth = strlen(MQTT_USER) > 0;
  const bool ok = mqtt.connect(client_id.c_str(), has_auth ? MQTT_USER : nullptr, has_auth ? MQTT_PASSWORD : nullptr,
                               TOPIC_STATUS, 0, true, "offline");  // blocks up to the socket timeout on failure
  if (ok) {
    mqtt.publish(TOPIC_STATUS, "online", true);
    mqtt.subscribe(TOPIC_TEMPERATURE);
    mqtt.subscribe(TOPIC_HUMIDITY);
    mqtt.subscribe(TOPIC_BATHROOM_FLOOR);
    mqtt.subscribe(TOPIC_TOILET_FLOOR);
    app_set_net_status("MQTT: connected");
    Serial.printf("net: connected, IP %s\n", WiFi.localIP().toString().c_str());
  } else {
    app_set_net_status("MQTT: retrying...");
    Serial.printf("net: MQTT connect failed, rc=%d\n", mqtt.state());
  }
}
