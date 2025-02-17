#ifndef SYSTEM_FILE_H
#define SYSTEM_FILE_H

#include <sys/stat.h>
#include <stdbool.h>

typedef struct s_system_file {
    char        *real_name;
    char        *sort_name;
    struct stat stats;
} t_system_file;

#endif //SYSTEM_FILE_H