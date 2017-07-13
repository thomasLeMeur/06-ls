/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/05 17:03:55 by tle-meur          #+#    #+#             */
/*   Updated: 2016/03/29 18:05:21 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "ft_ls.h"

int			lexico_sort(const t_list *a, const t_list *b)
{
	return (ft_strcmp(((t_ls_file *)a->content)->name,
				((t_ls_file *)b->content)->name));
}

static int	timodi_sort(const t_list *a, const t_list *b)
{
	struct stat	*inf1;
	struct stat	*inf2;

	inf1 = &((t_ls_file *)a->content)->st;
	inf2 = &((t_ls_file *)b->content)->st;
	if (inf1->st_mtimespec.tv_sec != inf2->st_mtimespec.tv_sec)
		return (inf2->st_mtimespec.tv_sec - inf1->st_mtimespec.tv_sec);
	else if (inf2->st_mtimespec.tv_nsec != inf1->st_mtimespec.tv_nsec)
		return (inf2->st_mtimespec.tv_nsec - inf1->st_mtimespec.tv_nsec);
	else
		return (lexico_sort(a, b));
}

static int	fisize_sort(const t_list *a, const t_list *b)
{
	struct stat	*inf1;
	struct stat	*inf2;

	inf1 = &((t_ls_file *)a->content)->st;
	inf2 = &((t_ls_file *)b->content)->st;
	if (inf2->st_size != inf1->st_size)
		return (inf2->st_size - inf1->st_size);
	else
		return (lexico_sort(a, b));
}

static void	reverse_sort(t_list **list)
{
	t_list	*a;
	t_list	*b;
	t_list	*c;

	a = *list;
	b = a->next;
	a->next = NULL;
	while (b)
	{
		c = b->next;
		b->next = a;
		a = b;
		b = c;
	}
	*list = a;
}

int			sort_list(t_list **list, t_ls_arg *arg)
{
	if (!list || !*list)
		return (1);
	if (arg->sort.s)
		ft_lstsort(*list, &fisize_sort);
	else if (arg->sort.t && arg->sort.type != 'f')
		ft_lstsort(*list, &timodi_sort);
	else if (arg->sort.type != 'f')
		ft_lstsort(*list, &lexico_sort);
	if (arg->sort.type == 'f' || arg->sort.r)
		reverse_sort(list);
	return (1);
}
