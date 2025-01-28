/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:02:02 by mrouves           #+#    #+#             */
/*   Updated: 2025/01/28 20:16:17 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <philo.h>

bool	safe_atou(const char *s, uint32_t *out)
{
	uint64_t	res;

	if (__builtin_expect(!s || !out, 0))
		return (false);
	res = 0;
	while (*s == ' ' || *s == '\t' || *s == '\r')
		s++;
	s += (*s == '+');
	if (!(*s))
		return (false);
	while (*s)
	{
		res = (res << 1) + (res << 3) + (*s) - '0';
		if (*s < '0' || *s++ > '9' || res > UINT32_MAX)
			return (false);
	}
	*out = res;
	return (true);
}
