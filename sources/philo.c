/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 05:16:28 by mrouves           #+#    #+#             */
/*   Updated: 2025/02/17 22:10:54 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	philo_wait(t_philo *philo, uint64_t microsecs)
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
	if (philo->state != DEAD && philo->state != state)
	{
		philo->state = state;
		if (!(flags & FQUIET))
			state_print(philo->mut_print, philo->state, philo->id, &philo->timer_start);
	}
	pthread_mutex_unlock(&philo->mut_lock);
}

static void	philo_life(t_philo *philo)
{
	pthread_mutex_lock(philo->mut_lfork);
	philo_state_set(philo, LFORK, 0);
	pthread_mutex_lock(philo->mut_rfork);
	philo_state_set(philo, RFORK, 0);
	philo_state_set(philo, EAT, 0);
	philo_wait(philo, philo->delays.eat * 1000);
	pthread_mutex_lock(&philo->mut_lock);
	philo->nb_meals++;
	gettimeofday(&philo->timer_death, NULL);
	pthread_mutex_unlock(&philo->mut_lock);
	philo_state_set(philo, SLEEP, 0);
	pthread_mutex_unlock(philo->mut_rfork);
	pthread_mutex_unlock(philo->mut_lfork);
	philo_wait(philo, philo->delays.sleep * 1000);
	philo_state_set(philo, THINK, 0);
}

void	*__philo_thread(t_philo *philo)
{
	pthread_mutex_lock(&philo->mut_lock);
	state_print(philo->mut_print, philo->state, philo->id, 0);
	pthread_mutex_unlock(&philo->mut_lock);
	pthread_mutex_lock(philo->mut_start);
	pthread_mutex_unlock(philo->mut_start);
	while (philo_state_get(philo) != DEAD)
	{
		if (philo->mut_lfork == philo->mut_rfork)
			continue ;
		philo_life(philo);
		usleep(WAIT_UPDATE);
	}
	return (NULL);
}
