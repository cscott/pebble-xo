#include <pebble.h>

static Window *s_window;

static TextLayer *s_time_layer;

static GBitmap *s_background_image;
static BitmapLayer *s_background_image_layer;

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
    // Create a long-lived buffer
    static char buffer[] = "00:00";
    clock_copy_time_string(buffer, sizeof(buffer));
    // Display this time on the TextLayer
    text_layer_set_text(s_time_layer, buffer);
}

static void main_window_load(Window *window) {
    time_t t = time(NULL);
    struct tm * tm = localtime(&t);
    GRect frame = GRect(0, 0, 144, 168);
    window_set_background_color(window, GColorBlack);
    // Init the background image
    s_background_image = gbitmap_create_with_resource(
        RESOURCE_ID_XO_BACKGROUND
    );
    s_background_image_layer = bitmap_layer_create(frame);
    bitmap_layer_set_bitmap(s_background_image_layer, s_background_image);
    layer_add_child(window_get_root_layer(window),
                    bitmap_layer_get_layer(s_background_image_layer));

    // Init the text layer used for the time
    s_time_layer = text_layer_create(GRect(2, 168-23, 144-4, 23));
    text_layer_set_text_color(s_time_layer, GColorWhite);
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_font(s_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_20)));
    layer_add_child(window_get_root_layer(window),
                    text_layer_get_layer(s_time_layer));

    // Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
    handle_minute_tick(tm, ~0);
}

static void main_window_unload(Window *window) {
    gbitmap_destroy(s_background_image);
    bitmap_layer_destroy(s_background_image_layer);
    text_layer_destroy(s_time_layer);
    tick_timer_service_unsubscribe();
}

static void init() {
    // Create main Window element and assign to pointer
    s_window = window_create();

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });

    // Show the Window on the watch, with animated=true
    window_stack_push(s_window, true /* Animated */);
}

static void deinit() {
    // Destroy Window
    window_destroy(s_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
