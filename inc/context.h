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
    bool    flags[255];
    t_list  *head;
    size_t  total_file_len;  
} t_context;

int process_file_info(const char *ctx_dirname, t_context *ctx);

#endif //CONTEXT_H