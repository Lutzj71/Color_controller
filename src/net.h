#pragma once

// WiFi + MQTT. Non-blocking: call net_init() once, net_loop() from loop().
//
// Topics:
//   subscribe  temperatury/wielicka          payload: float, degrees C
//   subscribe  temperatury/wielicka_h        payload: integer percent
//   subscribe  temperatury/bathroom_floor_t  payload: float, degrees C
//   subscribe  temperatury/toilet_floor_t    payload: float, degrees C
//   subscribe  temperatury/shower_t          payload: float, degrees C
//   subscribe  temperatury/shower_h          payload: integer percent
//   subscribe  shelly1pmminig3-3030f9ec8140/command/switch:0  payload: "on" / "off"
//   subscribe  temperatury/RelayUnit/rly4               payload: "true" / "false"
//   publish    color_controller/status       "online" (retained) / "offline" (LWT)
//   publish    temperatury/BathroomDisplay/Button2  "1" / "0"
//   publish    temperatury/BathroomDisplay/Button3  "1" / "0"
//   publish    color_controller/HeatTargetBathroom  integer, degrees C
//   publish    color_controller/HeatTargetToilet    integer, degrees C
void net_init(void);
void net_loop(void);

// Publishes "1"/"0" to temperatury/BathroomDisplay/Button2. No-op while WiFi/MQTT
// is not connected.
void net_publish_bathroom_heating_enable(bool on);

// Publishes "1"/"0" to temperatury/BathroomDisplay/Button3. No-op while WiFi/MQTT
// is not connected.
void net_publish_toilet_heating_enable(bool on);

// Publishes an integer (degrees C) to color_controller/HeatTargetBathroom.
// No-op while WiFi/MQTT is not connected.
void net_publish_bathroom_heat_target(int celsius);

// Publishes an integer (degrees C) to color_controller/HeatTargetToilet.
// No-op while WiFi/MQTT is not connected.
void net_publish_toilet_heat_target(int celsius);
