/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 11:51:03 by ayassin           #+#    #+#             */
/*   Updated: 2022/05/07 20:18:35 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/* This file is responsible for all mlx key hook actions (except of esc)

The key is use are
	1) Translation:
		a) arrow key for movment
		b) 'c' to center the image on the window

	2) Rotation
		a) 'q' and 'e' rotate about the z axis
		b) 'w' and 's' rotate about the x axis
		c) 'a' and 'd' rotate about the y axis
		d) '1' to get a top view
		e) '2' to get a side view
		f) '3' to get a face view
		d) '4' to get a isometric view

	3) Zooming
		a) '+' and '-' to zoom in and out
		
	4) Wave action
		a) '.' to move step by step
		b) ',' to move continuously through time
	
	5) exit
		a) 'esc' to exit
*/

static void	trans(int key, t_scn *scn);
static void	rotate(int key, t_scn *scn);
static void	zoom(int key, t_scn *scn);
int			key_hook(int key, t_scn *scn);
void		redraw(t_scn *scn, t_img *img, int x_pos, int y_pos);

/* translate the points on the image suing arrow keys. 
the 'c' key centers the image to the window*/
static void	trans(int key, t_scn *scn)
{
	if (key == 65362)
		scn->nums->y_trans -= 20;
	else if (key == 65364)
		scn->nums->y_trans += 20;
	else if (key == 65361)
		scn->nums->x_trans -= 20;
	else if (key == 65363)
		scn->nums->x_trans += 20;
	else if (key == 99)
	{
		scn->nums->x_trans = (scn->img->width) / 2;
		scn->nums->y_trans = (scn->img->hight) / 2;
	}
	redraw(scn, scn->img, scn->img->x_pos, scn->img->y_pos);
}

/* rotates the map by updating the rotation angles

1) 'q' and 'e' rotate about the z axis
2) 'w' and 's' rotate about the x axis
3) 'a' and 'd' rotate about the y axis
4) '1' to get a top view
5) '2' to get a side view
6) '3' to get a face view
7) '4' to get a isometric view*/
static void	rotate(int key, t_scn *scn)
{
	if (key == 101)
		scn->nums->alpha -= M_PI_2 / 9.0;
	else if (key == 113)
		scn->nums->alpha += M_PI_2 / 9.0;
	else if (key == 97)
		scn->nums->beta -= (M_PI_2 / 9.0);
	else if (key == 100)
		scn->nums->beta += M_PI_2 / 9.0;
	else if (key == 115)
		scn->nums->gamma -= M_PI_2 / 9.0;
	else if (key == 119)
		scn->nums->gamma += M_PI_2 / 9.0;
	else if (key == 49 || key == 52 || key == 65436 || key == 65430)
	{
		scn->nums->beta = BETA * (key == 52 || key == 65430);
		scn->nums->alpha = ALPHA * (key == 52 || key == 65430);
		scn->nums->gamma = GAMMA * (key == 52 || key == 65430);
	}
	else if (key == 50 || key == 51 || key == 65433 || key == 65435)
	{
		scn->nums->beta = M_PI_2 * (key == 50 || key == 65433);
		scn->nums->alpha = 0;
		scn->nums->gamma = M_PI_2;
	}
	redraw(scn, scn->img, scn->img->x_pos, scn->img->y_pos);
}

/*zooms on the map using the '+' and '-' keys. the divistions 'div' 
between points is increased/decreased 10% +- 1 ( the one is absolute)*/
static void	zoom(int key, t_scn *scn)
{
	float	x_unknown;
	float	y_unknown;

	x_unknown = (scn->img->width / 2.0 - scn->nums->x_trans) / scn->nums->div;
	y_unknown = (scn->img->hight / 2.0 - scn->nums->y_trans) / scn->nums->div;
	if (key == 65451 || key == 61)
		scn->nums->div = scn->nums->div * 1.1 + 1;
	else
		scn->nums->div = scn->nums->div * 0.9 - 1;
	if (scn->nums->div < 1)
		scn->nums->div = 1;
	scn->nums->x_trans = scn->img->width / 2.0 - x_unknown * scn->nums->div;
	scn->nums->y_trans = scn->img->hight / 2.0 - y_unknown * scn->nums->div;
	redraw(scn, scn->img, scn->img->x_pos, scn->img->y_pos);
}

/*switch function for keys*/
int	key_hook(int key, t_scn *scn)
{
	if ((key >= 65361 && key <= 65364) || key == 99)
		trans(key, scn);
	else if ((key >= 97 && key <= 119) || (key >= 49 && key <= 52)
		|| (key >= 65430 && key <= 65436))
		rotate(key, scn);
	else if (key == 65453 || key == 65451 || key == 61 || key == 45)
		zoom(key, scn);
	else if (key == 65307)
		x_press(scn);
	else if (key == 65439 || key == 46)
	{
		scn->single_step = 0;
		nxt_step(scn);
		scn->single_step = 1;
	}	
	else if (key == 44)
		scn->single_step = 0;
	return (0);
}

/* Destroy the imgae and then palce a new ceated image on the window.
Used to update the image on the window for any change that occures*/
void	redraw(t_scn *s, t_img *img, int x_pos, int y_pos)
{
	int		i;
	char	*temp[2];

	mlx_destroy_image(s->mlx_ptr, s->img->img_ptr);
	img->img_ptr = mlx_new_image(s->mlx_ptr, img->width, img->hight);
	s->img->addr = mlx_get_data_addr(img->img_ptr, &(img->bits_per_pixel),
			&(img->line_length), &(img->endian));
	if (!img->img_ptr || !img->addr)
		x_press(s);
	draw_points(s->grid, s->img);
	draw_line(s->grid, s->img);
	mlx_clear_window(s->mlx_ptr, s->win_ptr);
	mlx_put_image_to_window(s->mlx_ptr, s->win_ptr, img->img_ptr, x_pos, y_pos);
	temp[0] = TEXT;
	i = 20;
	while (ft_strrchr(temp[0], '\n'))
	{
		temp[1] = ft_substr(temp[0], 0, ft_strchr(temp[0], '\n') - temp[0]);
		if (temp[1])
			mlx_string_put(s->mlx_ptr, s->win_ptr, 21, i, BASE_COLOR, temp[1]);
		if (temp[1])
			free(temp[1]);
		temp[0] = ft_strchr(temp[0], '\n') + 1;
		i = i + 20;
	}
}
