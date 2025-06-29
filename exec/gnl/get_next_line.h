/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 20:58:08 by oelhasso          #+#    #+#             */
/*   Updated: 2025/06/29 18:16:00 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
size_t	gnl_strlen(char *str);
char	*ft_strdup_add(char *src, int add_bytes);
void	ft_strlcpy(char *dest, char *src, int add_bytes);
void	free_str(char **str);
int		ft_strchr(char *str, char c);
char	*rtn_line(char *buffer, int p);

#endif
