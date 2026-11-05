/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-el <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 14:43:01 by oben-el           #+#    #+#             */
/*   Updated: 2026/11/05 15:23:43 by oben-el          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	temp = read_fd(fd, stash);
	if (!temp)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	line = extract_line(stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	temp = update_stash(stash);
	if (!temp)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	return (line);
}

char	*read_fd(int fd, char *stash)
{
	char	*buffer;
	size_t	bytes_read;

	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (!buffer)
		return ;
	buffer[0] = '\0';
	while (!ft_strchr(stash, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			return (free(buffer), NULL);
		buffer[bytes_read] = '\0';
		stash = ft_strjoin(stash, buffer);
		if (!stash)
			return (NULL);
	}
	free(buffer);
	return (stash);
}

char	*extract_line(char	*stash)
{
	char	*line;
	size_t	i;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	i = -1;
	while (stash[++i] && stash[i] != '\n')
		line[i] = stash[i];
	if (stash[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*update_stash(char *stash)
{
	char	*new_stash;
	size_t	i;
	size_t	j;

	i = 0;
	while (stash[i])
		if (stash[i++] == '\n')
			break;
	if (!stash[i])
		return (NULL);
	j = ft_strlen(stash + i);
	new_stash = malloc(len + 1);
	if (!new_stash)
		return (NULL);
	j = 0;
	while (stash[i + j])
	{
		new_stash[j] = stash[i + j];
		j++;
	}
	new_stash[j] = '\0';
	return (new_stash);
}
