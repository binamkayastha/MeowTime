struct RoutineInfo {
  int numOfRoutines;
  int *numOfTasksPerRoutine;
  char ***newRoutines;
};

void store_init_if_none();
struct RoutineInfo store_load_routines(bool firstRun);
void freeStore();
void deleteStore();
void safeDelete(int key);
