struct RoutineInfo {
  int numOfRoutines;
  int *numOfTasksPerRoutine;
  char ***newRoutines;
};

void store_init_if_none();
struct RoutineInfo store_get_routines(bool firstRun);
void freeStore();
void deleteStore();
void safeDelete(int key);
static void inbox_received_handler(DictionaryIterator *iter, void *context);
