/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 17:10:00 by ayassin           #+#    #+#             */
/*   Updated: 2021/12/26 11:01:49 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* use "del" function on the content of node "lst"*/
void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst)
	{
		del(lst->content);
		free(lst);
	}
}
