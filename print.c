/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/22 18:59:15 by tle-meur          #+#    #+#             */
/*   Updated: 2016/03/30 14:39:29 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include "ft_ls.h"

static void	fill_fmt_l_end(t_ls_file *t, int w[6], char *dad)
{
	int	var;

	var = ft_nblen(t->st.st_nlink);
	w[1] = FT_MAX(w[1], var);
	var = ft_strlen(t->inf.o);
	w[2] = FT_MAX(w[2], var);
	var = ft_strlen(t->inf.g);
	w[3] = FT_MAX(w[3], var);
	var = ft_nblen(t->st.st_size);
	w[4] = FT_MAX(w[4], var);
	fill_rights(t, dad);
	var = (ft_strcmp(t->inf.g, "wheel") || (!ft_strcmp(t->inf.o, "root")
		&& t->inf.r[0] != 'd') || t->inf.r[10] == '@'
		|| t->inf.r[10] == '+') ? 11 : 12;
	w[0] = FT_MAX(w[0], var);
}

static int	fill_fmt_l(t_list *lst, t_ls_arg *arg, int w[6], char *dad)
{
	t_ls_file		*t;
	struct passwd	*own;
	struct group	*grp;

	w[1] = 2;
	w[0] = 10;
	while (lst && (t = (t_ls_file *)lst->content)
			&& ((lst = lst->next) || 1))
		if ((!(arg->opts & (a | A)) && *t->name == '.') || ((arg->opts & (A))
					&& (!ft_strcmp(t->name, ".") || !ft_strcmp(t->name, ".."))))
			continue ;
		else if (!(own = getpwuid(t->st.st_uid))
				|| !(t->inf.r = ft_strnew(12))
				|| !(grp = getgrgid(t->st.st_gid))
				|| !(t->inf.o = ft_strdup(own->pw_name))
				|| !(t->inf.g = ft_strdup(grp->gr_name))
				|| !(t->inf.m = ft_strnew(5))
				|| !(t->inf.h = ft_strnew(5))
				|| (!get_date(t, (arg->sort.type == 'c') ?
				t->st.st_ctimespec.tv_sec : t->st.st_mtimespec.tv_sec))
				|| (w[5] += t->st.st_blocks) * 0)
			return (0);
		else
			fill_fmt_l_end(t, w, dad);
	return (1);
}

static int	really_print(t_list *lst, t_ls_arg *o, int w[6], t_ls_file *t)
{
	while (lst && (t = (t_ls_file *)lst->content)
			&& ((lst = lst->next) || 1))
	{
		if ((!(o->opts & (a | A)) && *t->name == '.') || ((o->opts & (A))
			&& (!ft_strcmp(t->name, ".") || !ft_strcmp(t->name, ".."))))
			continue ;
		if (o->fmt == 'l' && ft_printf("%-*s%*d %-*s  %-*s  ", w[0], t->inf.r,
			w[1], t->st.st_nlink, w[2], t->inf.o, w[3], t->inf.g))
		{
			if (t->inf.r[0] == 'c' || t->inf.r[0] == 'b')
				ft_printf("%*lu, %*lu", w[4], MAJOR(t->st.st_rdev / 16), w[4],
						MINOR(t->st.st_rdev));
			else
				ft_printf("%*lu", w[4], t->st.st_size);
			ft_printf(" %3s %2d %5s ", t->inf.m, t->inf.d, t->inf.h);
		}
		ft_printf("%s", t->name);
		if (t->type & FT_LS_LINK && o->fmt == 'l' && !write_link(t))
			return (0);
		else if (t->type & FT_LS_DIR && !(t->type & FT_LS_LINK) && o->opts & p)
			ft_putchar('/');
		ft_putchar('\n');
	}
	return (1);
}

int			my_print(t_list *lst, t_ls_arg *arg, char *dad, int first)
{
	DIR			*dir;
	int			w[6];

	if (!dad || (lst && first & 1 && !(((t_ls_file *)lst->content)->type &
		FT_LS_FILE) && !(((t_ls_file *)lst->content)->type & FT_LS_LINK
		&& arg->fmt == 'l')))
		return ((dad && 1));
	ft_memset(w, 0, 6 * sizeof(int));
	if (print_dir_header(lst, arg, dad, first) && !lst)
	{
		if (!(dir = opendir(dad)))
		{
			ft_printf_fd(2, "ls: %s: ", ft_strrchr(dad, '/') + 1);
			perror("");
		}
		return (!(dir && closedir(dir)));
	}
	if (arg->fmt == 'l' && fill_fmt_l(lst, arg, w, dad) == 0)
		return (0);
	if (arg->fmt == 'l' && (ft_strcmp(dad, ".")
				|| ((first == 2 || first & 64) && !ft_strcmp(dad, "."))))
		ft_printf("total %d\n", w[5]);
	return (really_print(lst, arg, w, NULL));
}
