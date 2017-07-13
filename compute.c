/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/21 18:30:21 by tle-meur          #+#    #+#             */
/*   Updated: 2016/03/31 18:01:44 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	f_and_q(t_list **lst, t_list **other, char **path, int ret)
{
	if (path && *path)
		ft_lstdel(other, &my_lstdel);
	ft_memdel((void **)path);
	ft_lstdel(lst, &my_lstdel);
	return (ret);
}

static int	fill_stat(t_list *lst, char *dad)
{
	char		c;
	t_ls_file	*file;

	while (lst && (file = ((t_ls_file *)lst->content)))
	{
		if (dad)
			ft_sprintf(&file->path, "%s/%s", dad, file->name);
		else
			ft_sprintf(&file->path, "%s", file->name);
		if (!file->path || lstat(file->path, &file->st) * 0)
			return (f_and_q(NULL, NULL, NULL, 10));
		if (readlink(file->path, &c, 1) != -1)
			file->type |= FT_LS_LINK;
		file->m = file->st.st_mode & S_IFMT;
		file->type |= (file->m == S_IFDIR) ? FT_LS_DIR : FT_LS_FILE;
		lst = lst->next;
	}
	return (1);
}

int			create_child_list(t_list **lst, t_ls_arg *arg, char *dad)
{
	t_ls_file		add;
	int				ret;
	DIR				*dir;
	t_list			*tmp;
	struct dirent	*file;

	if (!(dir = opendir((dad) ? dad : ".")))
		return (1);
	ft_memset(&add, 0, sizeof(t_ls_file));
	while ((file = readdir(dir)))
		if (!(arg->opts & (a | A)) && *file->d_name == '.')
			;
		else if (!(add.name = ft_strdup(file->d_name))
			|| !(tmp = ft_lstnew(&add, sizeof(t_ls_file))))
			return ((closedir(dir) && 0)
			+ f_and_q(lst, NULL, &add.name, (add.name) ? 8 : 9));
		else
			ft_lstadd(lst, tmp);
	if ((ret = fill_stat(*lst, dad)) != 1 && ((closedir(dir) || 1)))
		return (f_and_q(lst, NULL, NULL, ret));
	if (closedir(dir))
		return (f_and_q(lst, NULL, NULL, 12));
	return (1);
}

static void	sprintf_trash(t_list *lst, char **path, char *dad, int first)
{
	*path = NULL;
	if (first & 1)
		ft_sprintf(path, "%s", ((t_ls_file *)lst->content)->name);
	else if (dad && *dad == '/' && !*(dad + 1))
		ft_sprintf(path, "/%s", ((t_ls_file *)lst->content)->name);
	else
		ft_sprintf(path, "%s/%s", dad, ((t_ls_file *)lst->content)->name);
}

int			my_compute(t_list *l, t_ls_arg *arg, char *dad, int f)
{
	char		*p;
	int			r;
	t_list		*tmp;
	t_ls_file	*fi;

	if (sort_list(&l, arg) && (r = my_print(l, arg, dad, f)) != 1)
		return (f_and_q(&l, NULL, NULL, r));
	while ((arg->opts & R || f & 1) && l && !(tmp = NULL)
		&& (fi = ((t_ls_file *)l->content)))
	{
		if (((ft_strcmp(fi->name, ".") && ft_strcmp(fi->name, "..")) || f & 1)
			&& (!(fi->type & FT_LS_LINK) || f & 1) && fi->type & FT_LS_DIR)
		{
			sprintf_trash(l, &p, dad, f);
			if (!p || (r = create_child_list(&tmp, arg, p)) != 1 || my_compute(
			tmp, arg, p, (f & 1) ? 2 + (f & 44) + (f == 1) * 64 : f - (f & 2)))
				return (f_and_q(&l, &tmp, &p, (r == 1 || !p) ? r + 700 : 14));
			ft_memdel((void **)&p);
		}
		f -= (f & 4) + (f & 8) + (f & 64) - ((f & 76) && 1) * 32;
		tmp = l;
		l = tmp->next;
		ft_lstdelone(&tmp, &my_lstdel);
	}
	return (f_and_q(&l, NULL, NULL, 0));
}
