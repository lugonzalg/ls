#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "context.h"
#include "libft.h"

# define SPACE_SEPARATOR ' '

void free_token(void *token) {
    free(token);
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
    t_list      *tmp;
    char        *token;
    struct stat *file_stats;

    while (head) {

        token = head->content;

        // Is a flag case
        if (*token == '-') {
            for (size_t idx = 1; idx < ft_strlen(token); idx++) {
                if (ft_strncmp(FLAGS, &token[idx], 1))
                    return 1;
                ctx->flags[(int)token[idx]] = true;
            }
        }
        // Is a file/directory case
        else {
            file_stats = ft_calloc(1, sizeof(struct stat));
            if (file_stats == NULL)
                return 1;

            if (stat(token, file_stats)) {
                free(file_stats);
                return 1;
            }
            tmp = ft_lstnew(file_stats);
        }
        tmp = head;
        head = head->next;
        ft_lstdelone(tmp, free_token);
    }
    return 0;
}

int process_user_input(int argc, char *argv[], t_context *ctx) {
    t_list  *head;

    head = create_token_list(argc, argv);
    if (head == NULL)
        return 1;

    if (fill_context(ctx, head))
        return 1;

    ft_lstclear(&head, free_token);
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