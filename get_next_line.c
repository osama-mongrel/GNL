/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-el <oben-el@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 08:34:14 by oben-el           #+#    #+#             */
/*   Updated: 2026/11/10 08:44:13 by oben-el          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*saved;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	saved = read_from_file(fd, saved);
	if (!saved)
		return (NULL);
	if (!parse_line(&line, &saved))
		return (NULL);
	return (line);
}

char	*read_from_file(int fd, char *saved)
{
	char	*buff;
	char	*joined;
	ssize_t	r;

	buff = malloc((size_t)BUFFER_SIZE + 1);
	if (!buff)
		return (free(saved), NULL);
	r = 1;
	while (!ft_strchr(saved, '\n'))
	{
		r = read(fd, buff, BUFFER_SIZE);
		if (r == -1)
			return (free(buff), free(saved), NULL);
		if (r == 0)
			return (free(buff), saved);
		buff[r] = '\0';
		joined = ft_strjoin(saved, buff);
		if (!joined)
			return (free(buff), free(saved), NULL);
		free(saved);
		saved = joined;
	}
	free(buff);
	return (saved);
}

int	parse_line(char	**line, char **saved)
{
	*line = get_line(*saved);
	if (!*line)
		return (free(*saved), *saved = NULL, 0);
	if (!get_rest(saved))
		return (free(*line), free(*saved), *saved = NULL, 0);
	return (1);
}

char	*get_line(char *s)
{
	char	*line;
	ssize_t	i;

	i = 0;
	while (s[i])
		if (s[i++] == '\n')
			break ;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (s[i])
	{
		line[i] = s[i];
		if (s[i++] == '\n')
			break ;
	}
	line[i] = '\0';
	return (line);
}

int	get_rest(char **s)
{
	char	*rest;
	ssize_t	i;
	ssize_t	j;

	i = 0;
	while ((*s)[i])
		if ((*s)[i++] == '\n')
			break ;
	if (!(*s)[i])
		return (free(*s), *s = NULL, 1);
	j = 0;
	while ((*s)[i + j])
		j++;
	rest = malloc(j + 1);
	if (!rest)
		return (0);
	j = -1;
	while ((*s)[i + ++j])
		rest[j] = (*s)[i + j];
	rest[j] = '\0';
	free(*s);
	*s = rest;
	return (1);
}
