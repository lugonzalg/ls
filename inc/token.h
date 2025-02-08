#ifndef TOKEN_H
#define TOKEN_H

typedef struct s_token {
    char            *token;
    struct s_token  *next;
} t_token;

#endif //TOKEN_H