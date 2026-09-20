#include <Arduino.h>
#include <lvgl.h>

#include "app.h"
#include "display.h"
#include "net.h"
#include "ui.h"

void setup() {
  Serial.begin(115200);
  lv_init();
  display_init();
  ui_init();
  app_init();
#ifndef DISABLE_NET
  net_init();
#endif
}

void loop() {
  lv_timer_handler();
#ifndef DISABLE_NET
  net_loop();
#endif
  delay(5);
}
