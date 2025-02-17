/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:02:02 by mrouves           #+#    #+#             */
/*   Updated: 2025/02/17 22:14:00 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

uint64_t	time_stamp(t_timeval start)
{
	t_timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - start.tv_sec) * 1000000
		+ (now.tv_usec - start.tv_usec));
}

void	state_print(pthread_mutex_t *lock, t_state state, uint32_t id, t_timeval *time)
{
	uint64_t	ms;

	ms = 0;
	if (time)
		ms = time_stamp(*time) / 1000;
	pthread_mutex_lock(lock);
	if (*((char *)STATE_MSGS + state))
		printf("%lu %d %s", ms, id, (char *)STATE_MSGS + state);
	pthread_mutex_unlock(lock);
}

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
