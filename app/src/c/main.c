#include <pebble.h>
#include "anim_number.h"
#include "anim_colour.h"
#include "anim_vehicle.h"
#include "data.h"

// Define constants for Basalt layout
#define RIGHT_BAR_WIDTH 40
#define RIGHT_MARGIN 5
#define SPACE 4
#define DELTA 10
#define MAX_ROUTES 10

// Declare global variables
static Window *s_window;
static TextLayer *s_time_layer;
static Layer *s_vehicle_layer, *s_loading_layer;
static GDrawCommandImage *s_plane_icon, *s_generic_failed_icon, *s_no_internet_icon;
static GDrawCommandSequence *s_streetcar_sequence, *s_subway_sequence, *s_bus_sequence, *s_regional_train_sequence;
static GTextAttributes *s_loading_text_attributes;
static WindowDataArray sample_data_arr;

// Function prototypes
static void set_time_text(WindowDataArray *data_arr) {
    static char time_buffer[8];
    snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d", 12, 34); // Example time
    text_layer_set_text(s_time_layer, time_buffer);
}

static void set_error_text(WindowDataArray *data_arr) {
    text_layer_set_text(s_time_layer, "Error");
}

static void vehicle_update_proc(Layer *layer, GContext *ctx) {
    graphics_context_set_fill_color(ctx, GColorRed);
    graphics_fill_circle(ctx, GPoint(20, 20), 10); // Example drawing
}

static void loading_layer_update_proc(Layer *layer, GContext *ctx) {
    graphics_context_set_text_color(ctx, GColorBlack);
    graphics_draw_text(ctx, "Loading...", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
        GRect(0, 50, layer_get_bounds(layer).size.w, 30), GTextOverflowModeWordWrap,
        GTextAlignmentCenter, s_loading_text_attributes);
}

// Adjust layout when creating layers
static void create_time_layer(GRect bounds, WindowDataArray *data_arr) {
    s_time_layer = text_layer_create(GRect(0, 0, bounds.size.w - RIGHT_BAR_WIDTH - 2, 58));
    set_time_text(data_arr);
    text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
}

static void create_vehicle(GRect bounds) {
    s_vehicle_layer = layer_create(GRect(bounds.size.w - RIGHT_BAR_WIDTH, 0, RIGHT_BAR_WIDTH, bounds.size.h - 10));
    layer_set_update_proc(s_vehicle_layer, vehicle_update_proc);
}

static void create_loading_layer(GRect bounds) {
    s_loading_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h - 20));
    layer_set_update_proc(s_loading_layer, loading_layer_update_proc);
    s_loading_text_attributes = graphics_text_attributes_create();
    graphics_text_attributes_enable_screen_text_flow(s_loading_text_attributes, 5);
}

// Handlers for window lifecycle
static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    // Create layers
    create_time_layer(bounds, window_get_user_data(window));
    create_vehicle(bounds);
    create_loading_layer(bounds);

    // Add layers to the window
    layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
    layer_add_child(window_layer, s_vehicle_layer);
    layer_add_child(window_layer, s_loading_layer);
}

static void window_unload(Window *window) {
    text_layer_destroy(s_time_layer);
    layer_destroy(s_vehicle_layer);
    layer_destroy(s_loading_layer);
    graphics_text_attributes_destroy(s_loading_text_attributes);
}

// Initialize the app
static void init(void) {
    sample_data_arr.array = malloc(MAX_ROUTES * sizeof(WindowData));
    for (int i = 0; i < MAX_ROUTES; i++) {
        sample_data_arr.array[i] = (WindowData){
            .time = 10,
            .unit = malloc(32 * sizeof(char)),
            .stop_name = malloc(32 * sizeof(char)),
            .dest_name = malloc(32 * sizeof(char)),
            .route_number = malloc(32 * sizeof(char)),
            .route_name = malloc(32 * sizeof(char)),
            .vehicle_type = STREETCAR,
            .color = GColorRed,
            .shape = ROUNDRECT,
        };
    }
    sample_data_arr.data_index = 0;

    set_error_text(&sample_data_arr);

    s_plane_icon = gdraw_command_image_create_with_resource(RESOURCE_ID_PLANE);
    s_generic_failed_icon = gdraw_command_image_create_with_resource(RESOURCE_ID_GENERIC_FAILED);
    s_no_internet_icon = gdraw_command_image_create_with_resource(RESOURCE_ID_NO_INTERNET);
    s_streetcar_sequence = gdraw_command_sequence_create_with_resource(RESOURCE_ID_STREETCAR_ANIM);
    s_subway_sequence = gdraw_command_sequence_create_with_resource(RESOURCE_ID_SUBWAY_ANIM);
    s_bus_sequence = gdraw_command_sequence_create_with_resource(RESOURCE_ID_BUS_ANIM);
    s_regional_train_sequence = gdraw_command_sequence_create_with_resource(RESOURCE_ID_TRAIN_ANIM);

    app_message_register_inbox_received(NULL); // Placeholder callback
    const int inbox_size = 2048;
    const int outbox_size = 32;
    app_message_open(inbox_size, outbox_size);

    s_window = window_create();
    window_set_click_config_provider(s_window, NULL); // Placeholder callback
    window_set_user_data(s_window, &sample_data_arr);
    window_set_window_handlers(s_window, (WindowHandlers){
        .load = window_load,
        .unload = window_unload
    });
    const bool animated = true;
    window_stack_push(s_window, animated);
}

// Deinitialize the app
static void deinit(void) {
    window_destroy(s_window);
    for (int i = 0; i < MAX_ROUTES; i++) {
        free(sample_data_arr.array[i].unit);
        free(sample_data_arr.array[i].stop_name);
        free(sample_data_arr.array[i].dest_name);
        free(sample_data_arr.array[i].route_number);
        free(sample_data_arr.array[i].route_name);
    }
    free(sample_data_arr.array);
}

// Main entry point
int main(void) {
    init();
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);
    app_event_loop();
    deinit();
}