/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/22 18:59:15 by tle-meur          #+#    #+#             */
/*   Updated: 2016/03/29 20:05:28 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <sys/acl.h>
#include <sys/xattr.h>

int			print_dir_header(t_list *lst, t_ls_arg *a, char *dad, int first)
{
	if (first & 64 || (first == 2 && lst) || (lst && first == 3
		&& ((((t_ls_file *)lst->content)->type & FT_LS_FILE)
		|| (((t_ls_file *)lst->content)->type & FT_LS_LINK && a->fmt == 'l'))))
		return (1);
	if (!first || first & 32 || !(first & (4 | 8)))
		ft_putchar('\n');
	ft_printf("%s:\n", dad);
	return (1);
}

int			write_link(t_ls_file *f)
{
	char	buf[1000];

	ft_memset(buf, 0, 1000);
	readlink(f->path, buf, 1000);
	ft_printf(" -> %s", buf);
	return (1);
}

int			get_date(t_ls_file *f, time_t date)
{
	time_t	now;
	char	*buf;

	if ((now = time(NULL)) == (time_t)-1)
		return (0);
	if (!(buf = ctime(&date)))
		return (0);
	ft_memcpy(f->inf.m, buf + 4, 3);
	f->inf.d = ft_atoi(buf + 8);
	if (date <= now && date + 15724800 >= now)
		ft_memcpy(f->inf.h, buf + 11, 5);
	else
		ft_memcpy(f->inf.h, buf + 20, 4);
	return (1);
}

static void	fill_rights_end(t_ls_file *f, char *dad)
{
	if (f->st.st_mode & S_IXOTH)
		f->inf.r[9] = (f->st.st_mode & S_ISVTX) ? 't' : 'x';
	else
		f->inf.r[9] = (f->st.st_mode & S_ISVTX) ? 'T' : '-';
	if (*f->name == '/')
		ft_sprintf(&f->path, "%s", f->name);
	else
		ft_sprintf(&f->path, "%s/%s", dad, f->name);
	if (f->path)
	{
		if (listxattr(f->path, NULL, 0, XATTR_NOFOLLOW) > 0)
			f->inf.r[10] = '@';
		else if (acl_get_link_np(f->path, ACL_TYPE_EXTENDED) > 0)
			f->inf.r[10] = '+';
	}
}

void		fill_rights(t_ls_file *f, char *dad)
{
	if ((f->m = f->st.st_mode & S_IFMT) == S_IFBLK || f->m == S_IFCHR)
		f->inf.r[0] = (f->m == S_IFBLK) ? 'b' : 'c';
	else if (f->m == S_IFDIR || f->m == S_IFLNK)
		f->inf.r[0] = (f->m == S_IFDIR) ? 'd' : 'l';
	else if (f->m == S_IFSOCK || f->m == S_IFIFO)
		f->inf.r[0] = (f->m == S_IFSOCK) ? 's' : 'p';
	f->inf.r[0] = (f->inf.r[0]) ? f->inf.r[0] : '-';
	f->inf.r[1] = (f->st.st_mode & S_IRUSR) ? 'r' : '-';
	f->inf.r[2] = (f->st.st_mode & S_IWUSR) ? 'w' : '-';
	if (f->st.st_mode & S_IXUSR)
		f->inf.r[3] = (f->st.st_mode & S_ISUID) ? 's' : 'x';
	else
		f->inf.r[3] = (f->st.st_mode & S_ISUID) ? 'S' : '-';
	f->inf.r[4] = (f->st.st_mode & S_IRGRP) ? 'r' : '-';
	f->inf.r[5] = (f->st.st_mode & S_IWGRP) ? 'w' : '-';
	if (f->st.st_mode & S_IXGRP)
		f->inf.r[6] = (f->st.st_mode & S_ISGID) ? 's' : 'x';
	else
		f->inf.r[6] = (f->st.st_mode & S_ISGID) ? 'S' : '-';
	f->inf.r[7] = (f->st.st_mode & S_IROTH) ? 'r' : '-';
	f->inf.r[8] = (f->st.st_mode & S_IWOTH) ? 'w' : '-';
	fill_rights_end(f, dad);
}
