// Note this define will return the length of the array if has not been decayed to a pointer yet
// Otherwise it will return the length of the first element (which should always be 1 theoretically))
#define len(arr) (int)( sizeof(arr) / sizeof(arr[0]))
// #define DEBUG
#ifdef DEBUG
#define log_debug(...) printf(__VA_ARGS__)
#else
#define log_debug(...)
#endif
#define log_info(...) printf(__VA_ARGS__)
