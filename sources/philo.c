/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 05:16:28 by mrouves           #+#    #+#             */
/*   Updated: 2025/03/23 22:54:01 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static void	philo_uwait(t_philo *philo, uint64_t microsecs)
{
	uint64_t	start;

	start = micro_time();
	while (!get_value(philo->mut_global, philo->stop)
		&& micro_time() - start < microsecs)
		usleep(WAIT_SLEEP);
}

static void	change_state(t_philo *philo, t_state state)
{
	if (!get_value(philo->mut_global, philo->stop))
		state_print(state, philo->mut_print, philo->id, philo->time_start);
}

static void	philo_simulate(t_philo *philo)
{
	pthread_mutex_lock(philo->mut_lfork);
	change_state(philo, LFORK);
	pthread_mutex_lock(philo->mut_rfork);
	change_state(philo, RFORK);
	change_state(philo, EAT);
	pthread_mutex_lock(&philo->mut_lock);
	philo->nb_meals++;
	philo->time_lmeal = micro_time();
	pthread_mutex_unlock(&philo->mut_lock);
	philo_uwait(philo, philo->delays.eat * 1000);
	pthread_mutex_lock(&philo->mut_lock);
	philo->time_lmeal = micro_time();
	pthread_mutex_unlock(&philo->mut_lock);
	change_state(philo, SLEEP);
	pthread_mutex_unlock(philo->mut_rfork);
	pthread_mutex_unlock(philo->mut_lfork);
	philo_uwait(philo, philo->delays.sleep * 1000);
	change_state(philo, THINK);
}

void	*__philo_thread(t_philo *philo)
{
	pthread_mutex_lock(philo->mut_global);
	pthread_mutex_unlock(philo->mut_global);
	if (philo->mut_lfork == philo->mut_rfork)
		state_print(LFORK, philo->mut_print, philo->id, philo->time_start);
	while (philo->mut_lfork != philo->mut_rfork
		&& !get_value(philo->mut_global, philo->stop))
	{
		philo_simulate(philo);
		philo_uwait(philo, WAIT_PHILO);
	}
	return (NULL);
}
