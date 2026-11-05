/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-el <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 14:43:01 by oben-el           #+#    #+#             */
/*   Updated: 2026/11/05 16:54:34 by oben-el          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!read_file(fd, &stash))
		return (NULL);
	line = extract_line(stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	if (!update_stash(&stash, line))
		return (free(line), NULL);
	return (line);
}

int	read_file(int fd, char **stash)
{
	char	buffer[(size_t)BUFFER_SIZE + 1];
	ssize_t	bytes_read;

	while (!ft_strchr(*stash, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(*stash);
			*stash = NULL;
			return (0);
		}
		if (bytes_read == 0)
			return (1);
		buffer[bytes_read] = '\0';
		*stash = ft_strjoin(*stash, buffer);
		if (!*stash)
			return (0);
	}
	return (1);
}

char	*extract_line(char	*stash)
{
	char	*line;
	size_t	i;

	i = 0;
	while (stash[i])
		if (stash[i++] == '\n')
			break ;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i])
	{
		line[i] = stash[i];
		if (stash[i++] == '\n')
			break ;
	}
	line[i] = '\0';
	return (line);
}

int	update_stash(char	**stash, char *line)
{
	char	*cleaned;
	size_t	i;
	size_t	j;

	i = ft_strlen(line);
	if (!(*stash)[i])
	{
		
}
