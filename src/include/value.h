#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

typedef struct value_t {
    enum {
        TAG_BOOL,
        TAG_INTEGER,
        TAG_FLOAT,
        TAG_STRING,
        TAG_FUNCTION,
        TAG_STRUCT,
    } tag;
    bool is_pointer;
    const char* name;
} value_t;

#endif // !VALUE_H
