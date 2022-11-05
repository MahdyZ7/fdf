# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/05 13:07:14 by ayassin           #+#    #+#              #
#    Updated: 2022/05/07 20:18:49 by ayassin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fdf

HEADER = fdf.h

CFLAGS = -Werror -Wall -Wextra

SRC = fdf.c readmap.c get_next_line.c draw_grid.c clear.c \
	wave.c wave2.c 

ifeq ($(shell uname -s), Darwin)
	CFLAGS += -Imlx
	SRC += key_hook.c
	LINK_MLX = -Lminilibx_macos -lmlx -framework OpenGl -framework APPKit
	MLX_LIB = minilibx_macos
else ifeq ($(shell uname -s), Linux)
	CFLAGS += -Imlx
	SRC += key_hook_linux.c
	LINK_MLX = -Lminilibx-linux -lmlx -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz
	MLX_LIB = minilibx-linux
endif

SUBDIRS = libft_beta ft_printf_beta $(MLX_LIB)

CC = gcc


OBJS = $(SRC:.c=.o)

all: $(NAME)

.c.o:
	$(CC) $(CFLAGS) -Ilibft_beta -Ift_printf_beta -c $^ -o $@

$(NAME): $(SUBDIRS) $(OBJS)
	for dir in $(SUBDIRS); do \
        $(MAKE) all -C $$dir; \
    done
	$(CC) $(CFALGS) $(SRC) $(LINK_MLX) \
		-Lft_printf_beta -lftprintf -Llibft_beta -lft -o $(NAME)

$(SUBDIRS):
	for dir in $(SUBDIRS); do \
        $(MAKE) all -C $$dir; \
    done

clean:
	rm -f $(OBJS)
	for dir in $(SUBDIRS); do \
        $(MAKE) clean -C $$dir; \
    done

fclean: clean
	rm -f $(NAME)
	for dir in $(SUBDIRS); do \
        $(MAKE) fclean -C $$dir; \
    done

re : fclean all

.PHONY: clean fclean all re