#ifndef LEXER_H
# define LEXER_H
// # include "libft.h"
# include <stdio.h>
# include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
# include <stddef.h>

// readline~Lexer

typedef enum e_token_kind
{
    TK_WORD,
    TK_OP,
    TK_EOF
}               t_token_kind;

typedef struct s_token
{
    char            *word;
    t_token_kind    kind;
	struct s_token     	*next;
}					t_token;

// 引用符処理
#define SINGLE_QUOTE_CHAR '\''
#define DOUBLE_QUOTE_CHAR '"'

//main
int main(int argc, char **argv, char **envp);

//readline
int	ft_readline(char **envp);
void interpret(char *line, int *stat_loc);

//Lexer
t_token	*tokenize(char *line);
char	**token_list_to_argv(t_token *tok);
t_token *new_token(char *str ,t_token_kind kind);
char	**tail_recursive(t_token *tok, int nargs, char **argv);
bool is_blank(char c);
bool consume_blank(char **rest, char *line);
bool	startswith(const char *s, const char *keyword);
bool is_operator(const char *line);
bool is_metacharacter(char c);
t_token *operator(char **rest, char *line);
t_token *word(char **rest, char *line);
void tokenize_error(const char *location, char **rest, char *line);



//exec以下(mock)
char *search_path(const char *filename);
void	validate_access(const char *path, const char *filename);
int exec(char *argv[]);


#endif