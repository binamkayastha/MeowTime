#include "pebble.h"
#include "store.h"
#include "utils.h"
#include <inttypes.h>
#include <string.h>

void store_init_if_none() {
    app_message_open(app_message_inbox_size_maximum(), 0);
    app_message_register_inbox_received(inbox_received_handler);


    if (persist_exists(0)) {
        log_debug("Store exists!");
        log_debug("Store value: %" PRIu32 "", persist_read_int(0));
        return;
    } else {
        log_debug("Store doesn't exist!");
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
        log_debug("firstRoutine writing to %d", key);
        log_debug(firstRoutine[i]);
        persist_write_string(++key, firstRoutine[i]);
    }

    char *secondRoutine[] = {"Morning Routine", "Feed Cat", "Drink Water", "Brush Teeth", "Duo Lingo", "Shower", "Make bed", "Measure weight", "Eat breakfast"};
    numberOfTasks = len(secondRoutine);
    persist_write_int(++key, numberOfTasks);
    for (int i = 0; i < numberOfTasks; i++) {
        log_debug("secondRoutine writing to %d", key);
        log_debug(secondRoutine[i]);
        persist_write_string(++key, secondRoutine[i]);
    }

    char *thirdRoutine[] = {"Evening Routine", "Budgeting", "Put away dishes on table", "Brush teeth", "Fill water bottle for the night", "Journal", "Create plan for tomorrow", "Read Stuff", "Pet cat", "Put out work cloths", "Set alarm for tomorrow morning", "Put phone in drawer"};
    numberOfTasks = len(thirdRoutine);
    persist_write_int(++key, numberOfTasks);
    for (int i = 0; i < numberOfTasks; i++) {
        log_debug("thirdRoutine writing to %d", key);
        log_debug(thirdRoutine[i]);
        persist_write_string(++key, thirdRoutine[i]);
    }
}

void store_deinit() {
    app_message_deregister_callbacks();
    freeStore();
}

struct RoutineInfo routineInfo;

struct RoutineInfo store_get_routines(bool firstRun) {
    int key = 0;

    if (!firstRun) {
        freeStore();
    }

    routineInfo.numOfRoutines = persist_read_int(key);
    log_debug("routineInfo.numOfRoutines: %d", routineInfo.numOfRoutines);
    routineInfo.routineTaskCount = malloc(sizeof(int) * routineInfo.numOfRoutines);
    if (routineInfo.routineTaskCount == NULL) { log_debug("Failed to allocate for routineTaskCount"); }
    routineInfo.routineStartKeys = malloc(sizeof(int) * routineInfo.numOfRoutines);
    if (routineInfo.routineStartKeys == NULL) { log_debug("Failed to allocate for routineStartKeys"); }
    log_debug("key value is %d\n", key);

    for(int i=0; i < routineInfo.numOfRoutines; i++) {
        log_debug("in loop key value is %d\n", key);
        routineInfo.routineTaskCount[i] = persist_read_int(++key);
        log_debug("after getting index key value is %d\n", key);
        int routineNameIndex = key+1;
        log_debug("routineNameIndex should be 2 %d\n", routineNameIndex);
        routineInfo.routineStartKeys[i] = routineNameIndex;
        key += routineInfo.routineTaskCount[i];
        persist_read_string(routineNameIndex, routineInfo.routineNames[i], 20);
        log_debug("Routine name retrieved: %s at key %d\n", routineInfo.routineNames[i], routineNameIndex);
    }
    return routineInfo;
}


void freeStore() {
    free(routineInfo.routineTaskCount);
    free(routineInfo.routineStartKeys);
}

void deleteStore() {
    int key = 0;
    safeDelete(key);
    safeDelete(++key);
    for(int i=0; i < routineInfo.numOfRoutines; i++) {
        for(int j=0; j < routineInfo.routineTaskCount[i]; j++) {
            safeDelete(++key);
        }
    }
}

void safeDelete(int key) {
    if (persist_exists(key)) {
        persist_delete(key);
    }
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
    // Figure out tab stop per language file in vim
    Tuple *part = dict_find(iter, MESSAGE_KEY_PART);
    Tuple *key = dict_find(iter, MESSAGE_KEY_KEY);
    Tuple *value = dict_find(iter, MESSAGE_KEY_VALUE);

    if (strcmp(part->value->cstring, "start") == 0) {
        log_debug("start");
    } else if(strcmp(part->value->cstring, "key_pair") == 0) {

        log_debug("success message recieved in c with key %" PRIu32 "", key->value->int32);
        if(value->type == TUPLE_CSTRING) {
            log_debug("success message recieved in c with value %s", value->value->cstring);
            persist_write_string(key->value->int32, value->value->cstring);
        } else if (value -> type == TUPLE_INT) {
            log_debug("success message recieved in c with value %" PRIu32"", value->value->int32);
            persist_write_int(key->value->int32, value->value->int32);
        }
    } else if(strcmp(part->value->cstring, "end") == 0) {
        log_debug("end");
        store_get_routines(false);
    }
}
