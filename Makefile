# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rsommer <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#              #
#    Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I./includes -I./libft -I./ft_printf
LIBS = -L./libft -L./ft_printf -lft -lftprintf -lreadline

SRCDIR = srcs
OBJDIR = obj

SOURCES = srcs/main.c \
          srcs/shell_loop.c \
          srcs/tokenization/tokenize.c \
          srcs/tokenization/tokenize_utils.c \
          srcs/tokenization/token_handlers.c \
          srcs/parsing/parse.c \
          srcs/parsing/parse_utils.c \
          srcs/parsing/parse_helpers.c \
          srcs/parsing/wildcard_parse.c \
          srcs/parsing/parentheses.c \
          srcs/utils/env.c \
          srcs/utils/env_helpers.c \
          srcs/utils/expand.c \
          srcs/utils/expand_utils.c \
          srcs/execution/execute.c \
          srcs/execution/execute_utils.c \
          srcs/execution/pipes.c \
          srcs/execution/pipes_helpers.c \
          srcs/execution/pipes_utils.c \
          srcs/utils/redirections.c \
          srcs/utils/redirect_utils.c \
          srcs/builtins/builtins_1.c \
          srcs/builtins/builtins_2.c \
          srcs/builtins/builtins_utils.c \
          srcs/utils/signals.c \
          srcs/utils/quote_utils.c \
          srcs/utils/quote_helpers.c \
          srcs/utils/wildcard.c \
          srcs/utils/wildcard_utils.c

OBJECTS = $(SOURCES:srcs/%.c=$(OBJDIR)/%.o)

LIBFT = ./libft/libft.a
FT_PRINTF = ./ft_printf/libftprintf.a

all: $(NAME)

$(NAME): $(LIBFT) $(FT_PRINTF) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $(NAME)

$(OBJDIR)/%.o: srcs/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	make -C ./libft

$(FT_PRINTF):
	make -C ./ft_printf

clean:
	rm -rf $(OBJDIR)
	make -C ./libft clean
	make -C ./ft_printf clean

fclean: clean
	rm -f $(NAME)
	make -C ./libft fclean
	make -C ./ft_printf fclean

re: fclean all

.PHONY: all clean fclean re