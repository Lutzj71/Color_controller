#pragma once

#include <stdint.h>

// Application layer on top of the generated SquareLine UI (ui.c, screens/).
// Keep application logic here so re-exporting the UI does not overwrite it.

// Call after ui_init().
void app_init(void);

// Screen 1: current outdoor values (label only).
void app_set_temperature(float celsius);
void app_set_pressure(int hpa);

// Screen 1: replace the whole history chart. v[0] is the oldest point, NAN = no
// data (gap in the line); points are step_s seconds apart, the last one is "now".
void app_set_temperature_history(const float *v, uint16_t count, uint32_t step_s);
void app_set_pressure_history(const float *v, uint16_t count, uint32_t step_s);

// Screen 2: floor temperatures (label only).
void app_set_bathroom_temperature(float celsius);
void app_set_toilet_temperature(float celsius);
void app_set_bathroom_air_temperature(float celsius);
void app_set_bathroom_humidity(int percent);

// Screen 2: heating LED indicators. on=true lights the LED (bright red with a
// glow), on=false shows it unlit (dark, no glow) - like a real LED.
void app_set_bathroom_heating(bool on);
void app_set_toilet_heating(bool on);

// Current state of the bathroom heating enable switch (ui_Bathroom_heating_enable).
bool app_get_bathroom_heating_enable(void);

// Current state of the toilet heating enable switch (ui_Toilet_heating_enable).
bool app_get_toilet_heating_enable(void);

// Current value of the shower heat target spinbox (ui_Spinbox_shower), degrees C.
int app_get_bathroom_heat_target(void);

// Current value of the toilet heat target spinbox (ui_Spinbox_toilet), degrees C.
int app_get_toilet_heat_target(void);

// Small status line at the bottom-left of the screen (network state).
void app_set_net_status(const char *text);
