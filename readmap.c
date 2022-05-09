/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 08:04:42 by ayassin           #+#    #+#             */
/*   Updated: 2022/05/09 17:01:09 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/* The encapsulated linked list structure that makes up the "Grid"

Each row is a linked list.
The first column is a linked list with the a pointer to a row in each node
	
* - * - * - * - * - *
|
* - * - * - * - * - *
|
* - * - * - * - * - *
|
* - * - * - * - * - *
|
* - * - * - * - * - *

*/

static int		ft_hexatoi(char *nbr);
static void		clear_str_sep(char **str_sep);
static t_list	*get_pnts(char **str_sep, int i);
static t_list	*get_lines(int fd);
t_list			*get_grid(char *file_name);

/*give a color for each point based on the number after the comma
else give it the base color */
static int	ft_hexatoi(char *nbr)
{
	int	n;

	n = 0;
	nbr = ft_strchr(nbr, ',');
	if (!nbr)
		return (BASE_COLOR);
	n = ft_atoi(nbr + 1);
	if (n)
		return (n);
	while (*nbr != 'x' && *nbr != 'X')
		nbr++;
	while (*(++nbr))
	{
		if (*nbr >= '0' && *nbr <= '9')
			n = 16 * n + *nbr - '0';
		else if (*nbr >= 'a' && *nbr <= 'f')
			n = 16 * n + *nbr - 'a' + 10;
		else if (*nbr >= 'A' && *nbr <= 'F')
			n = 16 * n + *nbr - 'A' + 10;
	}
	return (n);
}

/* free the output of ft_split after we are done with it*/
static void	clear_str_sep(char **str_sep)
{
	int	i;

	i = 0;
	if (str_sep)
	{
		while (str_sep[i])
			free(str_sep[i++]);
		free (str_sep);
	}
}

/*
Return a row linked list populated with the output of ft_split "str_sep"
the i is a varable drclaration cheat to save lines*/
static t_list	*get_pnts(char **str_sep, int i)
{
	t_list	*line;
	t_pnt	*temp;
	t_list	*chk;

	line = NULL;
	if (!str_sep)
		return (NULL);
	while (str_sep[i] && *(str_sep[i]) != '\n')
	{
		temp = (t_pnt *)malloc(sizeof(*temp));
		if (temp)
		{
			temp->z = ft_atoi(str_sep[i]);
			temp->color = ft_hexatoi(str_sep[i]);
		}
		chk = ft_lstnew(temp);
		ft_lstadd_back(&line, chk);
		if (!(chk && chk->content))
		{
			ft_lstclear(&line, free);
			return (NULL);
		}
		++i;
	}
	return (line);
}

/*
1) Read the file "fd" line by line
2) seprate the points by ft_split, 
3) call a function to populate a row per line in a row linked list,
4) add the row pointer as content for a cloumn node
5) add the column node to the end of the existing columns list
*/
static t_list	*get_lines(int fd)
{
	t_list	*grid;
	t_list	*temp;
	char	*str;
	char	**str_sep;

	grid = NULL;
	str = NULL;
	str = get_next_line(fd);
	while (str)
	{
		str_sep = ft_split(str, ' ');
		free(str);
		temp = ft_lstnew(get_pnts(str_sep, 0));
		clear_str_sep(str_sep);
		ft_lstadd_back(&grid, temp);
		if (!(temp && temp->content))
		{
			clear_grid(grid);
			ft_printf("Grid is empty or Malloc has failed for the grid\n");
			return (NULL);
		}
		str = get_next_line(fd);
	}
	return (grid);
}

/*
chech if the file "file_name" is valid then call a function to populate 
the grid form the file and return a pointer to it.
if any malloc fails the grid is cleared or the map is cut short of rows*/
t_list	*get_grid(char *file_name)
{
	t_list	*grid;
	int		fd;

	grid = NULL;
	fd = open(file_name, O_RDONLY);
	if (fd < 0)
		ft_printf("The file \"%s\" does not exist\n", file_name);
	else if (read(fd, "", 0) == -1)
		ft_printf("This \"%s\" is a directory not a map wise guy\n", file_name);
	else if (!ft_strnstr(file_name, ".fdf", ft_strlen(file_name)))
		ft_printf("The file \"%s\" type is invalid\n", file_name);
	else
		grid = get_lines(fd);
	close(fd);
	return (grid);
}
