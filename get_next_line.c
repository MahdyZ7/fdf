/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 08:50:48 by ayassin           #+#    #+#             */
/*   Updated: 2022/04/26 14:43:58 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/* shifts the content of the string "str" starting from "index" to the the 
beginning of the string then tirminates the string.
Returns 1 if the starting index is not null char, 0 otherwise*/
static int	shiftleft(char *str, int index)
{
	int	pos;

	pos = 0;
	if (str[index] == 0)
	{
		*str = 0;
		return (0);
	}
	if (str[index] == '\n')
		++index;
	while (str[pos + index])
	{
		str[pos] = str[pos + index];
		pos++;
	}
	str[pos] = 0;
	return (1);
}

/*Appends suffex string "sufstr" to the prefix string "prestr" 
up until "sufstr" char 0 or new line. 
Return 1 if sufstr has new one, -1 if Memory allocation failed, 0 otherwise
**malloc DANGER**/
static int	ft_strjoingnl(char **prestr, char *sufstr)
{
	int		i1;
	int		i2;
	int		lineflag;
	char	*fullstr;

	i1 = 0;
	i2 = 0;
	lineflag = 0;
	fullstr = (char *)malloc(ft_strlen(*prestr) + ft_strlen(sufstr) + 1);
	if (fullstr == NULL)
		return (-10);
	while (*prestr && (*prestr)[i1])
	{
		fullstr[i1] = (*prestr)[i1];
		++i1;
	}
	while (sufstr[i2] && sufstr[i2] != '\n')
		fullstr[i1++] = sufstr[i2++];
	if (shiftleft(sufstr, i2))
		fullstr[i1++] = '\n' + (0 * ++lineflag);
	fullstr[i1] = 0;
	if (*prestr)
		free (*prestr);
	*prestr = fullstr;
	return (lineflag);
}

/* Loops reading "readsize" chars from file discriptor "fd" into buffer "*buff"
until end of line, the line is malloced and returned; 
NULL can be EOF or error*/
char	*get_next_line_loop(char **buff, int readsize, int fd)
{
	int		flag;
	char	*line;

	flag = 0;
	line = NULL;
	while (flag == 0)
	{
		if (**buff == 0)
			readsize = read(fd, *buff, BUFFER_SIZE);
		if (readsize < BUFFER_SIZE)
			(*buff)[readsize] = 0 * ++flag;
		if (readsize < 1)
		{
			if (*buff)
			{
				free (*buff);
				*buff = NULL;
			}
			return (line);
		}
		flag += ft_strjoingnl(&line, *buff);
	}
	return (line);
}

/* Returns one line in the file discribtor "fd"
**malloc DANGER and possible memory leaks for buffer size > line size*/
char	*get_next_line(int fd)
{
	static char	*buff;
	char		*line;
	int			readsize;

	readsize = BUFFER_SIZE;
	if (!(BUFFER_SIZE >= 1 && read(fd, buff, 0) == 0))
		return (NULL);
	if (buff == NULL)
	{
		buff = (char *)malloc((readsize + 1) * sizeof(char));
		if (buff == NULL)
			return (NULL);
		buff[0] = 0;
	}
	buff[readsize] = 0;
	line = get_next_line_loop(&buff, readsize, fd);
	if (buff && *buff == 0)
	{
		free (buff);
		buff = NULL;
	}
	return (line);
}
