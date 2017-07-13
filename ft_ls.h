/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/05 13:56:19 by tle-meur          #+#    #+#             */
/*   Updated: 2016/03/29 19:42:03 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <time.h>
# include <dirent.h>
# include <sys/stat.h>
# include "libft.h"

# define NB_FLAGS	11
# define FT_LS_DIR	1
# define FT_LS_FILE	2
# define FT_LS_LINK	4

# define MINORBITS	20
# define MINORMASK	((1U << MINORBITS) - 1)
# define MAJOR(dev)	((unsigned int) ((dev) >> MINORBITS))
# define MINOR(dev) ((unsigned int) ((dev) & MINORMASK))

typedef struct	s_sort
{
	char	t;
	char	s;
	char	type;
	char	r;
}				t_sort;

enum			e_ls_options
{
	a = 1,
	A = 2,
	p = 4,
	R = 8
};

typedef struct	s_ls_arg
{
	char				fmt;
	t_sort				sort;
	enum e_ls_options	opts;
	char				flags[2][NB_FLAGS + 1];
}				t_ls_arg;

typedef struct	s_ls_inf
{
	char	*r;
	char	*o;
	char	*g;
	char	*m;
	int		d;
	char	*h;
}				t_ls_inf;

typedef	struct	s_ls_file
{
	int			m;
	struct stat	st;
	t_ls_inf	inf;
	char		type;
	char		*name;
	char		*path;
}				t_ls_file;

/*
**	utilities.c
*/
int				ft_nblen(long nb);
void			my_lstdel(void *content, size_t size);
int				del_lists(t_list ***lists, char ***tab, int ret);

/*
**	utilities_main.c
*/
void			fill_arg_options(t_ls_arg *arg);
int				print_help_file(char ***file_args);
void			clean_options(char *flags, int ac, char **av);
int				tab_to_list_init(t_list **lists, t_list *tmp, t_ls_arg *a,
				DIR **dir);

/*
**	compute.c
*/
int				create_child_list(t_list **lst, t_ls_arg *arg, char *dad);
int				my_compute(t_list *lst, t_ls_arg *arg, char *dad, int first);

/*
**	sort.c
*/
int				sort_list(t_list **list, t_ls_arg *arg);
int				lexico_sort(const t_list *a, const t_list *b);

/*
**	print.c
*/
int				my_print(t_list *lst, t_ls_arg *arg, char *dad, int first);

/*
**	utilities_print.c
*/
int				write_link(t_ls_file *f);
int				get_date(t_ls_file *f, time_t date);
void			fill_rights(t_ls_file *f, char *dad);
int				print_dir_header(t_list *lst, t_ls_arg *arg, char *dad, int f);

#endif
