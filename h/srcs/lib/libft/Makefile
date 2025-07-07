# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/21 14:32:14 by hjuma             #+#    #+#              #
#    Updated: 2024/10/21 14:35:51 by hjuma            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 	libft.a

SRC = ft_atoi.c \
	ft_bzero.c \
	ft_calloc.c \
	ft_strlen.c \
	ft_strlcpy.c \
	ft_strlcat.c \
	ft_strchr.c \
	ft_strrchr.c \
	ft_strnstr.c \
	ft_strncmp.c \
	ft_isalnum.c \
	ft_isalpha.c \
	ft_isascii.c \
	ft_isdigit.c \
	ft_isprint.c \
	ft_memchr.c \
	ft_memcmp.c \
	ft_memmove.c \
	ft_memcpy.c \
	ft_memset.c \
	ft_toupper.c \
	ft_tolower.c \
	ft_strdup.c \
	ft_substr.c \
	ft_strjoin.c \
	ft_strtrim.c \
	ft_strmapi.c \
	ft_split.c \
	ft_itoa.c \
	ft_strcmp.c \
	ft_strndup.c \
	ft_putchar_fd.c \
	ft_putstr_fd.c \
	ft_putendl_fd.c \
	ft_striteri.c \
	ft_putnbr_fd.c 

BONUS = ft_lstadd_back.c \
		ft_lstadd_front.c \
		ft_lstclear.c \
		ft_lstdelone.c \
		ft_lstiter.c \
		ft_lstlast.c \
		ft_lstmap.c \
		ft_lstnew.c \
		ft_lstsize.c 

OBJS = ${SRC:.c=.o}
BONUS_OBJS = ${BONUS:.c=.o}

LIBC = ar rcs
LIBR = ranlib
CC = gcc
RM = rm -rf
CFLAGS = -Wall -Wextra -Werror
INCLUDE	= -I.	
	
%.o: %.c
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

${NAME}: ${OBJS}
	${LIBC} ${NAME} ${OBJS}
	${LIBR} ${NAME} 

all:	${NAME}

bonus: ${OBJS} ${BONUS_OBJS}
	${LIBC} ${NAME} ${OBJS} ${BONUS_OBJS}
	${LIBR} ${NAME}

clean:
	${RM} ${OBJS} ${BONUS_OBJS}

fclean: clean
		${RM} ${NAME}

re: fclean all

.PHONY: all bonus clean fclean re
