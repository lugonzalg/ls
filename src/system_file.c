#include <stddef.h>
#include <stdlib.h>

#include "system_file.h"

void free_system_file(void *system_file) {
    t_system_file *ref;

    ref = system_file;
    free(ref->name);
    free(ref);
}

void free_token(void *token) {
    free(token);
    token = NULL;
}

void free_dummy(void *token) {
    (void)token;
}

t_system_file *sysfile_new(const char *basedir, const char *name) {
    t_system_file   *sysfile;
    char            *base_path;
    char            *relative_path;

    sysfile = ft_calloc(1, sizeof(t_system_file));
    if (sysfile == NULL)
        return NULL;

    base_path = ft_strjoin(basedir, "/");
    if (base_path == NULL) {
        free(sysfile);
        return NULL;
    }

    relative_path = ft_strjoin(base_path, name);
    if (relative_path == NULL) {
        free(sysfile);
        free(base_path);
        return NULL;
    }

    free(base_path);
    lstat(relative_path, &sysfile->stats);
    free(relative_path);

    sysfile->time_ref = sysfile->stats.st_mtim.tv_sec * 1000000000LL + sysfile->stats.st_mtim.tv_nsec;
    sysfile->name = ft_strdup(name);
    if (sysfile->name == NULL) {
        free(sysfile);
        return NULL;
    }
    return sysfile;
}