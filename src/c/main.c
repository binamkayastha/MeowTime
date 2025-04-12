#include "main.h"
#include "pebble.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define NUM_MENU_SECTIONS 1
#define NUM_MENU_ICONS 3
#define NUM_FIRST_MENU_ITEMS 6
#define NUM_SECOND_MENU_ITEMS 0

const int counts[] = {9, 12, 22, 10, 6, 13};
const char *routines[][100] = {
    {"Morning Routine", "Feed Cat", "Drink Water", "Brush Teeth", "Duo Lingo", "Shower", "Make bed", "Measure weight",
     "Eat breakfast"},
    {"Evening Routine", "Budgeting", "Put away dishes on table", "Brush teeth", "Fill water bottle for the night",
     "Journal", "Create plan for tomorrow", "Read Stuff", "Pet cat", "Put out work cloths",
     "Set alarm for tomorrow morning", "Put phone in drawer"},
    {"Lunchtime Poutine",
     "Source: seasonsandsuppers.ca/authentic-canadian-poutine-recipe/",
     "Gather the following ingredients",
     "3 Tbsp cornstarch",
     "2 Tbsp water",
     "6 Tbsp unsalted butter",
     "1/4 cup unbleached all purpose flour",
     "20 oz beef broth",
     "10 oz chicken broth",
     "Pepper, to taste",
     "Prepare the gravy: In bowl, dissolve cornstarch in water. Set aside.",
     "In saucepan, melt butter.",
     "Add flour and cook, stir regularly, 5 minutes, till it turns golden "
     "brown.",
     "Add beef and chicken broth, bring to a boil, stirring with a whisk.",
     "Stir in about HALF the cornstarch mixture and simmer for a minute or so.",
     "Season with pepper. Add salt, if necessary, to taste.",
     "Keep gravy warm till fries are ready",
     "Add baked fries to a large, clean bowl. Season lightly with salt while "
     "still warm.",
     "Add a ladle of hot poutine gravy to the bowl and using tongs, toss the "
     "fries in the gravy.",
     "Add more gravy, as needed to mostly coat the fries.",
     "Add the cheese curds and toss with the hot fries and gravy.",
     "Serve with freshly ground pepper. Serve immediately. Serve to cat (if "
     "safe)."},
    {"Cat Yoga", "Lay on back", "Cat tail pose left", "Cat tail pose right", "Cat Cow", "Extended Cat Pose left",
     "Extended Cat Pose right", "Cat Cow", "Cobra", "Child Pose"},
    {"Feed Cat", "Feed Cat", "Pet Cat", "Groom Cat", "Open catnip drawer and leave it open", "Go to work"},
    {"Huh?", "What?", "Huh?", "Oh no", "It seem like my cat Routini has taken over the app :o",
     "Don't worry, we'll get this sorted out post hackathon with real "
     "templates",
     "Thanks for trying out my app!",
     "I'm very tired, it's 5:03 AM and I've been up all night making cat "
     "checklists",
     "I don't even have a cat. I don't even know if they can eat poutine.", "Thanks for reading so far!",
     "I hope you like it!", "And if you have suggestions, send them my way :D", "Email: binamkayastha@gmail.com"}};

char progress[100];

static Window *s_main_window;
static Window *s_routine_window;
// TextLayer *text_layer_title;
TextLayer *text_layer_middle;
TextLayer *text_layer_bottom;

static MenuLayer *s_menu_layer;
static GBitmap *s_menu_icons[NUM_MENU_ICONS];
static GBitmap *s_background_bitmap;
static GBitmapSequence *s_sequence;
static GBitmap *s_bitmap;
static BitmapLayer *s_bitmap_layer;

static GBitmap *s_bitmap_cat;
static BitmapLayer *s_bitmap_layer_cat;

uint32_t first_delay_ms = 10;
static int s_current_icon = 0;
int current_routine = 0;
int current_routine_index = 0;

int main(void) {
    init();
    app_event_loop();
    deinit();
}

