/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 13:19:33 by ayassin           #+#    #+#             */
/*   Updated: 2022/05/07 20:16:42 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*Places a 'color' in the image 'data' in the x and y posityion
(put pixel clone)*/
void	pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	if (x >= data->width || y >= data->hight || y < 0 || x < 0)
		return ;
	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

/*saves the sin and cos of angels to save on operations*/
void	setup_nums(t_nums *nums)
{
	nums->cos_a = cosf(nums->alpha);
	nums->sin_a = sinf(nums->alpha);
	nums->cos_b = cosf(nums->beta);
	nums->sin_b = sinf(nums->beta);
	nums->cos_g = cosf(nums->gamma);
	nums->sin_g = sinf(nums->gamma);
}

/* setup the defulats for variables in the t_img struct and t_nums struct*/
void	setup_img(t_img *img, t_scn *scn, int width, int hight)
{
	img->width = width;
	img->hight = hight;
	img->zoom = 10;
	img->img_ptr = mlx_new_image(scn->mlx_ptr, img->width, img->hight);
	if (!img->img_ptr)
		x_press(scn);
	img->addr = mlx_get_data_addr(img->img_ptr, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	if (!img->addr)
		x_press(scn);
	img->x_pos = 0;
	img->y_pos = 0;
	scn->nums->width = ft_lstsize(scn->grid->content);
	scn->nums->hight = ft_lstsize(scn->grid);
	scn->nums->beta = BETA;
	scn->nums->gamma = GAMMA;
	scn->nums->alpha = ALPHA;
	scn->nums->x_trans = (img->width) / 2;
	scn->nums->y_trans = (img->hight) / 2;
	scn->nums->div = (img->width) / 2 / scn->nums->width;
	if (scn->nums->div > (img->hight) / 2 / scn->nums->hight)
		scn->nums->div = (img->hight) / 2 / scn->nums->hight;
	if (scn->nums->div == 0)
		scn->nums->div = 1;
}

/* Decalre the window and image, and set up hooks  and loops of mls*/
int	main_setup(t_scn *scn, char *map)
{
	int		width;
	int		hight;

	width = 1000;
	hight = 1000;
	scn->img->img_ptr = NULL;
	scn->now = NULL;
	scn->nxt = NULL;
	scn->mlx_ptr = mlx_init();
	if (!scn->mlx_ptr)
		x_press(scn);
	if (ft_strchr(map, '/'))
		map = ft_strrchr(map, '/') + 1;
	scn->win_ptr = mlx_new_window(scn->mlx_ptr, width, hight, map);
	if (!scn->win_ptr)
		x_press(scn);
	setup_img(scn->img, scn, width, hight);
	setup_spc(scn);
	redraw(scn, scn->img, scn->img->x_pos, scn->img->y_pos);
	mlx_hook(scn->win_ptr, 2, 1L << 0, key_hook, scn);
	mlx_hook(scn->win_ptr, 17, 0, x_press, scn);
	mlx_loop_hook(scn->mlx_ptr, nxt_step, scn);
	mlx_loop(scn->mlx_ptr);
	return (0);
}

/*creat structs, read grid and call the function mlx function*/
int	main(int argv, char **argc)
{
	t_scn	scn;
	t_img	img;
	t_nums	nums;

	if (argv != 2)
		return (0);
	scn.img = &img;
	img.nums = &nums;
	scn.nums = &nums;
	scn.single_step = 1;
	scn.grid = get_grid(argc[1]);
	if (scn.grid)
		main_setup(&scn, argc[1]);
	return (0);
}
