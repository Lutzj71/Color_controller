#pragma once

// WiFi + MQTT. Non-blocking: call net_init() once, net_loop() from loop().
//
// Topics:
//   subscribe  temperatury/wielicka          payload: float, degrees C
//   subscribe  temperatury/wielicka_h        payload: integer percent
//   subscribe  temperatury/bathroom_floor_t  payload: float, degrees C
//   subscribe  temperatury/toilet_floor_t    payload: float, degrees C
//   subscribe  shelly1pmminig3-3030f9ec8140/command/switch:0  payload: "on" / "off"
//   subscribe  temperatury/RelayUnit/rly4               payload: "true" / "false"
//   publish    color_controller/status       "online" (retained) / "offline" (LWT)
void net_init(void);
void net_loop(void);
