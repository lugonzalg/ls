#ifndef SYSTEM_FILE_H
#define SYSTEM_FILE_H

#include <sys/stat.h>
#include <stdbool.h>

typedef struct s_system_file {
    char        *name;
    struct stat stats;
} t_system_file;

#endif //SYSTEM_FILE_H