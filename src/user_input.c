#include <stdlib.h>

#include "libft.h"
#include "ft_printf.h"
#include "system_file.h"
#include "defs.h"

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