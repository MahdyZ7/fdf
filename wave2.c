/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wave2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 11:27:27 by ayassin           #+#    #+#             */
/*   Updated: 2022/05/09 10:13:50 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*  the wave eq Z_tt(x,y,t) = c^2 (Z_xx(x,y,t) + Z_yy(x,y,t))*/

int			nxt_step(t_scn *scn);
static void	update_grid(t_scn *scn);
static int	color(t_pnt *pt);
static void	boundary(t_scn *scn, int i, int j, float sxy[2]);
static void	comp_nxt(t_scn *scn, int width, int hight);

/* move a step forward in time and update the arrays accordingly
the  "signel step == 0" is to hult the mlx loop hook
1)	update the grid with values from the now array
2)	redraw the image based on the new grid
3)	swap the now and nxt grid pointers so that the nxt array in the now
4)	compute the new values to the nxt array*/
int	nxt_step(t_scn *scn)
{
	float	**temp;

	if (scn->single_step == 0)
	{
		update_grid(scn);
		redraw(scn, scn->img, scn->img->x_pos, scn->img->y_pos);
		temp = scn->now;
		scn->now = scn->nxt;
		scn->nxt = temp;
		comp_nxt(scn, ft_lstsize(scn->grid->content), ft_lstsize(scn->grid));
	}
	return (0);
}

/* 
change the color of the point based of the hight "z"
1) bright green for z = 0
2) lime green to orange for z > 0
3) teal to dark blue for z < 0
*/
static int	color(t_pnt *pt)
{
	int		red;
	int		green;
	int		blue;
	float	dif;

	red = 0;
	green = 0xff;
	blue = 0x80;
	dif = 2 / (1 + expf(-0.1 * pt->z)) - 1.0;
	if (pt->z > 0)
	{
		blue = 0;
		green = 0x80 + 0x7f * (1 - dif);
		red = 0x80 + 0x7f * dif;
	}
	else if (pt->z < 0)
	{
		red = 0;
		green = 0x80 * (1 + dif);
		blue = 0x80 + 0x7f * -dif;
	}
	return ((red << 16) + (green << 8) + blue);
}

/* populate each point in the grid with points from the now array
and update the color*/
static void	update_grid(t_scn *scn)
{
	int		i;
	int		j;
	t_list	*pt;
	t_list	*line;

	i = 0;
	line = scn->grid;
	while (line)
	{
		j = 0;
		pt = (line->content);
		while (pt)
		{
			((t_pnt *)(pt->content))->z = (scn->now)[i][j];
			((t_pnt *)(pt->content))->color
				= color(pt->content);
			j++;
			pt = pt->next;
		}
		i++;
		line = line->next;
	}
}

/* Use neuman boudary conditions Z_x(x,y,t) = 0 and Z_y(x,y,t) = 0.
which means taking double the other side if one side is outside the array
eg for x = 0 & y = 1 :
Z(0,1,t+1) = Sx * 2 * Z(1,1,t) + Sy * (Z(0,y + 1,t) + Z(0,y-1,t))
			+ 2 * (1 - Sx - Sy) * Z(x,y,t) - Z(x,y,t-1)*/
static void	boundary(t_scn *scn, int i, int j, float sxy[2])
{
	int	hight;
	int	width;

	hight = scn->nums->hight;
	width = scn->nums->width;
	if (i == 0 && i + 1 < hight)
		(scn->nxt)[i][j] += sxy[1] * 2 * ((scn->now)[i + 1][j]);
	else if (i == hight - 1 && i - 1 >= 0)
		(scn->nxt)[i][j] += sxy[1] * 2 * ((scn->now)[i - 1][j]);
	else if (i != 0 && i != hight - 1)
		(scn->nxt)[i][j]
			+= sxy[1] * ((scn->now)[i - 1][j] + (scn->now)[i + 1][j]);
	if (j == 0 && j + 1 < width)
		(scn->nxt)[i][j] += sxy[0] * 2 * ((scn->now)[i][j + 1]);
	else if (j == width - 1 && j - 1 >= 0)
		(scn->nxt)[i][j] += sxy[0] * 2 * ((scn->now)[i][j - 1]);
	else if (j != 0 && j != width - 1)
		(scn->nxt)[i][j]
			+= sxy[0] * ((scn->now)[i][j - 1] + (scn->now)[i][j + 1]);
}

/* compute the next step using the solution of the wave function
Z(x,y,t+1) = Sx * (Z(x+1,y,t) + Z(x-1,y,t)) + Sy * (Z(x,y + 1,t) + Z(x,y-1,t))
			+ 2 * (1 - Sx - Sy) * Z(x,y,t) - Z(x,y,t-1)*/
void	comp_nxt(t_scn *scn, int width, int hight)
{
	float	sxy[2];
	int		i;
	int		j;

	sxy[0] = STABILITY * (1 + (hight == 1)) * (width != 1);
	sxy[1] = STABILITY * (1 + (width == 1)) * (hight != 1);
	i = -1;
	while (++i < hight)
	{
		j = -1;
		while (++j < width)
		{
			(scn->nxt)[i][j] = 2 * (1 - sxy[0] - sxy[1]) * ((scn->now)
				[i][j]) - (scn->nxt)[i][j];
			if (i * j == 0 || i == hight - 1 || j == width - 1)
				boundary(scn, i, j, sxy);
			else
			(scn->nxt)[i][j] += (sxy[0]) * ((scn->now)
					[i][j - 1] + (scn->now)[i][j + 1])
					+ (sxy[1]) * ((scn->now)[i - 1][j]
						+ (scn->now)[i + 1][j]);
		}
	}
}
