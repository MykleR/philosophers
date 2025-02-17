/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:02:02 by mrouves           #+#    #+#             */
/*   Updated: 2025/02/17 23:03:27 by mrouves          ###   ########.fr       */
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

void	state_print(t_philo *philo, uint8_t flags)
{
	char		*msg;
	uint64_t	ms;

	ms = time_stamp(philo->timer_start) / 1000 * !(flags & FSTART);
	msg = (char *)STATE_MSGS + philo->state;
	pthread_mutex_lock(philo->mut_print);
	if (*msg)
		printf("%lu %d %s", ms, philo->id, msg);
	pthread_mutex_unlock(philo->mut_print);
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
