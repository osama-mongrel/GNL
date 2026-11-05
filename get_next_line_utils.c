/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-el <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 16:41:51 by oben-el           #+#    #+#             */
/*   Updated: 2026/11/05 16:54:37 by oben-el          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strchr(char *s, char c)
{
	if (!s)
		return (0);
	while (*s)
		if (*s++ == c)
			return (1);
	return (0);
}

size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin(char *stash, char *buffer)
{
	char	*new;
	size_t	i;
	size_t	j;
	
	if (!stash)
	{
		stash = malloc(1);
		stash[0] = '\0';
	}
	new = malloc(ft_strlen(stash) + ft_strlen(buffer) + 1);
	if (!new)
		return (NULL);
	i = -1;
	while (stash[++i])
		new[i] = stash[i];
	j = 0;
	while (buffer[j])
		new[i++] = buffer[j++];
	new[i] = '\0';
	free(stash);
	return (new);
}
