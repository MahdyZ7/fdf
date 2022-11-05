/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 13:20:25 by ayassin           #+#    #+#             */
/*   Updated: 2022/05/07 20:16:23 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H
# include "minilibx_macos/mlx.h"
# include "minilibx-linux//mlx.h"
# include "libft_beta/libft.h"
# include "ft_printf_beta/ft_printf.h"
# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif

# define BASE_COLOR 0x1BFF80
# define ALPHA M_PI_4
# define BETA 0
# define GAMMA 0.61548
# define STABILITY 0.5

#ifdef linux
#  define W_KEY 119
#  define A_KEY 97
#  define S_KEY 115
#  define D_KEY 100
#  define LEFT_KEY 65361
#  define RIGHT_KEY 65363
#  define ESC_KEY 65307
#  define M_KEY 109
#  define H_KEY 104
#  define UP_KEY 65362
#  define DOWN_KEY 65364
#  define I_KEY 34
#endif

#ifdef __MACH__
#  define W_KEY 13
#  define A_KEY 0
#  define S_KEY 1
#  define D_KEY 2
#  define LEFT_KEY 123
#  define RIGHT_KEY 124
#  define UP_KEY 126
#  define DOWN_KEY 125
#  define ESC_KEY 53
#  define M_KEY 46
#  define H_KEY 4
#  define C_KEY 8
#  define I_KEY 34
#endif

# define TEXT "     ---  Key Controls  ----    \n\n\
	1) View\n\
		\t    [1] for top view\n\
		\t    [2] for side view\n\
		\t    [3] for face view\n\
		\t    [4] for isometric view\n\
		\t    [c] to center the object\n\
\n\
	2) Translation\n\
		\t    [<- ^ ->] Arrow keys for movment\n\
\n\
	3) Rotation\n\
		\t    [q] and [e] rotate about the z axis\n\
		\t    [w] and [s] rotate about the x axis\n\
		\t    [a] and [d] rotate about the y axis\n\
\n\
	4) Zooming\n\
		\t    [+] to zoom in\n\
		\t    [-] to zoom out\n\
\n\
	5) Wave action\n\
		\t    [.] to move step by step\n\
		\t    [,] to move continuously\n\
\n\
	6) Exit\n\
		\t    [esc] to exit\n"

// save important nums
typedef struct s_nums
{
	int		width;
	int		hight;
	int		div;
	int		x_trans;
	int		y_trans;
	float	alpha;
	float	beta;
	float	gamma;
	float	cos_a;
	float	sin_a;
	float	cos_b;
	float	sin_b;
	float	cos_g;
	float	sin_g;
}				t_nums;

// hold image related information
typedef struct s_img
{
	void	*img_ptr;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		hight;
	int		zoom;
	int		x_pos;
	int		y_pos;
	t_nums	*nums;
}				t_img;

// hold window info and key information
typedef struct s_scn
{
	void	*mlx_ptr;
	void	*win_ptr;
	t_img	*img;
	t_nums	*nums;
	t_list	*grid;
	float	**nxt;
	float	**now;
	int8_t	single_step;
}				t_scn;

// hold the x,y,z, and color of each point;
typedef struct s_pnt
{
	long long	x;
	long long	y;
	int			z;
	int			color;
}				t_pnt;

// fdf.c

void	pixel_put(t_img *data, int x, int y, int color);
void	setup_nums(t_nums *nums);

// get_next_line.c

char	*get_next_line(int fd);
char	*get_next_line_loop(char **buff, int readsize, int fd);

// readmap.c

t_list	*get_grid(char *file_name);

// key_hook.c

int		key_hook(int key, t_scn *scn);
void	redraw(t_scn *scn, t_img *img, int x_pos, int y_pos);

// draw_grid.c

void	draw_points(t_list *grid, t_img *img);
void	draw_line(t_list *line, t_img *img);

// clear.c

void	clear_grid(t_list *grid);
int		x_press(t_scn *scn);
void	clear_spc(float ***grid, int hight);

// wave.c

void	setup_spc(t_scn *scn);

// wave2.c

int		nxt_step(t_scn *scn);

#endif