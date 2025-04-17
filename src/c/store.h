struct RoutineInfo {
  int num_of_routines;
  // The task count includes routine name
  int *num_of_tasks;
  // The store keys for each routine where the routine starts
  int *start_keys;
  char names[20][15];
};

void store_init_if_none();
void store_deinit();
struct RoutineInfo store_get_routines(bool firstRun);
void free_store();
void delete_store();
void safe_delete(int key);
static void inbox_received_handler(DictionaryIterator *iter, void *context);
