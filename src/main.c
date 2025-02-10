#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "system_file.h"
#include "context.h"
#include "libft.h"
#include "ft_printf.h"

# define SPACE_SEPARATOR ' '

void free_system_file(void *system_file) {
    t_system_file *ref;

    ref = system_file;
    free(ref->name);
    ref->name = NULL;
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

int fill_context(t_context *ctx, t_list *head) {
    t_list          *tmp;
    char            *token;
    t_system_file   *system_file;

    while (head) {

        token = head->content;

        // Is a flag case
        if (*token == '-' && *(token + 1)) {
            while (*(++token)) {
                if (ft_strncmp(FLAGS, token, 1)){
                    ft_printf("ls: invalid option -- '%c'\n", *token);
                    ft_printf("Try 'ls --help' for more information.\n");
                    return 2;
                }
                ctx->flags[(int)*token] = true;
            }
        }
        // Is a file/directory case
        else {
            system_file = ft_calloc(1, sizeof(t_system_file));
            if (system_file == NULL)
                return 2;

            stat(token, &system_file->stats);
            system_file->name = ft_strdup(token);
            tmp = ft_lstnew(system_file);
            ft_lstadd_back(&ctx->head, tmp);
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

    retval = fill_context(ctx, head);
    if (retval)
        return retval;
    ft_lstclear(&head, free_token);

    ft_lstclear(&ctx->head, free_system_file);
    return retval;
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