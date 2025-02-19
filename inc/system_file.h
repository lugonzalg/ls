#ifndef SYSTEM_FILE_H
#define SYSTEM_FILE_H

#include <sys/stat.h>
#include <stdbool.h>

#include "context.h"

typedef struct s_system_file {
    char        *name;
    struct stat stats;
    long long   time_ref;
} t_system_file;

void free_system_file(void *system_file);
void free_token(void *token);
void free_dummy(void *token);
t_system_file *sysfile_new(const char *basedir, const char *name);

#endif //SYSTEM_FILE_H