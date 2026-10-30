#include <Arduino.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>

#include "app.h"
#include "net.h"
#include "ui.h"

static constexpr uint16_t HISTORY_MAX = 64;
static constexpr int Y_INTERVALS = 4;  // 5 major Y ticks as set up in SquareLine

// A history chart filled from temperatury/wielicka_hist_*. Values are stored
// multiplied by `scale` (lv_coord_t is an integer); the Y range is fitted to the
// data, at least `min_span` units, in whole units per major tick.
struct HistoryChart {
  lv_obj_t *chart;
  lv_chart_series_t *ser;
  int scale;
  int min_span;
  uint32_t span_s;  // time covered by the series, 0 = no data yet
  lv_coord_t buf[HISTORY_MAX];
};

static HistoryChart temperature_chart = {nullptr, nullptr, 10, 2};
static HistoryChart pressure_chart = {nullptr, nullptr, 1, 4};

// Tick labels: Y in real units, X as hours back from now ("-24h" ... "teraz").
static void chart_draw_cb(lv_event_t *e) {
  lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
  if (!lv_obj_draw_part_check_type(dsc, &lv_chart_class, LV_CHART_DRAW_PART_TICK_LABEL) || !dsc->text) return;
  const HistoryChart *hc = static_cast<const HistoryChart *>(lv_event_get_user_data(e));

  if (dsc->id == LV_CHART_AXIS_PRIMARY_Y) {
    lv_snprintf(dsc->text, dsc->text_length, "%d", (int)(dsc->value / hc->scale));
  } else if (dsc->id == LV_CHART_AXIS_PRIMARY_X) {
    const int32_t back = Y_INTERVALS - dsc->value;  // the X axis also has 5 major ticks
    if (hc->span_s == 0) dsc->text[0] = '\0';
    else if (back == 0) lv_snprintf(dsc->text, dsc->text_length, "teraz");
    else lv_snprintf(dsc->text, dsc->text_length, "-%luh", (unsigned long)(hc->span_s * back / Y_INTERVALS / 3600));
  }
}

// Replace the generated demo data with an app-owned, initially empty buffer.
static void setup_chart(HistoryChart &hc, lv_obj_t *chart) {
  hc.chart = chart;
  hc.ser = lv_chart_get_series_next(chart, NULL);
  for (uint16_t i = 0; i < HISTORY_MAX; i++) hc.buf[i] = LV_CHART_POINT_NONE;
  lv_chart_set_point_count(chart, HISTORY_MAX);
  lv_chart_set_ext_y_array(chart, hc.ser, hc.buf);
  lv_obj_add_event_cb(chart, chart_draw_cb, LV_EVENT_DRAW_PART_BEGIN, &hc);
  lv_chart_refresh(chart);
}

static void set_history(HistoryChart &hc, const float *v, uint16_t count, uint32_t step_s) {
  if (count > HISTORY_MAX) {  // keep the newest points
    v += count - HISTORY_MAX;
    count = HISTORY_MAX;
  }

  float lo = INFINITY, hi = -INFINITY;
  for (uint16_t i = 0; i < count; i++) {
    if (isnan(v[i])) {
      hc.buf[i] = LV_CHART_POINT_NONE;
      continue;
    }
    hc.buf[i] = (lv_coord_t)lroundf(v[i] * hc.scale);
    lo = min(lo, v[i]);
    hi = max(hi, v[i]);
  }

  if (lo <= hi) {
    // Whole units, span a multiple of Y_INTERVALS so every tick label is an integer.
    int y_lo = (int)floorf(lo);
    int y_hi = (int)ceilf(hi);
    int span = max(y_hi - y_lo, hc.min_span);
    span = (span + Y_INTERVALS - 1) / Y_INTERVALS * Y_INTERVALS;
    y_lo -= (span - (y_hi - y_lo)) / 2;
    lv_chart_set_range(hc.chart, LV_CHART_AXIS_PRIMARY_Y, y_lo * hc.scale, (y_lo + span) * hc.scale);
    hc.span_s = (count - 1) * step_s;
  } else {
    hc.span_s = 0;  // no point with data
  }

  lv_chart_set_point_count(hc.chart, count);
  lv_chart_set_ext_y_array(hc.chart, hc.ser, hc.buf);
  lv_chart_refresh(hc.chart);
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
  // ui_Humidity / ui_Humidity_history (SquareLine names) now show pressure.
  setup_chart(temperature_chart, ui_Temperature_history);
  setup_chart(pressure_chart, ui_Humidity_history);
  lv_label_set_text(ui_Temperature, "-- C");
  // "1013 hPa" does not fit in one line left of the chart's Y tick labels (they
  // start ~x 133): unit on a second line, label at x 4..130.
  lv_obj_set_width(ui_Humidity, 126);
  lv_obj_set_height(ui_Humidity, LV_SIZE_CONTENT);
  lv_obj_set_x(ui_Humidity, 4 + 126 / 2 - LV_HOR_RES / 2);
  lv_label_set_long_mode(ui_Humidity, LV_LABEL_LONG_CLIP);
  lv_label_set_text(ui_Humidity, "--\nhPa");
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
}

void app_set_pressure(int hpa) {
  lv_label_set_text_fmt(ui_Humidity, "%d\nhPa", hpa);
}

void app_set_temperature_history(const float *v, uint16_t count, uint32_t step_s) {
  set_history(temperature_chart, v, count, step_s);
}

void app_set_pressure_history(const float *v, uint16_t count, uint32_t step_s) {
  set_history(pressure_chart, v, count, step_s);
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
