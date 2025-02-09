#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdbool.h>

#include "libft.h"

# define FLAGS        "alrRt"
# define ALL          'a'
# define REVERSE      'r'
# define RECURSIVE    'R'
# define LIST         'l'
# define SORT_BY_TIME 't'

typedef struct s_context {
    bool            flags[255];
    struct s_list   *head;
} t_context;

#endif //CONTEXT_H