#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x4D, 0x42, 0xA2, 0x55, 0x92, 0x22, 0x42, 0xE2, 0x9A, 0x66, 0x59, 0x98, 0x48, 0x30, 0x28, 0x39 }
PBL_APP_INFO(MY_UUID,
             "XO Clock", "cscott.net",
             1, 0, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;

TextLayer text_time_layer;

BmpContainer image_container;

void handle_init(AppContextRef ctx) {
    (void)ctx;

    window_init(&window, "XO Clock");
    window_stack_push(&window, true /* Animated */);

    resource_init_current_app(&XO_IMAGE_RESOURCES);

    // Note: This needs to be "de-inited" in the application's
    //       deinit handler.
    bmp_init_container(RESOURCE_ID_IMAGE_XO_BACKGROUND, &image_container);

    layer_add_child(&window.layer, &image_container.layer.layer);

    text_layer_init(&text_time_layer, window.layer.frame);
    text_layer_set_text_color(&text_time_layer, GColorWhite);
    text_layer_set_text_alignment(&text_time_layer, GTextAlignmentCenter);
    text_layer_set_background_color(&text_time_layer, GColorClear);
    layer_set_frame(&text_time_layer.layer, GRect(2, 168-23, 144-2, 168-3));
    text_layer_set_font(&text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_20)));
    layer_add_child(&window.layer, &text_time_layer.layer);
}

void handle_deinit(AppContextRef ctx) {
    (void)ctx;

    // Note: Failure to de-init this here will result in instability and
    //       unable to allocate memory errors.
    bmp_deinit_container(&image_container);
}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {

  (void)ctx;

  // Need to be static because they're used by the system later.
  static char time_text[] = "00:00";

  char *time_format;


  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  string_format_time(time_text, sizeof(time_text), time_format, t->tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
      time_text[0] = ' ';
  }

  text_layer_set_text(&text_time_layer, time_text);

}


void pbl_main(void *params) {
    PebbleAppHandlers handlers = {
        .init_handler = &handle_init,
        .deinit_handler = &handle_deinit,
        .tick_info = {
            .tick_handler = &handle_minute_tick,
            .tick_units = MINUTE_UNIT
        }
    };
    app_event_loop(params, &handlers);
}
