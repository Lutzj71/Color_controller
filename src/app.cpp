#include <Arduino.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>

#include "app.h"
#include "net.h"
#include "ui.h"

static constexpr uint16_t HISTORY_POINTS = 60;

static lv_coord_t temperature_history[HISTORY_POINTS];
static lv_coord_t humidity_history[HISTORY_POINTS];
static lv_chart_series_t *temperature_series;
static lv_chart_series_t *humidity_series;

// Replace the generated demo data with an app-owned buffer of HISTORY_POINTS.
static lv_chart_series_t *setup_chart(lv_obj_t *chart, lv_coord_t *buf, lv_coord_t min, lv_coord_t max) {
  lv_chart_series_t *ser = lv_chart_get_series_next(chart, NULL);
  lv_chart_set_point_count(chart, HISTORY_POINTS);
  for (uint16_t i = 0; i < HISTORY_POINTS; i++) buf[i] = LV_CHART_POINT_NONE;
  lv_chart_set_ext_y_array(chart, ser, buf);
  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, min, max);
  lv_chart_refresh(chart);
  return ser;
}

static void bathroom_heating_enable_event_cb(lv_event_t *e) {
  lv_obj_t *sw = static_cast<lv_obj_t *>(lv_event_get_target(e));
  net_publish_bathroom_heating_enable(lv_obj_has_state(sw, LV_STATE_CHECKED));
}

static void toilet_heating_enable_event_cb(lv_event_t *e) {
  lv_obj_t *sw = static_cast<lv_obj_t *>(lv_event_get_target(e));
  net_publish_toilet_heating_enable(lv_obj_has_state(sw, LV_STATE_CHECKED));
}

static void bathroom_heat_target_event_cb(lv_event_t *) {
  net_publish_bathroom_heat_target(app_get_bathroom_heat_target());
}

static void toilet_heat_target_event_cb(lv_event_t *) {
  net_publish_toilet_heat_target(app_get_toilet_heat_target());
}

bool app_get_bathroom_heating_enable(void) {
  return lv_obj_has_state(ui_Bathroom_heating_enable, LV_STATE_CHECKED);
}

bool app_get_toilet_heating_enable(void) {
  return lv_obj_has_state(ui_Toilet_heating_enable, LV_STATE_CHECKED);
}

int app_get_bathroom_heat_target(void) {
  return lv_spinbox_get_value(ui_Spinbox_shower);
}

int app_get_toilet_heat_target(void) {
  return lv_spinbox_get_value(ui_Spinbox_toilet);
}

void app_init(void) {
  temperature_series = setup_chart(ui_Temperature_history, temperature_history, 0, 50);
  humidity_series = setup_chart(ui_Humidity_history, humidity_history, 0, 100);
  lv_label_set_text(ui_Temperature, "-- C");
  lv_label_set_text(ui_Humidity, "-- %");
  lv_label_set_text(ui_Temperature_bathroom, "-- C");
  lv_label_set_text(ui_Temperature_toilet, "-- C");
  lv_label_set_text(ui_Temperature_air_bathroom, "-- C");
  lv_label_set_text(ui_Humidity_bathroom, "-- %");
  app_set_bathroom_heating(false);
  app_set_toilet_heating(false);
  lv_obj_add_event_cb(ui_Bathroom_heating_enable, bathroom_heating_enable_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(ui_Toilet_heating_enable, toilet_heating_enable_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(ui_Spinbox_shower, bathroom_heat_target_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(ui_Spinbox_toilet, toilet_heat_target_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

void app_set_temperature(float celsius) {
  lv_label_set_text_fmt(ui_Temperature, "%.1f C", celsius);
  lv_chart_set_next_value(ui_Temperature_history, temperature_series, (lv_coord_t)(celsius + 0.5f));
}

void app_set_humidity(int percent) {
  lv_label_set_text_fmt(ui_Humidity, "%d %%", percent);
  lv_chart_set_next_value(ui_Humidity_history, humidity_series, percent);
}

void app_set_bathroom_temperature(float celsius) {
  lv_label_set_text_fmt(ui_Temperature_bathroom, "%.1f C", celsius);
}

void app_set_toilet_temperature(float celsius) {
  lv_label_set_text_fmt(ui_Temperature_toilet, "%.1f C", celsius);
}

void app_set_bathroom_air_temperature(float celsius) {
  lv_label_set_text_fmt(ui_Temperature_air_bathroom, "%.1f C", celsius);
}

void app_set_bathroom_humidity(int percent) {
  lv_label_set_text_fmt(ui_Humidity_bathroom, "%d %%", percent);
}

// Colors/glow lifted from the LED objects as exported by SquareLine.
static constexpr uint32_t LED_ON_COLOR = 0xFF0000;
static constexpr uint32_t LED_ON_SHADOW = 0xBE2C2C;
static constexpr uint32_t LED_OFF_COLOR = 0x330000;

static void set_led(lv_obj_t *led, bool on) {
  lv_obj_set_style_bg_color(led, lv_color_hex(on ? LED_ON_COLOR : LED_OFF_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_shadow_color(led, lv_color_hex(LED_ON_SHADOW), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_shadow_width(led, on ? 15 : 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_shadow_spread(led, on ? 2 : 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_shadow_opa(led, on ? 255 : 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void app_set_bathroom_heating(bool on) {
  set_led(ui_Bathroom_heating_LED, on);
}

void app_set_toilet_heating(bool on) {
  set_led(ui_Toilet_heating_LED, on);
}

void app_set_net_status(const char *text) {
  static lv_obj_t *label;
  if (!label) {
    label = lv_label_create(lv_layer_top());
    lv_obj_set_style_text_color(label, lv_color_hex(0x808080), 0);
    lv_obj_align(label, LV_ALIGN_BOTTOM_LEFT, 8, -6);
  }
  if (strcmp(lv_label_get_text(label), text) != 0) lv_label_set_text(label, text);
}
