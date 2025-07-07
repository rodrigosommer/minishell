# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: terabu <terabu@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/24 14:24:24 by subarunrun        #+#    #+#              #
#    Updated: 2023/05/11 09:47:51 by terabu           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
RL_FLAGS = -lreadline -lhistory -L$(shell brew --prefix readline)/lib
INCLUDE = -I include -I $(LIBFT_DIR) -I $(shell brew --prefix readline)/include -I/usr/local/opt/readline/include
CFLAGS = -Wall -Werror -Wextra $(INCLUDE)

SOURCES_DIR = ./srcs
UTILS_DIR = ./srcs/utils
ERROR_DIR = ./srcs/error
TOKEN_DIR = ./srcs/tokenizer
PARSER_DIR = ./srcs/parser
EXPAND_DIR = ./srcs/expand
REDIR_DIR = ./srcs/redirect
HEREDOC_DIR = ./srcs/heredoc
PIPE_DIR = ./srcs/pipe
EXEC_DIR = ./srcs/exec
SIGNAL_DIR = ./srcs/signal
BUILTIN_DIR = ./srcs/builtin
WRAPPER_DIR = ./srcs/wrapper
LIBFT_DIR = ./srcs/lib/libft

SOURCES = $(SOURCES_DIR)/0_main.c\
		  $(SOURCES_DIR)/1_init_environ_list.c\
		  $(SOURCES_DIR)/2_builtin_search.c\
		  $(SOURCES_DIR)/3_destructors.c\
		  $(ERROR_DIR)/error_exec.c\
		  $(ERROR_DIR)/error_general.c\
		  $(ERROR_DIR)/error_put.c\
		  $(ERROR_DIR)/error_syntax.c\
		  $(SIGNAL_DIR)/signal_handle.c\
		  $(SIGNAL_DIR)/signal.c\
		  $(BUILTIN_DIR)/cd.c\
		  $(BUILTIN_DIR)/echo.c\
		  $(BUILTIN_DIR)/env.c\
		  $(BUILTIN_DIR)/exit.c\
		  $(BUILTIN_DIR)/exit_utils.c\
		  $(BUILTIN_DIR)/export.c\
		  $(BUILTIN_DIR)/export_utils.c\
		  $(BUILTIN_DIR)/pwd.c\
		  $(BUILTIN_DIR)/unset_utils.c\
		  $(BUILTIN_DIR)/unset.c\
		  $(TOKEN_DIR)/tokenizer.c\
		  $(TOKEN_DIR)/tokenizer_check_literal.c\
		  $(TOKEN_DIR)/tokenizer_check_quote.c\
		  $(TOKEN_DIR)/tokenizer_utils.c\
		  $(PARSER_DIR)/parser.c\
		  $(PARSER_DIR)/parser_node_redirect.c\
		  $(PARSER_DIR)/parser_make_tok.c\
		  $(PARSER_DIR)/parser_check.c\
		  $(EXPAND_DIR)/expand_dollar_sign.c\
		  $(EXPAND_DIR)/expand_word.c\
		  $(EXPAND_DIR)/expand.c\
		  $(HEREDOC_DIR)/heredoc_delete.c\
		  $(HEREDOC_DIR)/heredoc.c\
		  $(REDIR_DIR)/redirect.c\
		  $(REDIR_DIR)/redirect_fd.c\
		  $(EXEC_DIR)/exec_get_cmd.c\
		  $(EXEC_DIR)/exec.c\
		  $(PIPE_DIR)/pipe.c\
		  $(PIPE_DIR)/pipex_utils.c\
		  $(WRAPPER_DIR)/wrap_systemcall_1.c\
		  $(WRAPPER_DIR)/wrap_systemcall_2.c\

VPATH = $(SOURCES_DIR) $(UTILS_DIR) $(BUILTIN_DIR) $(ERROR_DIR) \
 		$(PARSER_DIR) $(REDIR_DIR) $(TOKEN_DIR) $(SIGNAL_DIR) $(PIPE_DIR) \
		$(EXEC_DIR) $(WRAPPER_DIR) $(EXPAND_DIR) $(HEREDOC_DIR)
OBJS_DIR = objs
OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(SOURCES:.c=.o)))
LIBFT = -L$(LIBFT_DIR) -lft

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $^ $(RL_FLAGS) $(LIBFT) -o $(NAME)

$(OBJS_DIR)/%.o: %.c
	mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)
	$(MAKE) -C ${LIBFT_DIR} clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C ${LIBFT_DIR} fclean

re: fclean all
