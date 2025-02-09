#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "system_file.h"
#include "context.h"
#include "libft.h"

# define SPACE_SEPARATOR ' '

void free_system_file(void *system_file) {
    free(((t_system_file *)system_file)->name);
    free(((t_system_file *)system_file));
}

void free_token(void *token) {
    free(token);
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

int fill_context(t_context *ctx, t_list *head) {
    t_list          *tmp;
    char            *token;
    t_system_file   *system_file;

    while (head) {

        token = head->content;

        // Is a flag case
        if (*token == '-') {
            for (size_t idx = 1; idx < ft_strlen(token); idx++) {
                if (ft_strncmp(FLAGS, &token[idx], 1))
                    return 1;
                ctx->flags[(int)token[idx]] = true;
                free(token);
            }
        }
        // Is a file/directory case
        else {
            system_file = ft_calloc(1, sizeof(t_system_file));
            if (system_file == NULL)
                return 1;

            if (stat(token, &system_file->stats)) {
                free(system_file);
                return 1;
            }

            system_file->name = token;
            tmp = ft_lstnew(system_file);
            ft_lstadd_back(&ctx->head, tmp);
        }
        tmp = head;
        head = head->next;
        ft_lstdelone(tmp, free_dummy);
    }
    return 0;
}

int process_user_input(int argc, char *argv[], t_context *ctx) {
    t_list  *head;

    head = create_token_list(argc, argv);
    if (head == NULL)
        return 1;

    if (fill_context(ctx, head)) {
        ft_lstdelone(head, free_token);
        return 1;
    }

    ft_lstclear(&ctx->head, free_system_file);
    return 0;
}

int main(int argc, char *argv[]) {

    t_context ctx;
    int       retval;

    retval = 1;
    ft_bzero(&ctx, sizeof(t_context));
    if (argc != 1)
        retval = process_user_input(argc, argv, &ctx);

    if (retval)
        return retval;
    return 0;
}