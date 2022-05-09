/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 19:52:57 by ayassin           #+#    #+#             */
/*   Updated: 2022/04/07 09:16:56 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Return int length of string "s1"
**NOT PROTECTED VS NULL INPUT**/
int	ft_strlen(const char *s)
{
	int	len;

	if (s == NULL)
		return (0);
	len = -1;
	while (s[++len])
		;
	return (len);
}
