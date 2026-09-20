#include <Arduino.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>

#include "app.h"
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

void app_init(void) {
  temperature_series = setup_chart(ui_Temperature_history, temperature_history, 0, 50);
  humidity_series = setup_chart(ui_Humidity_history, humidity_history, 0, 100);
  lv_label_set_text(ui_Temperature, "-- C");
  lv_label_set_text(ui_Humidity, "-- %");
  lv_label_set_text(ui_Temperature_bathroom, "-- C");
  lv_label_set_text(ui_Temperature_toilet, "-- C");
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

void app_set_net_status(const char *text) {
  static lv_obj_t *label;
  if (!label) {
    label = lv_label_create(lv_layer_top());
    lv_obj_set_style_text_color(label, lv_color_hex(0x808080), 0);
    lv_obj_align(label, LV_ALIGN_BOTTOM_LEFT, 8, -6);
  }
  if (strcmp(lv_label_get_text(label), text) != 0) lv_label_set_text(label, text);
}
