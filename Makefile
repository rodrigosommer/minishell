# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rsommer <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/05 16:20:27 by rsommer           #+#    #+#              #
#    Updated: 2025/06/05 16:20:42 by rsommer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

# Directories
SRC_DIR = srcs
OBJ_DIR = objs
LIBFT_DIR = libft
FTPRINTF_DIR = ft_printf

# Source files
SRCS = main.c \
       parsing/lexer.c \
       parsing/parser.c \
       parsing/expander.c \
       execution/executor.c \
       execution/pipes.c \
       execution/redirections.c \
       execution/builtins/builtin_exit.c \
       execution/builtins/builtin_cd.c \
       execution/builtins/builtin_echo.c \
       execution/builtins/builtin_env.c \
       execution/builtins/builtin_export.c \
       execution/builtins/builtin_pwd.c \
       execution/builtins/builtin_unset.c \
       utils/error_handling.c \
       utils/env_utils.c \
       utils/free_utils.c \
       utils/readline_utils.c \
       utils/signals.c \
       utils/global.c

# Object files
OBJS = $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))

# Compiler settings
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -Iincludes -I$(LIBFT_DIR)/includes -I$(FTPRINTF_DIR)/includes
LIBFT = $(LIBFT_DIR)/libft.a
FTPRINTF = $(FTPRINTF_DIR)/libftprintf.a
READLINE = -lreadline -L/usr/local/opt/readline/lib

# Colors
GREEN = \033[0;32m
RED = \033[0;31m
NC = \033[0m

# Rules
all: $(NAME)

$(NAME): $(LIBFT) $(FTPRINTF) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(FTPRINTF) $(READLINE) -o $(NAME)
	@echo "$(GREEN)✓ Minishell compiled successfully$(NC)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)
	@echo "$(GREEN)✓ Libft compiled successfully$(NC)"

$(FTPRINTF):
	@make -C $(FTPRINTF_DIR)
	@echo "$(GREEN)✓ Ft_printf compiled successfully$(NC)"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)✗ Object files removed$(NC)"
	@make -C $(LIBFT_DIR) clean
	@make -C $(FTPRINTF_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)✗ $(NAME) removed$(NC)"
	@make -C $(LIBFT_DIR) fclean
	@make -C $(FTPRINTF_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
