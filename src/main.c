#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

#include "system_file.h"
#include "context.h"
#include "libft.h"
#include "ft_printf.h"
#include "sort_aux.h"
#include "user_input.h"
#include "defs.h"

static void *resize(void *ptr, size_t curr_size, size_t new_size) {
    void *new_ptr;

    new_ptr = ft_calloc(1, new_size);
    if (new_ptr == NULL)
        return NULL;

    ft_memmove(new_ptr, ptr, curr_size);
    free(ptr);
    return new_ptr;
}

static void print_file_info(
    t_system_file **file_array,
    t_context *ctx,
    size_t n_of_files,
    const char *basedir
) {
    size_t  idx;

    idx = 0;
    if (ctx->flags[RECURSIVE])
        ft_printf("%s:\n", basedir);

    //if (ctx->total_file_len <= GRID_LEN) {
    while (idx < n_of_files)
        ft_printf("%s  ", file_array[idx++]->name);
    ft_printf("\n");
}

static bool is_not_dot(const char *filename) {
    return ft_strncmp(".", filename, 2) && ft_strncmp("..", filename, 3);
}

static int recursive(t_system_file **file_array, size_t n_of_files, t_context *ctx, const char *ctx_dirname) {

    char    *base;
    char    *next_dir;

    base = ft_strjoin(ctx_dirname, "/");
    if (base == NULL)
        return 2;

    for (size_t j = 0; j < n_of_files; j++) {
        if (file_array[j]->stats.st_mode == 16893 && is_not_dot(file_array[j]->name)) {
            next_dir = ft_strjoin(base, file_array[j]->name);
            if (next_dir == NULL)
                return 2;
            process_file_info(next_dir, ctx);
            free(next_dir);
        }
    }
    free(base);
    return 0;
}

int process_file_info(const char *basedir, t_context *ctx) {
    t_system_file **file_array;
    DIR           *curr_dir;
    struct dirent *curr_file;
    size_t        n_of_dirs;
    size_t        idx;

    n_of_dirs = N_OF_DIRS;
    idx = 0;
    curr_dir = opendir(basedir);
    if (curr_dir == NULL) {
        return 2;
    }

    file_array = (t_system_file **)malloc(n_of_dirs * sizeof(t_system_file *));
    curr_file = readdir(curr_dir); 
    while (curr_file) {

        if (!ctx->flags[ALL] && *curr_file->d_name == '.') {
            curr_file = readdir(curr_dir);
            continue ;
        }

        file_array[idx] = sysfile_new(basedir, curr_file->d_name);
        if (file_array[idx] == NULL)
            return 2;

        ctx->total_file_len += ft_strlen(curr_file->d_name);
        idx++;

        if (idx == n_of_dirs) {
            file_array = resize(file_array, 
                    n_of_dirs * sizeof(t_system_file *), 
                    (n_of_dirs * 2) * sizeof(t_system_file *));
            if (file_array == NULL)
                return 2;

            n_of_dirs *= 2;
        }
        curr_file = readdir(curr_dir); 
    }
    ctx->total_file_len += (idx - 1) * 2;
    closedir(curr_dir);
    ft_merge_sort(file_array, 0, idx - 1, sizeof(t_system_file *), ctx->comparison_method);
    print_file_info(file_array, ctx, idx, basedir);

    if (ctx->flags[RECURSIVE]) {
        recursive(file_array, idx, ctx, basedir);
    }

    for (size_t i = 0; i < idx; i++) {
        free(file_array[i]->name);
        free(file_array[i]);
    }
    free(file_array);
    return 0;
}

static int run_program(t_context *ctx) {
    t_list          *tmp;
    t_system_file   *sysfile;

    if (ctx->flags[SORT_BY_TIME]) {
        if (ctx->flags[REVERSE])
            ctx->comparison_method = reverse_timestamp_compare;
        else
            ctx->comparison_method = timestamp_compare;
    }
    else if (ctx->flags[REVERSE])
        ctx->comparison_method = reverse_alphanumeric_compare;

    tmp = ctx->head;
    while (tmp) {
        sysfile = tmp->content;
        process_file_info(sysfile->name, ctx);
        tmp = tmp->next;
    }
    return 0;
}

int main(int argc, char *argv[]) {

    t_context ctx;
    int       retval;

    retval = 0;
    ft_bzero(&ctx, sizeof(t_context));
    ctx.comparison_method = alphanumeric_compare;

    if (argc != 1)
        retval = process_user_input(argc, argv, &ctx);
        
    if (ctx.head == NULL)
        assign_file(&ctx, ".");

    run_program(&ctx);
    ft_lstclear(&ctx.head, free_system_file);
    return retval;
}