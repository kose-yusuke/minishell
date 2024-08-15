/* error.h - エラー処理に関する関数の定義。 */
#ifndef ERROR_H
# define ERROR_H

// TODO: おそらくdebug用の関数
void	assert_error(const char *msg, char *location);
void	error_exit(const char *msg, int exit_status);
void	parser_error(const char *unexpected_token);
void	report_error(char *cmd, char *file, char *info);

#endif
