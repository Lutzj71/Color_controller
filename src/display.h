#pragma once

// Panlee ZX7D00CE01S (SC05): 800x480 RGB565 panel + GT911 touch.
// Sets up LovyanGFX and registers the display and touch drivers with LVGL.
// Call once, after lv_init() and before ui_init().
void display_init(void);
