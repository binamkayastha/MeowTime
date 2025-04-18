#include "main.h"
#include "pebble.h"
#include "store.h"
#include <stdarg.h>
#include "utils.h"
#include <stdio.h>
#include <string.h>

#define NUM_MENU_ICONS 3
#define CURRENT_TASK_LENGTH 100

char progress[100];

TextLayer *text_layer_bottom;
TextLayer *text_layer_middle;

static BitmapLayer *s_bitmap_layer_cat;
static GBitmap *s_background_bitmap;
static GBitmap *s_bitmap;
static GBitmap *s_bitmap_cat;
static GBitmap *s_menu_icons[NUM_MENU_ICONS];

static MenuLayer *s_menu_layer;
static Window *s_main_window;
static Window *s_routine_window;

static int s_current_icon = 0;
int current_routine = 0;
int current_routine_index = 0;
char currentTask[CURRENT_TASK_LENGTH];

int main(void) {
    init();
    app_event_loop();
    deinit();
}

struct RoutineInfo routines;
static void init() {
    store_init_if_none();
    routines = store_get_routines(true);

    create_and_push_main_window();
    create_routine_window();
}

static void deinit() {
    // text_layer_destroy(text_layer_title);
    text_layer_destroy(text_layer_bottom);
    text_layer_destroy(text_layer_middle);

    bitmap_layer_destroy(s_bitmap_layer_cat);
    gbitmap_destroy(s_background_bitmap);
    gbitmap_destroy(s_bitmap);
    gbitmap_destroy(s_bitmap_cat);
    gbitmap_destroy(s_menu_icons[0]);
    gbitmap_destroy(s_menu_icons[1]);
    gbitmap_destroy(s_menu_icons[2]);

    menu_layer_destroy(s_menu_layer);
    window_destroy(s_main_window);
    window_destroy(s_routine_window);

    store_deinit();
}

static void create_and_push_main_window() {
    s_main_window = window_create();
    
    // Here we load the bitmap assets
    s_menu_icons[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ONE);
    s_menu_icons[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TWO);
    s_menu_icons[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_THREE);

    s_bitmap_cat = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_THREE);

    // And also load the background
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_BAR_ICON_CHECK);

    // Now we prepare to initialize the menu layer
    Layer *window_layer = window_get_root_layer(s_main_window);
    GRect bounds = layer_get_frame(window_layer);

    // Create the menu layer
    s_menu_layer = menu_layer_create(bounds);
    menu_layer_set_callbacks(s_menu_layer, NULL,
                                (MenuLayerCallbacks){
                                    .get_num_rows = menu_get_num_rows_callback,
                                    .get_header_height = menu_get_header_height_callback,
                                    .draw_header = menu_draw_header_callback,
                                    .draw_row = menu_draw_row_callback,
                                    .select_click = menu_select_callback,
                                });

    // Bind the menu layer's click config provider to the window for interactivity
    menu_layer_set_click_config_onto_window(s_menu_layer, s_main_window);
    layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
    
    window_stack_push(s_main_window, true);
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return routines.num_of_routines;
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    menu_cell_basic_header_draw(ctx, cell_layer, "           Meow Time");
}

static void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    if (cell_index->section == 0 && cell_index->row < routines.num_of_routines) {
        menu_cell_basic_draw(ctx, cell_layer, routines.names[cell_index->row], NULL, NULL);
    }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    log_debug("menus eledcted");
    log_debug("%d", cell_index->row);
    if (cell_index->row < routines.num_of_routines) {
        s_current_icon = (s_current_icon + 1) % NUM_MENU_ICONS;
        // After changing the icon, mark the layer to have it updated
        layer_mark_dirty(menu_layer_get_layer(menu_layer));
        current_routine = cell_index->row;
        current_routine_index = 0;
        update_text();
        window_stack_push(s_routine_window, true);
    }
}

static void create_routine_window() {
    s_routine_window = window_create();
    window_set_click_config_provider(s_routine_window, routine_window_click_config_provider);

    // Add middle text layer
    text_layer_middle = text_layer_create(GRect(0, 20, 144, 200));
    text_layer_set_text_alignment(text_layer_middle, GTextAlignmentCenter);
    text_layer_set_overflow_mode(text_layer_middle, GTextOverflowModeWordWrap);
    text_layer_set_text(text_layer_middle, routines.names[current_routine_index]);
    text_layer_set_font(text_layer_middle, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    layer_add_child(window_get_root_layer(s_routine_window), text_layer_get_layer(text_layer_middle));

    // Add bottom text layer
    text_layer_bottom = text_layer_create(GRect(0, 140, 144, 400));
    text_layer_set_text_alignment(text_layer_bottom, GTextAlignmentCenter);
    text_layer_set_overflow_mode(text_layer_bottom, GTextOverflowModeWordWrap);
    text_layer_set_font(text_layer_bottom, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    layer_add_child(window_get_root_layer(s_routine_window), text_layer_get_layer(text_layer_bottom));

    // Create bitmap layer for static cat
    s_bitmap_layer_cat = bitmap_layer_create(GRect(35, 5, 100, 139));
    bitmap_layer_set_compositing_mode(s_bitmap_layer_cat, GCompOpSet);
    bitmap_layer_set_bitmap(s_bitmap_layer_cat, s_bitmap_cat);
}

static void routine_window_click_config_provider(void *context) {
    log_debug("Click config provider\n"); 
    ButtonId id = BUTTON_ID_SELECT; // The Select button
    window_single_click_subscribe(id, select_click_handler);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    log_debug("Click handler %d\n", current_routine_index);
    update_text();
}


static void update_text() {
    int len = routines.num_of_tasks[current_routine];
    log_debug("sizeof %d\n", len); // \n indicates a newline character
    // A single click has just occured
    current_routine_index++;
    vibes_short_pulse();
    log_debug("Right before text change.");
    if (current_routine_index == len) {
        text_layer_set_text(text_layer_middle, "");
        text_layer_set_text(text_layer_bottom, "Routini meows");
        layer_add_child(window_get_root_layer(s_routine_window), bitmap_layer_get_layer(s_bitmap_layer_cat));
    } else if (current_routine_index >= len || current_routine_index == -1) {
        current_routine_index = 0;
        layer_remove_from_parent(bitmap_layer_get_layer(s_bitmap_layer_cat));
        window_stack_pop(true);
    } else {
        persist_read_string(routines.start_keys[current_routine] + current_routine_index, currentTask, CURRENT_TASK_LENGTH);
        text_layer_set_text(text_layer_middle, currentTask);
        log_debug("setting text to %s", currentTask);
        snprintf(progress, 10, "%d/%d", current_routine_index, len - 1);
        text_layer_set_text(text_layer_bottom, progress);
    }
}
