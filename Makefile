# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/01/14 15:29:40 by tle-meur          #+#    #+#              #
#    Updated: 2016/03/30 15:33:47 by tle-meur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ft_ls

FLAGS	=	-Wall -Wextra -Werror -O3

SRCS	=	main.c utilities_main.c sort.c compute.c utilities.c \
			utilities_print.c print.c

OBJS	=	$(SRCS:.c=.o)

all		:	$(NAME)

%.o		:	%.c
			@gcc $(FLAGS) -o $@ -c $< -I libft/

$(NAME)	:	$(OBJS)
			@make -C libft/
			@gcc $(FLAGS) -o $@ $^ -L libft/ -lft
			@echo "$(NAME) created"
clean	:
			@rm -f $(OBJS)
			@make -C libft/ clean
			@echo "objects deleted"

fclean	:	clean
			@make -C libft/ fclean
			@rm -f $(NAME)
			@echo "$(NAME) deleted"

re		:	fclean $(NAME)

simple	:	re
			@rm -f $(OBJS)
			@make -C libft/ clean
			@echo "objects deleted"

.PHONY	:	all clean fclean re simple
