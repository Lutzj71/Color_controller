// Pin map and timings taken from Panlee's PanelLan_esp32_arduino (board/sc05).

#include <Arduino.h>
#include <driver/i2c.h>
#include <esp_heap_caps.h>
#include <lvgl.h>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>

#include "display.h"

static constexpr int SCREEN_WIDTH = 800;
static constexpr int SCREEN_HEIGHT = 480;
// Draw buffers live in internal SRAM: the panel's frame buffer is in PSRAM and
// its DMA gets starved (flicker) when the CPU and WiFi also hammer PSRAM.
static constexpr int DRAW_BUF_LINES = 24;  // 800 * 24 * 2 B = 38 KB per buffer

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_RGB _panel;
  lgfx::Bus_RGB _bus;
  lgfx::Light_PWM _light;
  lgfx::Touch_GT911 _touch;

public:
  LGFX() {
    {
      auto cfg = _bus.config();
      cfg.panel = &_panel;
      // D0..D15 = B0..B4, G0..G5, R0..R4
      cfg.pin_d0 = 17;
      cfg.pin_d1 = 16;
      cfg.pin_d2 = 15;
      cfg.pin_d3 = 7;
      cfg.pin_d4 = 6;
      cfg.pin_d5 = 21;
      cfg.pin_d6 = 0;
      cfg.pin_d7 = 46;
      cfg.pin_d8 = 3;
      cfg.pin_d9 = 8;
      cfg.pin_d10 = 18;
      cfg.pin_d11 = 10;
      cfg.pin_d12 = 11;
      cfg.pin_d13 = 12;
      cfg.pin_d14 = 13;
      cfg.pin_d15 = 14;

      cfg.pin_pclk = 9;
      cfg.pin_vsync = 38;
      cfg.pin_hsync = 5;
      cfg.pin_henable = 39;

      cfg.freq_write = 12000000;
      cfg.hsync_polarity = 1;
      cfg.hsync_front_porch = 20;
      cfg.hsync_pulse_width = 1;
      cfg.hsync_back_porch = 87;
      cfg.vsync_polarity = 1;
      cfg.vsync_front_porch = 5;
      cfg.vsync_pulse_width = 1;
      cfg.vsync_back_porch = 31;
      cfg.pclk_active_neg = 0;
      _bus.config(cfg);
      _panel.setBus(&_bus);
    }
    {
      auto cfg = _panel.config();
      cfg.memory_width = SCREEN_WIDTH;
      cfg.panel_width = SCREEN_WIDTH;
      cfg.memory_height = SCREEN_HEIGHT;
      cfg.panel_height = SCREEN_HEIGHT;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      _panel.config(cfg);
    }
    {
      auto cfg = _light.config();
      cfg.pin_bl = 45;
      cfg.invert = false;
      cfg.freq = 22222;
      cfg.pwm_channel = 7;
      _light.config(cfg);
      _panel.setLight(&_light);
    }
    {
      auto cfg = _touch.config();
      cfg.i2c_addr = 0x5D;
      cfg.i2c_port = I2C_NUM_1;
      cfg.pin_sda = 48;
      cfg.pin_scl = 47;
      cfg.pin_int = -1;
      cfg.pin_rst = -1;
      cfg.freq = 400000;
      cfg.x_min = 0;
      cfg.x_max = SCREEN_WIDTH;
      cfg.y_min = 0;
      cfg.y_max = SCREEN_HEIGHT;
      cfg.bus_shared = false;
      cfg.offset_rotation = 0;
      _touch.config(cfg);
      _panel.setTouch(&_touch);
    }
    setPanel(&_panel);
  }
};

static LGFX tft;
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;

static void flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
  const int32_t w = area->x2 - area->x1 + 1;
  const int32_t h = area->y2 - area->y1 + 1;
  tft.startWrite();
  tft.pushImage(area->x1, area->y1, w, h, reinterpret_cast<lgfx::rgb565_t *>(color_p));
  tft.endWrite();
  lv_disp_flush_ready(drv);
}

static void touch_read_cb(lv_indev_drv_t *, lv_indev_data_t *data) {
  uint16_t x, y;
  if (tft.getTouch(&x, &y)) {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = x;
    data->point.y = y;
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void display_init(void) {
  tft.init();
  tft.setBrightness(255);

  const size_t buf_px = SCREEN_WIDTH * DRAW_BUF_LINES;
  auto *buf1 = static_cast<lv_color_t *>(heap_caps_malloc(buf_px * sizeof(lv_color_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT));
  auto *buf2 = static_cast<lv_color_t *>(heap_caps_malloc(buf_px * sizeof(lv_color_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT));
  if (!buf1 || !buf2) {
    Serial.println("display_init: draw buffer allocation failed (out of internal RAM?)");
    while (true) delay(1000);
  }
  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, buf_px);

  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  disp_drv.flush_cb = flush_cb;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touch_read_cb;
  lv_indev_drv_register(&indev_drv);
}
