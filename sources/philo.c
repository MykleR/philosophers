/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 05:16:28 by mrouves           #+#    #+#             */
/*   Updated: 2025/03/04 19:26:46 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static void	philo_uwait(t_philo *philo, uint64_t microsecs)
{
	uint64_t	start;

	start = micro_time();
	while (philo_state_get(philo) != DEAD && micro_time() - start < microsecs)
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

void	philo_state_set(t_philo *philo, t_state state, bool quiet)
{
	pthread_mutex_lock(&philo->mut_lock);
	if (philo->state != DEAD && philo->state != state)
	{
		philo->state = state;
		if (!quiet)
			state_print(philo);
	}
	pthread_mutex_unlock(&philo->mut_lock);
}

static void	philo_simulate(t_philo *philo)
{
	pthread_mutex_lock(philo->mut_lfork);
	philo_state_set(philo, LFORK, false);
	pthread_mutex_lock(philo->mut_rfork);
	philo_state_set(philo, RFORK, false);
	philo_state_set(philo, EAT, false);
	philo_uwait(philo, philo->delays.eat * 1000);
	pthread_mutex_lock(&philo->mut_lock);
	philo->nb_meals++;
	philo->time_lmeal = micro_time();
	pthread_mutex_unlock(&philo->mut_lock);
	philo_state_set(philo, SLEEP, false);
	pthread_mutex_unlock(philo->mut_rfork);
	pthread_mutex_unlock(philo->mut_lfork);
	philo_uwait(philo, philo->delays.sleep * 1000);
	philo_state_set(philo, THINK, false);
}

void	*__philo_thread(t_philo *philo)
{
	pthread_mutex_lock(philo->mut_start);
	pthread_mutex_unlock(philo->mut_start);
	while (philo_state_get(philo) != DEAD)
	{
		if (philo->mut_lfork != philo->mut_rfork)
			philo_simulate(philo);
		else
			philo_state_set(philo, LFORK, false);
		philo_uwait(philo, WAIT_PHILO);
	}
	return (NULL);
}
