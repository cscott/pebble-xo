#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x4D, 0x42, 0xA2, 0x55, 0x92, 0x22, 0x42, 0xE2, 0x9A, 0x66, 0x59, 0x98, 0x48, 0x30, 0x28, 0x39 }
PBL_APP_INFO(MY_UUID,
             "XO Clock", "cscott.net",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;

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
}

void handle_deinit(AppContextRef ctx) {
    (void)ctx;

    // Note: Failure to de-init this here will result in instability and
    //       unable to allocate memory errors.
    bmp_deinit_container(&image_container);
}

void pbl_main(void *params) {
    PebbleAppHandlers handlers = {
        .init_handler = &handle_init,
        .deinit_handler = &handle_deinit
    };
    app_event_loop(params, &handlers);
}