static void init() {
    create_and_push_main_window();

    s_routine_window = window_create();
    window_set_click_config_provider(s_routine_window, routine_window_click_config_provider);

    text_layer_middle = text_layer_create(GRect(0, 20, 144, 200));
    text_layer_set_text_alignment(text_layer_middle, GTextAlignmentCenter);
    text_layer_set_overflow_mode(text_layer_middle, GTextOverflowModeWordWrap);
    text_layer_set_text(text_layer_middle, routines[current_routine_index][0]);
    // text_layer_set_font(text_layer_middle,
    // fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_font(text_layer_middle, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    layer_add_child(window_get_root_layer(s_routine_window), text_layer_get_layer(text_layer_middle));

    text_layer_bottom = text_layer_create(GRect(0, 140, 144, 400));
    text_layer_set_text_alignment(text_layer_bottom, GTextAlignmentCenter);
    text_layer_set_overflow_mode(text_layer_bottom, GTextOverflowModeWordWrap);
    // text_layer_set_text(text_layer_bottom, routines[current_routine_index][0]);
    text_layer_set_font(text_layer_bottom, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    layer_add_child(window_get_root_layer(s_routine_window), text_layer_get_layer(text_layer_bottom));

    s_bitmap_layer = bitmap_layer_create(GRect(5, 5, 48, 48));

    s_sequence = gbitmap_sequence_create_with_resource(RESOURCE_ID_IMAGE_CAT);
    // Create blank GBitmap using APNG frame size
    GSize frame_size = gbitmap_sequence_get_bitmap_size(s_sequence);
    s_bitmap = gbitmap_create_blank(frame_size, GBitmapFormat8Bit);
    bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
    bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
    layer_add_child(window_get_root_layer(s_routine_window), bitmap_layer_get_layer(s_bitmap_layer));

    layer_add_child(window_get_root_layer(s_routine_window), bitmap_layer_get_layer(s_bitmap_layer));

    s_bitmap_layer_cat = bitmap_layer_create(GRect(35, 5, 100, 139));
    bitmap_layer_set_compositing_mode(s_bitmap_layer_cat, GCompOpSet);
    bitmap_layer_set_bitmap(s_bitmap_layer_cat, s_bitmap_cat);
}

static void deinit() {
    // text_layer_destroy(text_layer_title);
    text_layer_destroy(text_layer_middle);
    text_layer_destroy(text_layer_bottom);
    window_destroy(s_main_window);
    window_destroy(s_routine_window);
    gbitmap_sequence_destroy(s_sequence);
    gbitmap_destroy(s_bitmap);
    bitmap_layer_destroy(s_bitmap_layer);
    gbitmap_destroy(s_bitmap_cat);
    bitmap_layer_destroy(s_bitmap_layer_cat);
}

static void create_and_push_main_window() {
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers){
                                                  .load = main_window_load,
                                                  .unload = main_window_unload,
                                              });
    window_stack_push(s_main_window, true);
}

static void routine_window_click_config_provider(void *context) {
    printf("Click config provider\n"); // \n indicates a newline character
    // https://developer.rebble.io/developer.pebble.com/guides/events-and-services/buttons/index.html
    // Subcribe to button click events here
    ButtonId id = BUTTON_ID_SELECT; // The Select button
    // ButtonId id = BUTTON_ID_DOWN;       // The Down button
    //     uint16_t repeat_interval_ms = 200;  // Fire every 200 ms while held
    //     down
    // window_single_repeating_click_subscribe(id, repeat_interval_ms,
    // down_repeating_click_handler);
    window_single_click_subscribe(id, select_click_handler);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    printf("Click handler %d\n",
           current_routine_index); // \n indicates a newline character
    update_text();
}

