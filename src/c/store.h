struct RoutineInfo {
  int numOfRoutines;
  // The task count includes routine name
  int *routineTaskCount;
  int *routineStartKeys;
  char routineNames[20][15];
};

void store_init_if_none();
void store_deinit();
struct RoutineInfo store_get_routines(bool firstRun);
void freeStore();
void deleteStore();
void safeDelete(int key);
static void inbox_received_handler(DictionaryIterator *iter, void *context);
