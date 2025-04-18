#include "pebble.h"
#include "store.h"
#include <stdarg.h>
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

    int num_of_routines = 3;
    int key = 0;
    persist_write_int(key, num_of_routines);


    char *first_routine[] = {"Tutorial", "This is a routine", "Keep pressing right to see the next task!",
                            "You can add your own routines through the app settings in the pebble",
                            "Do your routines and Routini the cat will be pleased."};
    int num_of_tasks = len(first_routine);
    persist_write_int(++key, num_of_tasks);
    for (int i = 0; i < num_of_tasks; i++) {
        log_debug("first_routine writing to %d", key);
        log_debug(first_routine[i]);
        persist_write_string(++key, first_routine[i]);
    }

    char *second_routine[] = {"Morning Routine", "Feed Cat", "Drink Water", "Brush Teeth", "Duo Lingo", "Shower", "Make bed", "Measure weight", "Eat breakfast"};
    num_of_tasks = len(second_routine);
    persist_write_int(++key, num_of_tasks);
    for (int i = 0; i < num_of_tasks; i++) {
        log_debug("second_routine writing to %d", key);
        log_debug(second_routine[i]);
        persist_write_string(++key, second_routine[i]);
    }

    char *third_routine[] = {"Evening Routine", "Budgeting", "Put away dishes on table", "Brush teeth", "Fill water bottle for the night", "Journal", "Create plan for tomorrow", "Read Stuff", "Pet cat", "Put out work cloths", "Set alarm for tomorrow morning", "Put phone in drawer"};
    num_of_tasks = len(third_routine);
    persist_write_int(++key, num_of_tasks);
    for (int i = 0; i < num_of_tasks; i++) {
        log_debug("third_routine writing to %d", key);
        log_debug(third_routine[i]);
        persist_write_string(++key, third_routine[i]);
    }
}

void store_deinit() {
    app_message_deregister_callbacks();
    free_store();
}

struct RoutineInfo routines;

struct RoutineInfo store_get_routines(bool firstRun) {
    int key = 0;

    if (!firstRun) {
        free_store();
    }

    routines.num_of_routines = persist_read_int(key);
    log_debug("routineInfo.numOfRoutines: %d", routineInfo.numOfRoutines);
    routines.num_of_tasks = malloc(sizeof(int) * routines.num_of_routines);
    if (routines.num_of_tasks == NULL) { log_debug("Failed to allocate for routineTaskCount"); }
    routines.start_keys = malloc(sizeof(int) * routines.num_of_routines);
    if (routines.start_keys == NULL) { log_debug("Failed to allocate for routineStartKeys"); }
    log_debug("key value is %d\n", key);

    for(int i=0; i < routines.num_of_routines; i++) {
        log_debug("in loop key value is %d\n", key);
        routines.num_of_tasks[i] = persist_read_int(++key);
        log_debug("after getting index key value is %d\n", key);
        int routine_name_index = key+1;
        log_debug("routine_name_index should be 2 %d\n", routine_name_index);
        routines.start_keys[i] = routine_name_index;
        key += routines.num_of_tasks[i];
        persist_read_string(routine_name_index, routines.names[i], 20);
        log_debug("Routine name retrieved: %s at key %d\n", routineInfo.routineNames[i], routine_name_index);
    }
    return routines;
}


void free_store() {
    free(routines.num_of_tasks);
    free(routines.start_keys);
}

void deleteStore() {
    int key = 0;
    safe_delete(key);
    safe_delete(++key);
    for(int i=0; i < routines.num_of_routines; i++) {
        for(int j=0; j < routines.num_of_tasks[i]; j++) {
            safe_delete(++key);
        }
    }
}

void safe_delete(int key) {
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
