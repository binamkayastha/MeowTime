#include "pebble.h"
#include "store.h"
#include "utils.h"
#include <inttypes.h>
#include <string.h>

void store_init_if_none() {
    if (persist_exists(0)) {
        printf("Store exists!");
        printf("Store value: %" PRIu32 "", persist_read_int(0));
        return;
    } else {
        printf("Store doesn't exist!");
    }

    int numberOfRoutines = 3;
    int key = 0;
    persist_write_int(key, numberOfRoutines);


    char *firstRoutine[] = {"Tutorial", "This is a routine", "Keep pressing right to see the next task!",
                            "You can add your own routines through the app settings in the pebble",
                            "Do your routines and Routini the cat will be pleased."};
    int numberOfTasks = len(firstRoutine);
    persist_write_int(++key, numberOfTasks);
    for (int i = 0; i < numberOfTasks; i++) {
        printf("firstRoutine writing to %d", key);
        printf(firstRoutine[i]);
        persist_write_string(++key, firstRoutine[i]);
    }

    char *secondRoutine[] = {"Morning Routine", "Feed Cat", "Drink Water", "Brush Teeth", "Duo Lingo", "Shower", "Make bed", "Measure weight", "Eat breakfast"};
    numberOfTasks = len(secondRoutine);
    persist_write_int(++key, numberOfTasks);
    for (int i = 0; i < numberOfTasks; i++) {
        printf("secondRoutine writing to %d", key);
        printf(secondRoutine[i]);
        persist_write_string(++key, secondRoutine[i]);
    }

    char *thirdRoutine[] = {"Evening Routine", "Budgeting", "Put away dishes on table", "Brush teeth", "Fill water bottle for the night", "Journal", "Create plan for tomorrow", "Read Stuff", "Pet cat", "Put out work cloths", "Set alarm for tomorrow morning", "Put phone in drawer"};
    numberOfTasks = len(thirdRoutine);
    persist_write_int(++key, numberOfTasks);
    for (int i = 0; i < numberOfTasks; i++) {
        printf("thirdRoutine writing to %d", key);
        printf(thirdRoutine[i]);
        persist_write_string(++key, thirdRoutine[i]);
    }
}


int numOfRoutines;
int *numOfTasksPerRoutine;
char ***newRoutines;
struct RoutineInfo routineInfo;

struct RoutineInfo store_get_routines(bool firstRun) {
    int key = 0;

    if (!firstRun) {
        freeStore();
    }

    numOfRoutines = persist_read_int(key);
    newRoutines = malloc(sizeof(char**) * numOfRoutines);
    numOfTasksPerRoutine = malloc(sizeof(int) * numOfRoutines);

    for(int i=0; i < numOfRoutines; i++) {
        numOfTasksPerRoutine[i] = persist_read_int(++key);
        newRoutines[i] = malloc(sizeof(char*) * numOfTasksPerRoutine[i]);
        printf("Reading next Routine");

        for(int j=0; j < numOfTasksPerRoutine[i]; j++) {
            int size_of_string = persist_get_size(++key);
            newRoutines[i][j] = malloc(sizeof(char)*size_of_string);
            persist_read_string(key, newRoutines[i][j], size_of_string);
            printf(newRoutines[i][j]);
        }
    }
    routineInfo = (struct RoutineInfo){ numOfRoutines, numOfTasksPerRoutine, newRoutines };
    return routineInfo;
}


void freeStore() {
    for(int i=0; i < routineInfo.numOfRoutines; i++) {
        for(int j=0; j < routineInfo.numOfTasksPerRoutine[i]; j++) {
            free(routineInfo.newRoutines[i][j]);
        }
        free(routineInfo.newRoutines[i]);
    }
    free(routineInfo.newRoutines);
    free(routineInfo.numOfTasksPerRoutine);
}

void deleteStore() {
    int key = 0;
    safeDelete(key);
    safeDelete(++key);
    for(int i=0; i < numOfRoutines; i++) {
        for(int j=0; j < numOfTasksPerRoutine[i]; j++) {
            safeDelete(++key);
        }
    }
}

void safeDelete(int key) {
    if (persist_exists(key)) {
        persist_delete(key);
    }
}

