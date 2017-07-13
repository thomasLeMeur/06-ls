/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/05 13:56:54 by tle-meur          #+#    #+#             */
/*   Updated: 2016/03/31 17:51:51 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ft_ls.h"

static int	tab_to_lists(t_list ***lists, t_ls_arg *a, char ***tab)
{
	int			i;
	t_ls_file	*t;
	t_ls_file	add;
	DIR			*dir;
	t_list		*tmp;

	ft_memset(&add, (i = -1) + 1, sizeof(t_ls_file));
	while ((*tab)[++i])
	{
		if (!(tmp = ft_lstnew(&add, sizeof(t_ls_file)))
				|| !(t = (t_ls_file *)tmp->content)
				|| !(t->name = ft_strdup((*tab)[i])))
			return (del_lists(lists, tab, (tmp) ? 1000 : 1001));
		if (tab_to_list_init(&(*lists)[0], tmp, a, &dir))
			continue ;
		if (t->type & FT_LS_DIR && closedir(dir))
			return (del_lists(lists, tab, 1002));
		ft_lstpush((t->type & FT_LS_FILE || (t->type & FT_LS_LINK
			&& a->fmt == 'l')) ? &(*lists)[1] : &(*lists)[2], tmp);
	}
	return (del_lists(NULL, tab, 1));
}

static void	print_bads(t_list *lst)
{
	struct stat	tmp;

	while (lst)
	{
		stat(((t_ls_file *)lst->content)->name, &tmp);
		if (!*((t_ls_file *)lst->content)->name)
			ft_printf_fd(2, "ls: fts_open: ");
		else
			ft_printf_fd(2, "ls: %s: ", ((t_ls_file *)lst->content)->name);
		perror("");
		lst = lst->next;
	}
}

static int	compute_with_args(t_ls_arg *arg, char ***file_args)
{
	int		ret;
	t_list	**list;

	if (!(list = (t_list **)ft_memalloc(3 * sizeof(t_list *))))
		return (999);
	if ((ret = tab_to_lists(&list, arg, file_args) != 1))
		return (del_lists(&list, NULL, ret));
	if (arg->sort.type != 'f')
		ft_lstsort(list[0], &lexico_sort);
	sort_list(&list[1], arg);
	sort_list(&list[2], arg);
	print_bads(list[0]);
	if (list[1])
		if (!(ret = my_print(list[1], arg, ".", 3))
				|| (list[2] && ft_printf("\n") * 0))
			return (del_lists(&list, NULL, ret));
	if (list[2])
	{
		if (list[0] || list[1] || list[2]->next)
			ret = my_compute(list[2], arg, ".", (list[0]) ? 5 : 9);
		else
			ret = my_compute(list[2], arg, ".", 1);
		list[2] = NULL;
	}
	return (del_lists(&list, NULL, ret));
}

int			main(int ac, char **av)
{
	t_ls_arg	arg;
	int			r;
	t_list		*list;
	char		**file_args;

	ft_memset(&arg, 0, sizeof(t_ls_arg));
	ft_memcpy(arg.flags[0], "AafhlpRrSt1", NB_FLAGS);
	if (!(file_args = ft_unixcmd_parser(ac, av, arg.flags[0], arg.flags[1])))
		return ((r =write(2, "usage: ls [-ARSafhlprt1] [file ...]\n", 36)) * 0);
	if (arg.flags[1][3] == 1)
		return (print_help_file(&file_args));
	clean_options(arg.flags[1], ac, av);
	arg.fmt = '1';
	fill_arg_options(&arg);
	list = NULL;
	if (*file_args)
		return (compute_with_args(&arg, &file_args));
	else
	{
		free(file_args);
		if ((r = create_child_list(&list, &arg, NULL)) != 1)
			return (r);
		return (my_compute(list, &arg, ".", 2));
	}
}