static void main_window_load(Window *window) {
    // Here we load the bitmap assets
    s_menu_icons[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ONE);
    s_menu_icons[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TWO);
    s_menu_icons[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_THREE);

    s_bitmap_cat = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_THREE);

    // And also load the background
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_BAR_ICON_CHECK);

    // Now we prepare to initialize the menu layer
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_frame(window_layer);

    // Create the menu layer
    s_menu_layer = menu_layer_create(bounds);
    menu_layer_set_callbacks(s_menu_layer, NULL,
                             (MenuLayerCallbacks){
                                 .get_num_sections = menu_get_num_sections_callback,
                                 .get_num_rows = menu_get_num_rows_callback,
                                 .get_header_height = menu_get_header_height_callback,
                                 .draw_header = menu_draw_header_callback,
                                 .draw_row = menu_draw_row_callback,
                                 .select_click = menu_select_callback,
                             });

    // Bind the menu layer's click config provider to the window for interactivity
    menu_layer_set_click_config_onto_window(s_menu_layer, window);

    layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void main_window_unload(Window *window) {
    // Destroy the menu layer
    menu_layer_destroy(s_menu_layer);

    // Cleanup the menu icons
    for (int i = 0; i < NUM_MENU_ICONS; i++) {
        gbitmap_destroy(s_menu_icons[i]);
    }

    gbitmap_destroy(s_background_bitmap);
}

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) { return NUM_MENU_SECTIONS; }

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    switch (section_index) {
    case 0:
        return NUM_FIRST_MENU_ITEMS;
    case 1:
        return NUM_SECOND_MENU_ITEMS;
    default:
        return 0;
    }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    // Determine which section we're working with
    switch (section_index) {
    case 0:
        // Draw title text in the section header
        menu_cell_basic_header_draw(ctx, cell_layer, "           Meow Time");
        break;
    case 1:
        menu_cell_basic_header_draw(ctx, cell_layer, "One more");
        break;
    }
}

static void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    if (cell_index->section == 0 && cell_index->row < 6) {
        menu_cell_basic_draw(ctx, cell_layer, routines[cell_index->row][0], NULL, NULL);
    }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    printf("menus eledcted");
    printf("%d", cell_index->row);
    if (cell_index->row < 6) {
        s_current_icon = (s_current_icon + 1) % NUM_MENU_ICONS;
        // After changing the icon, mark the layer to have it updated
        layer_mark_dirty(menu_layer_get_layer(menu_layer));
        current_routine = cell_index->row;
        current_routine_index = 0;
        update_text();
        window_stack_push(s_routine_window, true);
    }
}

static void update_text() {
    int len = counts[current_routine];
    printf("sizeof %d\n", len); // \n indicates a newline character
    // A single click has just occured
    current_routine_index++;
    vibes_short_pulse();
    printf("Right before text change.");
    if (current_routine_index == len) {
        text_layer_set_text(text_layer_middle, "");
        text_layer_set_text(text_layer_bottom, "Routini meows");
        layer_add_child(window_get_root_layer(s_routine_window), bitmap_layer_get_layer(s_bitmap_layer_cat));
        app_timer_register(first_delay_ms, timer_handler, NULL);
    } else if (current_routine_index >= len || current_routine_index == -1) {
        current_routine_index = 0;
        layer_remove_from_parent(bitmap_layer_get_layer(s_bitmap_layer_cat));
        window_stack_pop(true);
    } else {
        text_layer_set_text(text_layer_middle, routines[current_routine][current_routine_index]);
        printf("setting text to %s", routines[current_routine][current_routine_index]);
        snprintf(progress, 10, "%d/%d", current_routine_index, len - 1);
        text_layer_set_text(text_layer_bottom, progress);
    }
}

static void timer_handler(void *context) {
    // This is for the gif, but it didn't really work
    uint32_t next_delay;

    // Advance to the next APNG frame, and get the delay for this frame
    if (gbitmap_sequence_update_bitmap_next_frame(s_sequence, s_bitmap, &next_delay)) {
        // Set the new frame into the BitmapLayer
        bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
        layer_mark_dirty(bitmap_layer_get_layer(s_bitmap_layer));

        // Timer for that frame's delay
        app_timer_register(next_delay, timer_handler, NULL);
    }
}
