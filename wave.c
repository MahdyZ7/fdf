/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wave.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 12:08:23 by ayassin           #+#    #+#             */
/*   Updated: 2022/05/07 19:58:31 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*  the wave eq Z_tt(x,y,t) = c^2 (Z_xx(x,y,t) + Z_yy(x,y,t))*/

static void	int_spc1(t_scn *scn, int width, int hight);
static void	y_bounds(t_scn *scn, int hight, int width, float sxy[2]);
static void	x_bounds(t_scn *scn, int hight, int width, float sxy[2]);
static void	int_spc2(t_scn *scn, int width, int hight);
void		setup_spc(t_scn *scn);

/* Malloc two arrays "now" and "nxt" and call functions to fill them up*/
void	setup_spc(t_scn *scn)
{
	int	width;
	int	hight;
	int	i;

	width = ft_lstsize(scn->grid->content);
	hight = ft_lstsize(scn->grid);
	scn->now = malloc(sizeof(*(scn->now)) * hight);
	scn->nxt = malloc(sizeof(*(scn->now)) * hight);
	if (!(scn->nxt) || !(scn->now))
		x_press(scn);
	i = 0;
	while (i < hight)
	{
		(scn->now)[i] = malloc(sizeof(**(scn->now)) * width);
		(scn->nxt)[i] = malloc(sizeof(**(scn->now)) * width);
		if (!(scn->nxt)[i] || !(scn->now)[i])
			x_press(scn);
		++i;
	}
	int_spc1(scn, width, hight);
}

/* initialize the now array based on the first intial condition 
Z(x,y,t) = f(x,y,t) = map(x,y) ; t = 0*/
static void	int_spc1(t_scn *scn, int width, int hight)
{
	int		i;
	int		j;
	t_list	*line;
	t_list	*point;

	i = 0;
	line = scn->grid;
	while (line)
	{
		j = 0;
		point = line->content;
		while (point)
		{
			(scn->now)[i][j] = ((t_pnt *)(point->content))->z;
			point = point->next;
			++j;
		}
		line = line->next;
		++i;
	}
	int_spc2(scn, width, hight);
}

/* Use neuman boudary conditions Z_x(x,y,t) = 0 and Z_y(x,y,t) = 0
for y = 0 and y = hight - 1.
which means taking double the other side if one side is outside the array
*/
static void	y_bounds(t_scn *scn, int hight, int width, float sxy[2])
{
	int	y;
	int	x;

	y = 0;
	while (y < hight)
	{
		x = -1;
		while (++x < width)
		{
			(scn->nxt)[y][x]
				= (1 - 2 * sxy[0] - 2 * sxy[1]) * ((scn->now)[y][x]);
			if (y == 0 && y + 1 < hight)
				(scn->nxt)[y][x] += sxy[1] * 2 * ((scn->now)[y + 1][x]);
			else if (y == hight - 1 && y - 1 >= 0)
				(scn->nxt)[y][x] += sxy[1] * 2 * ((scn->now)[y - 1][x]);
			if (x == 0 && x + 1 < width)
				(scn->nxt)[y][x] += sxy[0] * 2 * ((scn->now)[y][x + 1]);
			else if (x == width - 1 && x - 1 >= 0)
				(scn->nxt)[y][x] += sxy[0] * 2 * ((scn->now)[y][x - 1]);
			else if (x != 0 && x != width - 1)
				(scn->nxt)[y][x] += sxy[0]
					* ((scn->now)[y][x - 1] + (scn->now)[y][x + 1]);
		}
		y += (hight == 1) + hight - 1;
	}
}

/* Use neuman boudary conditions Z_x(x,y,t) = 0 and Z_y(x,y,t) = 0
for x = 0 and x = width - 1 (less the four corners).
which means taking double the other side if one side is outside the array
*/
static void	x_bounds(t_scn *scn, int hight, int width, float sxy[2])
{
	int	y;
	int	x;

	x = 0;
	while (x < width)
	{
		y = 0;
		while (++y < hight - 1)
		{
			(scn->nxt)[y][x]
				= (1 - 2 * sxy[0] - 2 * sxy[1]) * ((scn->now)[y][x]);
			if (y - 1 >= 0 && y + 1 < hight)
				(scn->nxt)[y][x] += sxy[1]
					* ((scn->now)[y - 1][x] + (scn->now)[y + 1][x]);
			if (x == 0 && x + 1 < width)
				(scn->nxt)[y][x] += sxy[0] * 2 * ((scn->now)[y][x + 1]);
			else if (x == width - 1 && x - 1 >= 0)
				(scn->nxt)[y][x] += sxy[0] * 2 * ((scn->now)[y][x - 1]);
		}
		x += (width == 1) + width - 1;
	}
}

/* initialize the nxt array based on the second intial condition Z_t = 0.
Z(x,y,1) = 0.5*Sx*(Z(x+1,y,0) + Z(x-1,y,0)) + 0.5*Sy*(Z(x,y + 1,0) + Z(x,y-1,0)) 
			+ (1 - Sx - Sy) * Z(x,y,0)*/
void	int_spc2(t_scn *scn, int width, int hight)
{
	int		y;
	int		x;
	float	sxy[2];

	sxy[0] = STABILITY / 2 * (1 + (hight == 1)) * (width != 1);
	sxy[1] = STABILITY / 2 * (1 + (width == 1)) * (hight != 1);
	y = 0;
	y_bounds(scn, hight, width, sxy);
	x_bounds(scn, hight, width, sxy);
	while (++y < hight - 1)
	{
		x = 0;
		while (++x < width - 1)
		{
			(scn->nxt)[y][x]
				= sxy[0] * ((scn->now)[y][x - 1] + (scn->now)[y][x + 1])
				+ sxy[1] * ((scn->now)[y - 1][x] + (scn->now)[y + 1][x])
				+ (1 - 2 * sxy[0] - 2 * sxy[1]) * ((scn->now)[y][x]);
		}
	}
}
