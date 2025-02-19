#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

#include "system_file.h"
#include "context.h"
#include "libft.h"
#include "ft_printf.h"

# define SPACE_SEPARATOR ' '
# define GRID_LEN 146
# define N_OF_DIRS 100

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

t_list  *create_token_list(int argc, char *argv[]){
    char    **token_list;
    t_list  *head;
    t_list  *tmp;

    head = NULL;
    for (int idx = 1; idx < argc; idx++) {
        token_list = ft_split(argv[idx], SPACE_SEPARATOR);
        if (token_list == NULL) {
            ft_lstclear(&head, free_token);
            return NULL;
        }

        for (size_t j = 0; token_list[j]; j++) {
            tmp = ft_lstnew(token_list[j]);
            if (tmp == NULL) {
                ft_lstclear(&head, free_token);
                return NULL;
            }
            ft_lstadd_back(&head, tmp);
        }
        free(token_list);
    }
    return head;
}

static int assign_flag(t_context *ctx, const char *token) {
    if (*(token + 1) == '-') {
        ft_printf("ls: invalid option - '%c'\n", *(token + 1));
        return 2;
    }

    while (*(++token)) {
        if (!ft_strchr(FLAGS, *token)){
            ft_printf("ls: invalid option -- '%c'\n", *token);
            ft_printf("Try 'ls --help' for more information.\n");
            return 2;
        }
        ctx->flags[(int)*token] = true;
    }
    return 0;
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
    stat(relative_path, &sysfile->stats);
    free(relative_path);

    sysfile->time_ref = sysfile->stats.st_mtim.tv_sec * 1000000000LL + sysfile->stats.st_mtim.tv_nsec;
    sysfile->name = ft_strdup(name);
    if (sysfile->name == NULL) {
        free(sysfile);
        return NULL;
    }
    return sysfile;
}

int assign_file(t_context *ctx, const char *token) {
    t_list        *tmp;
    t_system_file *sysfile;

    sysfile = sysfile_new(".", token);
    if (sysfile == NULL)
        return 2;

    tmp = ft_lstnew(sysfile);
    ft_lstadd_back(&ctx->head, tmp);
    return 0;
}

int extract_data(t_context *ctx, t_list *head) {
    char            *token;

    while (head) {
        token = head->content;

        // Is a flag case
        if (*token == '-' && *(token + 1)) {
            if (assign_flag(ctx, token))
                return 2;
        }
        // Is a file/directory case
        else {
            if (assign_file(ctx, token))
                return 2;
        }
        head = head->next;
    }

    return 0;
}

int process_user_input(int argc, char *argv[], t_context *ctx) {
    t_list  *head;
    int     retval;

    retval = 0;
    head = create_token_list(argc, argv);
    if (head == NULL)
        return 1;

    retval = extract_data(ctx, head);
    if (retval) {
        ft_lstclear(&head, free_token);
        return retval;
    }
    ft_lstclear(&head, free_token);
    return retval;
}

static void *resize(void *ptr, size_t curr_size, size_t new_size) {
    void *new_ptr;

    new_ptr = ft_calloc(1, new_size);
    if (new_ptr == NULL)
        return NULL;

    ft_memmove(new_ptr, ptr, curr_size);
    free(ptr);
    return new_ptr;
}

bool alphanumeric_compare(void *v1, int low, int high) {

    t_system_file   **sysfile;
    int             diff;
    int             len1;
    int             len2;
    int             max_len;

    sysfile = v1;
    len1 = ft_strlen(sysfile[low]->name);
    len2 = ft_strlen(sysfile[high]->name);
    max_len = len1;
    if (max_len < len2)
        max_len = len2;

    diff = ft_strncmp(sysfile[low]->name, sysfile[high]->name, max_len);

    if (diff <= 0)
        return true;
    return false;

}

bool timestamp_compare(void *v1, int low, int high) {
    t_system_file **sysfile = v1;

    if (sysfile[low]->time_ref < sysfile[high]->time_ref)
        return false;
    
    if (sysfile[low]->time_ref > sysfile[high]->time_ref)
        return true;

    return alphanumeric_compare(v1, low, high);
}

bool reverse_timestamp_compare(void *v1, int low, int high) {
    return !timestamp_compare(v1, low, high);
}

bool reverse_alphanumeric_compare(void *v1, int low, int high) {
    return !alphanumeric_compare(v1, low, high);
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