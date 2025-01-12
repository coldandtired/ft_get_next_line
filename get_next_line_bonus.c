/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwillis <lwillis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:19:18 by lwillis           #+#    #+#             */
/*   Updated: 2024/12/20 10:28:23 by lwillis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*join_and_free(char *old, char *new)
{
	char	*tmp;

	if (!old)
		old = ft_strdup("");
	tmp = ft_strjoin(old, new);
	free(old);
	return (tmp);
}

static char	*get_full_buffer(int fd, char *stash)
{
	char	*buffer;
	ssize_t	bytes_read;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		if (bytes_read == -1)
		{
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = 0;
		stash = join_and_free(stash, buffer);
		if (get_first_nl(buffer) > -1)
			break ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	return (stash);
}

static char	*get_first_line(char *stash)
{
	ssize_t	pos;

	if (!stash[0])
		return (NULL);
	pos = get_first_nl(stash);
	if (-1 == pos)
		return (ft_strdup(stash));
	return (ft_substr(stash, 0, pos + 1));
}

static char	*get_new_stash(char *stash)
{
	ssize_t	pos;
	char	*new_stash;

	if (!stash)
		return (NULL);
	pos = get_first_nl(stash);
	new_stash = ft_strdup(&stash[pos + 1]);
	free(stash);
	return (new_stash);
}

char	*get_next_line(int fd)
{
	static char	*stash[10000];
	char		*line;

	if (fd < 0 || fd > 9999 || BUFFER_SIZE < 1)
		return (NULL);
	stash[fd] = get_full_buffer(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = get_first_line(stash[fd]);
	if (-1 == get_first_nl(stash[fd]))
	{
		free(stash[fd]);
		stash[fd] = NULL;
	}
	else
		stash[fd] = get_new_stash(stash[fd]);
	if (stash[fd] && 0 == ft_strlen(stash[fd]))
	{
		free(stash[fd]);
		stash[fd] = NULL;
	}
	return (line);
}
