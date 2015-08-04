#include <pebble.h>

static Window *window;
static TextLayer *s_battery_layer;
static TextLayer *s_time_layer;

static GBitmap *s_bitmap;
static BitmapLayer *s_bitmap_layer;

static void handle_battery(BatteryChargeState charge_state) {
  static char battery_text[] = "100%";

  snprintf(battery_text, sizeof(battery_text), "%d%%", charge_state.charge_percent);
  text_layer_set_text(s_battery_layer, battery_text);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
  handle_battery(battery_state_service_peek());
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BG);

  s_bitmap_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
#ifdef PBL_PLATFORM_APLITE
  bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpAssign);
#elif PBL_PLATFORM_BASALT
  bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
#endif
  layer_add_child(window_layer, bitmap_layer_get_layer(s_bitmap_layer));

  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(-5, 75, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, COLOR_FALLBACK(GColorWhite, GColorBlack));
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

  // Create battery TextLayer
  s_battery_layer = text_layer_create(GRect(114, 136, 30, 34));
  text_layer_set_text_color(s_battery_layer, COLOR_FALLBACK(GColorWhite, GColorBlack));
  text_layer_set_background_color(s_battery_layer, GColorClear);
  text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentCenter);
  text_layer_set_text(s_battery_layer, "100%");
  layer_add_child(window_layer, text_layer_get_layer(s_battery_layer));

  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_state_service_subscribe(handle_battery);
}

static void window_unload(Window *window) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  text_layer_destroy(s_time_layer);
  bitmap_layer_destroy(s_bitmap_layer);
  gbitmap_destroy(s_bitmap);
}

static void init(void) {
  window = window_create();

  window_set_background_color(window, COLOR_FALLBACK(GColorBlack, GColorWhite));

  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
