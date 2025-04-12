int main(void);

static void init();
static void deinit();

static void routine_window_click_config_provider(void *context);

static void create_and_push_main_window();

static void main_window_load(Window *window);
static void main_window_unload(Window *window);

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data);
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
static void menu_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *data);
static void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data);
static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);

static void select_click_handler(ClickRecognizerRef recognizer, void *context);
static void update_text();
static void timer_handler(void *context);
