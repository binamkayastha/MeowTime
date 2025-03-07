#include <pebble.h>

Window *window;
TextLayer *text_layer_title;
TextLayer *text_layer_middle;
int current_routine_index = 0;


char *routine[] = {
  "Make Bed",
  "Drink Water",
  "Brush Teeth",
};

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  printf("Click handler %d\n", current_routine_index); // \n indicates a newline character
  int len = sizeof(routine)/sizeof(routine[0]);
  printf("sizeof %d\n", len); // \n indicates a newline character
  // A single click has just occured
  current_routine_index++;
  if (current_routine_index == len) {
    text_layer_set_text(text_layer_middle, "Done! Great Job");
  } else if (current_routine_index >= len) {
    text_layer_set_text(text_layer_middle, "TODO: Go back to home page");
  } else {
    text_layer_set_text(text_layer_middle, routine[current_routine_index]);
  }
}

static void click_config_provider(void *context) {
  printf("Click config provider\n"); // \n indicates a newline character
  // https://developer.rebble.io/developer.pebble.com/guides/events-and-services/buttons/index.html
  // Subcribe to button click events here
  ButtonId id = BUTTON_ID_SELECT;  // The Select button
  // ButtonId id = BUTTON_ID_DOWN;       // The Down button
  //     uint16_t repeat_interval_ms = 200;  // Fire every 200 ms while held down
  // window_single_repeating_click_subscribe(id, repeat_interval_ms,
                                                // down_repeating_click_handler);
  window_single_click_subscribe(id, select_click_handler);
}

void init() {
  window = window_create();

  // Add title
  text_layer_title = text_layer_create(GRect(0, 0, 144, 40));
  text_layer_set_text_alignment(text_layer_title, GTextAlignmentCenter);
  text_layer_set_text(text_layer_title, "ROUTINI");
  text_layer_set_font(text_layer_title, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_get_root_layer(window),
                    text_layer_get_layer(text_layer_title));


  // Add middletext
  text_layer_middle = text_layer_create(GRect(0, 70, 144, 40));
  text_layer_set_text_alignment(text_layer_middle, GTextAlignmentCenter);
  text_layer_set_text(text_layer_middle, routine[current_routine_index]);
  text_layer_set_font(text_layer_middle, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(window_get_root_layer(window),
                    text_layer_get_layer(text_layer_middle));


  // Add button click providers
  printf("Hello, World\n"); // \n indicates a newline character
  window_set_click_config_provider(window, click_config_provider);



  window_stack_push(window, true);
}

void deinit() {
  text_layer_destroy(text_layer_title);
  text_layer_destroy(text_layer_middle);
  window_destroy(window);
}

int main() {
  init();
  app_event_loop();
  deinit();
  return 0;
}
