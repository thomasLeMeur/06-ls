/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/21 16:47:43 by tle-meur          #+#    #+#             */
/*   Updated: 2016/03/29 18:42:34 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		my_lstdel(void *content, size_t size)
{
	(void)size;
	if (!content)
		return ;
	ft_memdel((void **)&((t_ls_file *)content)->name);
	ft_memdel((void **)&((t_ls_file *)content)->path);
	ft_memdel((void **)&((t_ls_file *)content)->inf.r);
	ft_memdel((void **)&((t_ls_file *)content)->inf.o);
	ft_memdel((void **)&((t_ls_file *)content)->inf.g);
	ft_memdel((void **)&((t_ls_file *)content)->inf.m);
	ft_memdel((void **)&((t_ls_file *)content)->inf.h);
	free(content);
}

int			del_lists(t_list ***lists, char ***tab, int ret)
{
	ft_tabdel((void ***)tab);
	if (lists && *lists)
	{
		ft_lstdel(&(*lists)[0], &my_lstdel);
		ft_lstdel(&(*lists)[1], &my_lstdel);
		ft_lstdel(&(*lists)[2], &my_lstdel);
	}
	ft_memdel((void **)lists);
	return (ret);
}

int			ft_nblen(long nb)
{
	int	len;

	len = 0;
	if (!nb)
		return (1);
	while (nb)
	{
		nb /= 10;
		len++;
	}
	return (len);
}
