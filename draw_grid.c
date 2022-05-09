/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_grid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 14:06:31 by ayassin           #+#    #+#             */
/*   Updated: 2022/05/09 17:00:10 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	color_grade(t_pnt *pt0, t_pnt *ptm, t_pnt *pt1);
static void	dl_helper(t_pnt *point0, t_pnt pointm, t_pnt *point1, t_img *img);
void		draw_line(t_list *line, t_img *img);
static void	set_pnt(t_pnt *point, t_img *img, int x_index, int y_index);
void		draw_points(t_list *grid, t_img *img);

/* returns a color from a garadient between points "pt0" and "pt1" 
based on how far point "ptm" is along the line btween them*/
static int	color_grade(t_pnt *pt0, t_pnt *ptm, t_pnt *pt1)
{
	int		alpha;
	int		red;
	int		green;
	int		blue;
	float	dif;

	if (pt0->color == pt1->color)
		return (pt0->color);
	dif = sqrtf(powf(ptm->x - pt0->x, 2) + powf(ptm->y - pt0->y, 2));
	dif /= sqrtf(powf(pt1->x - pt0->x, 2) + powf(pt1->y - pt0->y, 2));
	alpha = (pt0->color) >> 24 & 255;
	alpha += (int)((((pt1->color) >> 24 & 255) - alpha) * dif);
	red = (pt0->color) >> 16 & 255;
	red += (int)((((pt1->color) >> 16 & 255) - red) * dif);
	green = (pt0->color) >> 8 & 255;
	green += (int)((((pt1->color) >> 8 & 255) - green) * dif);
	blue = (pt0->color) & 255;
	blue += (int)((((pt1->color) & 255) - blue) * dif);
	return ((alpha << 24) + (red << 16) + (green << 8) + blue);
}

/* draw a line between "point0" and "point1" using Bresenham's line algorithm
'pointm' is updatable progress variable that draws the line pixel by pixel*/
static void	dl_helper(t_pnt *point0, t_pnt pointm, t_pnt *point1, t_img *img)
{
	float	dxy[2];
	int		sxy[2];
	float	error[2];

	dxy[0] = fabs((point1->x) * 1.0 - (pointm.x) * 1.0);
	sxy[0] = ((pointm.x) < (point1->x)) * 2 - 1;
	dxy[1] = -fabs((point1->y) * 1.0 - (pointm.y) * 1.0);
	sxy[1] = ((pointm.y) < (point1->y)) * 2 - 1;
	error[0] = dxy[0] + dxy[1];
	while (pointm.x != point1->x || pointm.y != point1->y)
	{
		pixel_put(img, pointm.x, pointm.y, pointm.color);
		error[1] = 2 * error[0];
		if (error[1] >= dxy[1] && pointm.x != point1->x)
		{
			error[0] += dxy[1];
			pointm.x += sxy[0];
		}
		if (error[1] <= dxy[0] && pointm.y != point1->y)
		{
			error[0] += dxy[0];
			pointm.y += sxy[1];
		}
		pointm.color = color_grade(point0, &pointm, point1);
	}
}

/*traverse the grid "line" and draw a line between each point
and its left and lower neighbour*/
void	draw_line(t_list *line, t_img *img)
{
	t_list	*(pt[2]);

	while (line)
	{
		pt[0] = (line->content);
		pt[1] = NULL;
		if (line->next)
			pt[1] = line->next->content;
		while (pt[0])
		{
			if (pt[0]->next)
				dl_helper(pt[0]->content, *(t_pnt *)(pt[0]->content),
					pt[0]->next->content, img);
			if (pt[1])
			{
				dl_helper(pt[0]->content, *(t_pnt *)(pt[0]->content),
					pt[1]->content, img);
				pt[1] = pt[1]->next;
			}
			pt[0] = pt[0]->next;
		}
		line = line->next;
	}
}

/* calculate the x and y coordinates of "point" based on its x and y index*/
static void	set_pnt(t_pnt *point, t_img *img, int x_index, int y_index)
{
	long long	tempz;

	point->x = x_index * img->nums->cos_a + y_index * img->nums->sin_a;
	point->y = y_index * img->nums->cos_a - x_index * img->nums->sin_a;
	tempz = point->y * img->nums->sin_g
		+ point->z * img->nums->cos_g * img->nums->div;
	point->y = point->y * img->nums->cos_g
		- point->z * img->nums->sin_g * img->nums->div;
	point->x = (point->x) * img->nums->cos_b
		+ tempz * img->nums->sin_b;
}

/* traverse the "grid" and place each point on the imag "img"*/
void	draw_points(t_list *grid, t_img *img)
{
	t_list	*lst_point;
	t_pnt	*pnt;
	int		x_index;
	int		y_index;

	setup_nums(img->nums);
	y_index = (0 - img->nums->hight / 2) * img->nums->div;
	while (grid)
	{
		lst_point = grid->content;
		x_index = (0 - img->nums->width / 2) * img->nums->div;
		while (lst_point)
		{
			pnt = lst_point->content;
			set_pnt(pnt, img, x_index, y_index);
			pnt->y = pnt->y + img->nums->y_trans;
			pnt->x = pnt->x + img->nums->x_trans;
			x_index += img->nums->div;
			pixel_put(img, pnt->x, pnt->y, pnt->color);
			lst_point = lst_point->next;
		}
		y_index += img->nums->div;
		grid = grid->next;
	}
}
