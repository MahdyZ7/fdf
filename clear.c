/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 11:48:12 by ayassin           #+#    #+#             */
/*   Updated: 2022/05/07 19:53:48 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	clear_grid(t_list *grid);
void	clear_spc(float ***arr, int hight);
int		x_press(t_scn *scn);

/* free the grid consiting of two t_list*/
void	clear_grid(t_list *grid)
{
	t_list	*temp;
	t_list	*temp2;

	temp2 = grid;
	while (grid)
	{
		temp = grid->content;
		ft_lstclear(&temp, free);
		temp2 = grid;
		grid = grid->next;
		free(temp2);
	}
}

/* free a 2d array "arr" of a certian "hight"*/
void	clear_spc(float ***arr, int hight)
{
	int	i;

	i = 0;
	while (i < hight && (*arr)[i])
		free((*arr)[i++]);
	free(*arr);
	*arr = NULL;
}

/* free any malloced data and exit the program*/
int	x_press(t_scn *scn)
{
	int	hight;

	if (!scn)
		exit(0);
	if (scn->grid)
	{
		hight = ft_lstsize(scn->grid);
		if (scn->now)
			clear_spc(&(scn->now), hight);
		if (scn->nxt)
			clear_spc(&(scn->nxt), hight);
		clear_grid(scn->grid);
	}
	if (scn->img)
	{
		if (scn->img->img_ptr)
			mlx_destroy_image(scn->mlx_ptr, scn->img->img_ptr);
	}
	if (scn->mlx_ptr && scn->win_ptr)
		mlx_destroy_window(scn->mlx_ptr, scn->win_ptr);
	exit(0);
	return (0);
}
