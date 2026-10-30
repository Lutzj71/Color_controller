// Pin map and timings taken from Panlee's PanelLan_esp32_arduino (board/sc05).
//
// Tear-free output: the panel is driven by ESP-IDF's esp_lcd RGB driver with
// two frame buffers in PSRAM. LVGL renders straight into the back buffer
// (direct mode) and the driver switches buffers at the frame boundary, so the
// panel never scans out a half-drawn frame. LVGL itself copies the changed
// areas into the other buffer before the next refresh (refr_sync_areas).

#include <Arduino.h>
#include <Wire.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_rgb.h>
#include <freertos/semphr.h>
#include <lvgl.h>
#include <string.h>

#include "display.h"

static constexpr int SCREEN_WIDTH = 800;
static constexpr int SCREEN_HEIGHT = 480;
// Bounce buffers live in internal SRAM and are refilled from the PSRAM frame
// buffer by the CPU, so the panel's DMA never reads PSRAM directly and does not
// get starved (flicker) when LVGL and WiFi hammer PSRAM. If a refill is late,
// the start of the next chunk shows stale pixels (short dashes at the left edge
// every BOUNCE_BUF_LINES lines); larger buffers give the refill more slack.
// Must divide SCREEN_HEIGHT.
static constexpr int BOUNCE_BUF_LINES = 20;  // 800 * 20 * 2 B = 32 KB, x2

static constexpr int PIN_BACKLIGHT = 45;
static constexpr int PIN_TOUCH_SDA = 48;
static constexpr int PIN_TOUCH_SCL = 47;
static constexpr uint8_t GT911_ADDR = 0x5D;
static constexpr uint16_t GT911_REG_STATUS = 0x814E;
static constexpr uint16_t GT911_REG_POINT1 = 0x8150;

static esp_lcd_panel_handle_t panel;
static SemaphoreHandle_t frame_done_sem;
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;

// Called once per frame, after the driver has finished reading the frame
// buffer it was scanning out.
static bool IRAM_ATTR on_frame_done(esp_lcd_panel_handle_t, const esp_lcd_rgb_panel_event_data_t *, void *) {
  BaseType_t woken = pdFALSE;
  xSemaphoreGiveFromISR(frame_done_sem, &woken);
  return woken == pdTRUE;
}

static void flush_cb(lv_disp_drv_t *drv, const lv_area_t *, lv_color_t *color_p) {
  // Direct mode: color_p is the whole back buffer, so act only once per frame.
  if (lv_disp_flush_is_last(drv)) {
    xSemaphoreTake(frame_done_sem, 0);
    // color_p is one of the driver's frame buffers: this only queues the switch.
    esp_lcd_panel_draw_bitmap(panel, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color_p);
    // Once the current frame ends the old buffer is free for LVGL to draw into.
    xSemaphoreTake(frame_done_sem, portMAX_DELAY);
  }
  lv_disp_flush_ready(drv);
}

static bool gt911_read(uint16_t reg, uint8_t *data, size_t len) {
  Wire1.beginTransmission(GT911_ADDR);
  Wire1.write(reg >> 8);
  Wire1.write(reg & 0xFF);
  if (Wire1.endTransmission(false) != 0) return false;
  if (Wire1.requestFrom(GT911_ADDR, len) != len) return false;
  for (size_t i = 0; i < len; i++) data[i] = Wire1.read();
  return true;
}

static void gt911_write(uint16_t reg, uint8_t value) {
  Wire1.beginTransmission(GT911_ADDR);
  Wire1.write(reg >> 8);
  Wire1.write(reg & 0xFF);
  Wire1.write(value);
  Wire1.endTransmission();
}

