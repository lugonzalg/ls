#ifndef USER_INPUT_H
# define USER_INPUT_H

#include "libft.h"
#include "context.h"

t_list  *create_token_list(int argc, char *argv[]);
int extract_data(t_context *ctx, t_list *head);
int process_user_input(int argc, char *argv[], t_context *ctx);
int assign_file(t_context *ctx, const char *token);
#endif //USER_INPUT_H