/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/05 13:56:54 by tle-meur          #+#    #+#             */
/*   Updated: 2016/03/31 18:00:17 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "ft_ls.h"

int			print_help_file(char ***file_args)
{
	int		fd;
	int		ret;
	char	*line;

	line = NULL;
	ft_tabdel((void ***)file_args);
	if ((fd = open("help.txt", O_RDONLY)) == -1)
		return (3);
	while ((ret = ft_gnl(fd, &line)) > 0)
		ft_putendl(line);
	ft_memdel((void **)&line);
	if ((fd = close(fd)) == -1)
		return ((!ret) ? 4 : 5);
	return ((!ret) ? 0 : 6);
}

void		clean_options(char *flags, int ac, char **av)
{
	int	i;
	int	j;
	int	readable;

	i = 0;
	readable = 1;
	while (++i < ac && readable)
		if (av[i][0] == '-' && av[i][1] == '-' && !av[i][2])
			readable = 0;
		else if (av[i][0] == '-' && av[i][1] && av[i][1] != '-' && !(j = 0))
			while (av[i][++j])
			{
				if (av[i][j] == '1' && (flags[10] = 1))
					flags[4] = 0;
				else if (av[i][j] == 'l' && (flags[4] = 1))
					flags[10] = 0;
			}
}

void		fill_arg_options(t_ls_arg *arg)
{
	if (arg->flags[1][0] == 1 && !arg->flags[1][1])
		arg->opts |= A;
	if (arg->flags[1][1] == 1)
		arg->opts |= a;
	if (arg->flags[1][2] == 1 && (arg->sort.type = 'f')
			&& (arg->opts |= a))
		arg->opts -= ((arg->opts & A) ? A : 0);
	if (arg->flags[1][4] == 1)
		arg->fmt = 'l';
	if (arg->flags[1][5] == 1)
		arg->opts |= p;
	if (arg->flags[1][6] == 1)
		arg->opts |= R;
	if (arg->flags[1][7] == 1 && !arg->flags[1][2])
		arg->sort.r = 1;
	if (arg->flags[1][8] == 1 && !arg->flags[1][2])
		arg->sort.s = 1;
	if (arg->flags[1][9] == 1 && !arg->flags[1][2])
		arg->sort.t = 1;
	if (arg->flags[1][10] == 1)
		arg->fmt = '1';
}

static int	push_and_quit(t_list **list, t_list *tmp)
{
	ft_lstpush(list, tmp);
	return (1);
}

int			tab_to_list_init(t_list **list, t_list *tmp, t_ls_arg *a, DIR **dir)
{
	char		c;
	t_ls_file	*t;

	t = tmp->content;
	if (stat(t->name, &t->st) == -1)
	{
		if (readlink(t->name, &c, 1) != -1 && (t->type |= FT_LS_LINK))
		{
			if (lstat(t->name, &t->st) == -1)
				return (push_and_quit(list, tmp));
		}
		else
			return (push_and_quit(list, tmp));
	}
	if ((t->type & FT_LS_LINK || readlink(t->name, &c, 1) != -1)
		&& (t->type |= FT_LS_LINK) && a->fmt == 'l'
		&& lstat(t->name, &t->st) < 0)
		return (push_and_quit(list, tmp));
	t->m = (t->st.st_mode & S_IFMT);
	t->type |= (t->m == S_IFDIR) ? FT_LS_DIR : FT_LS_FILE;
	if (t->type & FT_LS_DIR && !(*dir = opendir(t->name)))
		return (push_and_quit(list, tmp));
	return (0);
}