static void touch_read_cb(lv_indev_drv_t *, lv_indev_data_t *data) {
  static lv_indev_state_t state = LV_INDEV_STATE_REL;
  static lv_point_t point;

  uint8_t status;
  // Bit 7 = new data ready; until then report the previous state.
  if (gt911_read(GT911_REG_STATUS, &status, 1) && (status & 0x80)) {
    uint8_t p[4];
    if ((status & 0x0F) > 0 && gt911_read(GT911_REG_POINT1, p, sizeof(p))) {
      point.x = p[0] | (p[1] << 8);
      point.y = p[2] | (p[3] << 8);
      state = LV_INDEV_STATE_PR;
    } else {
      state = LV_INDEV_STATE_REL;
    }
    gt911_write(GT911_REG_STATUS, 0);
  }
  data->state = state;
  data->point = point;
}

static void panel_init(void) {
  esp_lcd_rgb_panel_config_t cfg = {};
  cfg.clk_src = LCD_CLK_SRC_DEFAULT;
  cfg.data_width = 16;
  cfg.num_fbs = 2;
  cfg.bounce_buffer_size_px = SCREEN_WIDTH * BOUNCE_BUF_LINES;
  cfg.disp_gpio_num = -1;
  cfg.pclk_gpio_num = 9;
  cfg.vsync_gpio_num = 38;
  cfg.hsync_gpio_num = 5;
  cfg.de_gpio_num = 39;
  // D0..D15 = B0..B4, G0..G5, R0..R4
  const int data_pins[16] = {17, 16, 15, 7, 6, 21, 0, 46, 3, 8, 18, 10, 11, 12, 13, 14};
  memcpy(cfg.data_gpio_nums, data_pins, sizeof(data_pins));
  cfg.flags.fb_in_psram = 1;

  cfg.timings.pclk_hz = 12000000;
  cfg.timings.h_res = SCREEN_WIDTH;
  cfg.timings.v_res = SCREEN_HEIGHT;
  cfg.timings.hsync_front_porch = 20;
  cfg.timings.hsync_pulse_width = 1;
  cfg.timings.hsync_back_porch = 87;
  cfg.timings.vsync_front_porch = 5;
  cfg.timings.vsync_pulse_width = 1;
  cfg.timings.vsync_back_porch = 31;
  // Sync signals idle high, DE idle low, data latched on the rising PCLK edge.
  cfg.timings.flags.hsync_idle_low = 0;
  cfg.timings.flags.vsync_idle_low = 0;
  cfg.timings.flags.de_idle_high = 0;
  cfg.timings.flags.pclk_active_neg = 0;
  cfg.timings.flags.pclk_idle_high = 0;

  ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&cfg, &panel));

  frame_done_sem = xSemaphoreCreateBinary();
  esp_lcd_rgb_panel_event_callbacks_t cbs = {};
  // With bounce buffers the frame buffer is read by the bounce ISR, which runs
  // ahead of VSYNC; this fires when it has finished with the frame buffer.
  cbs.on_frame_buf_complete = on_frame_done;
  ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(panel, &cbs, nullptr));

  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel));
}

void display_init(void) {
  pinMode(PIN_BACKLIGHT, OUTPUT);
  digitalWrite(PIN_BACKLIGHT, LOW);

  panel_init();

  void *fb0, *fb1;
  ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(panel, 2, &fb0, &fb1));
  // PSRAM content is random after reset; avoid flashing garbage.
  const size_t fb_px = SCREEN_WIDTH * SCREEN_HEIGHT;
  memset(fb0, 0, fb_px * sizeof(lv_color_t));
  memset(fb1, 0, fb_px * sizeof(lv_color_t));
  lv_disp_draw_buf_init(&draw_buf, fb0, fb1, fb_px);

  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  disp_drv.flush_cb = flush_cb;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.direct_mode = 1;
  lv_disp_drv_register(&disp_drv);

  digitalWrite(PIN_BACKLIGHT, HIGH);

  Wire1.begin(PIN_TOUCH_SDA, PIN_TOUCH_SCL, 400000);
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touch_read_cb;
  lv_indev_drv_register(&indev_drv);
}
