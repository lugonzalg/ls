#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdbool.h>

# define FLAGS "alrRt"

typedef struct s_context {
    bool a; //all
    bool l; //list
    bool r; //reverse
    bool R; //recursive
    bool t; // sort by time
} t_context;

#endif //CONTEXT_H