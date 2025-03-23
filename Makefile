# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrouves <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/08 18:27:35 by mrouves           #+#    #+#              #
#    Updated: 2025/03/23 22:58:36 by mrouves          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include sources/pretty_compile.mk

SOURCES		:=	main.c\
				philo.c\
				table.c\

NAME 			:= philo

DIR_HEADERS		:= headers
DIR_SOURCES		:= sources
DIR_OBJS		:= .objs

OBJS			:= $(addprefix $(DIR_OBJS)/, $(SOURCES:%.c=%.o))

CC				:= clang
CFLAGS			:= -Wall -Wextra -Werror -g
IFLAGS			:= -I $(DIR_HEADERS)
DIR_DUP			= mkdir -p $(@D)

all: $(NAME) $(OBJS)

$(NAME): $(OBJS)
	@$(call run_and_test, $(CC) $(CFLAGS) $(IFLAGS) $^ -o $@ -pthread)
	@printf "$(BOLD)$@$(NO_COLOR) compiled $(OK_COLOR)successfully$(NO_COLOR)\n"

$(DIR_OBJS)/%.o: $(DIR_SOURCES)/%.c
	@$(DIR_DUP)
	@$(call run_and_test,$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@)

clean:
	@rm -rf $(DIR_OBJS)
	@printf "Cleaned $(BOLD)$(DIR_OBJS)$(NO_COLOR)\n"

fclean: clean
	@rm -f $(NAME)
	@printf "Cleaned $(BOLD)$(NAME)$(NO_COLOR)\n"

re: fclean all

.PHONY: clean fclean re all
