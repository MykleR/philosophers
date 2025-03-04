/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:02:02 by mrouves           #+#    #+#             */
/*   Updated: 2025/03/04 19:29:02 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

uint64_t	micro_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000000L + time.tv_usec);
}

void	state_print(t_philo *philo)
{
	static char	*msgs = MSG_THINK MSG_LFORK MSG_RFORK \
		MSG_EAT MSG_SLEEP MSG_DEAD;
	char		*msg;
	uint64_t	ms;

	msg = msgs + philo->state;
	pthread_mutex_lock(philo->mut_print);
	ms = (micro_time() - philo->time_start) / 1000;
	if (*msg)
		printf("%lu %u %s\n", ms, philo->id + 1, msg);
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
