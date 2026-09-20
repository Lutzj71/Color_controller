#pragma once

// Application layer on top of the generated SquareLine UI (ui.c, screens/).
// Keep application logic here so re-exporting the UI does not overwrite it.

// Call after ui_init().
void app_init(void);

// Update the label and push a new point into the history chart.
void app_set_temperature(float celsius);
void app_set_humidity(int percent);

// Screen 2: floor temperatures (label only).
void app_set_bathroom_temperature(float celsius);
void app_set_toilet_temperature(float celsius);

// Small status line at the bottom-left of the screen (network state).
void app_set_net_status(const char *text);
