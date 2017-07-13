/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/05 16:43:45 by tle-meur          #+#    #+#             */
/*   Updated: 2016/03/25 19:51:30 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_and_quit(char ***tab, char *name, char bad_option)
{
	if (bad_option)
		ft_printf_fd(2, "%s: illegal option -- %c\n", name, bad_option);
	ft_tabdel((void ***)tab);
	if (!bad_option)
		exit(2);
	return (NULL);
}

static void	clean_tab(char *readable, char ***tab)
{
	*readable = -1;
	ft_tabdel((void ***)tab);
	if (!(*tab = (char **)ft_memalloc(2 * sizeof(char *))))
		exit(1);
	if (!((*tab)[0] = ft_strdup("")))
		free_and_quit(tab, NULL, 0);
}

static char	read_flags(char *str, char *readable, char *opts, char *flags)
{
	char	*ret;

	if (*str == '-' && *(str + 1))
		return (*str);
	if (*str == '-' && str++)
		*readable = 0;
	while (*str)
	{
		if (!(ret = ft_strchr(opts, *str)))
			return (*str);
		flags[ret - opts] = 1;
		str++;
	}
	return (0);
}

char		**ft_unixcmd_parser(int ac, char **av, char *opts, char *flags)
{
	int		i;
	char	bad;
	int		ind;
	char	**tab;
	char	readable;

	i = 0;
	ind = 0;
	readable = 1;
	if (!(tab = (char **)ft_memalloc(ac * sizeof(char *))))
		exit(1);
	while (++i < ac && readable != -1)
		if (!*av[i])
			clean_tab(&readable, &tab);
		else if (readable && *av[i] == '-' && ((av[i][1] && av[i][1] != '-')
			|| (av[i][1] == '-')))
		{
			if ((bad = read_flags(av[i] + 1, &readable, opts, flags)))
				return (free_and_quit(&tab, *av, bad));
		}
		else if (!(readable = 0))
			if (!(tab[ind++] = ft_strdup(av[i])))
				return (free_and_quit(&tab, NULL, 0));
	return (tab);
}
