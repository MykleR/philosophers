/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:02:02 by mrouves           #+#    #+#             */
/*   Updated: 2025/02/16 23:44:33 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <philo.h>

uint64_t	timestamp(t_timeval start)
{
	t_timeval	now;
    uint64_t	ms;
	
	gettimeofday(&now, NULL);
	ms = (now.tv_sec - start.tv_sec) * 1000
		+ (now.tv_usec - start.tv_usec) / 1000;
	return (ms);
}

void	state_print(t_philo *philo, uint8_t flags)
{
	uint64_t	time;
	t_state		state;

	if (flags & FQUIET)
		return ;
	if (flags & FSLOCK)
		pthread_mutex_lock(&philo->mut_lock);
	state = philo->state;
	time = timestamp(philo->time_start) * !(flags & FWAIT);
	if (flags & FSLOCK)
		pthread_mutex_unlock(&philo->mut_lock);
	pthread_mutex_lock(philo->mut_print);
	printf("[%010lu] %d %s", time, philo->id, (char *)STATE_MSGS + state);
	pthread_mutex_unlock(philo->mut_print);
}

void ft_sleep(t_philo *philo, uint64_t ms)
{
	t_timeval	start;
	
	gettimeofday(&start, NULL);
	while (philo_state_get(philo) != DEAD && timestamp(start) < ms)
		usleep(1000);
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
