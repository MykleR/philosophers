/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 05:16:28 by mrouves           #+#    #+#             */
/*   Updated: 2025/02/17 23:10:32 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static void	philo_uwait(t_philo *philo, uint64_t microsecs)
{
	t_timeval	start;

	gettimeofday(&start, NULL);
	while (philo_state_get(philo) != DEAD && time_stamp(start) < microsecs)
		usleep(WAIT_SLEEP);
}

t_state	philo_state_get(t_philo *philo)
{
	t_state	state;

	pthread_mutex_lock(&philo->mut_lock);
	state = philo->state;
	pthread_mutex_unlock(&philo->mut_lock);
	return (state);
}

void	philo_state_set(t_philo *philo, t_state state, uint8_t flags)
{
	pthread_mutex_lock(&philo->mut_lock);
	if (philo->state != DEAD && ((flags & FFORCE) || philo->state != state))
	{
		philo->state = state;
		if (!(flags & FQUIET))
			state_print(philo, flags);
	}
	pthread_mutex_unlock(&philo->mut_lock);
}

static void	philo_simulate(t_philo *philo)
{
	pthread_mutex_lock(philo->mut_lfork);
	philo_state_set(philo, LFORK, 0);
	pthread_mutex_lock(philo->mut_rfork);
	philo_state_set(philo, RFORK, 0);
	philo_state_set(philo, EAT, 0);
	philo_uwait(philo, philo->delays.eat * 1000);
	pthread_mutex_lock(&philo->mut_lock);
	philo->nb_meals++;
	gettimeofday(&philo->timer_death, NULL);
	pthread_mutex_unlock(&philo->mut_lock);
	philo_state_set(philo, SLEEP, 0);
	pthread_mutex_unlock(philo->mut_rfork);
	pthread_mutex_unlock(philo->mut_lfork);
	philo_uwait(philo, philo->delays.sleep * 1000);
	philo_state_set(philo, THINK, 0);
}

void	*__philo_thread(t_philo *philo)
{
	philo_state_set(philo, THINK, FFORCE | FSTART);
	pthread_mutex_lock(philo->mut_start);
	pthread_mutex_unlock(philo->mut_start);
	while (philo_state_get(philo) != DEAD)
	{
		if (philo->mut_lfork != philo->mut_rfork)
			philo_simulate(philo);
		usleep(WAIT_UPDATE);
	}
	return (NULL);
}
