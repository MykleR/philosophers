/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 05:16:28 by mrouves           #+#    #+#             */
/*   Updated: 2025/02/16 19:07:16 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

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
		state_print(philo, flags & (~FSLOCK));
	}
	pthread_mutex_unlock(&philo->mut_lock);
}

static void philo_eat(t_philo *philo)
{
	if (philo_state_get(philo) != EAT)
		return ;
	usleep(philo->delays.eat * 1000);
	pthread_mutex_lock(&philo->mut_lock);
	gettimeofday(&philo->time_lmeal, NULL);
	pthread_mutex_unlock(&philo->mut_lock);
	philo_state_set(philo, SLEEP, 0);
}


static void	philo_think(t_philo *philo)
{
	if (philo->mut_lfork == philo->mut_rfork
		|| philo_state_get(philo) != THINK)
		return ;
	pthread_mutex_lock(philo->mut_lfork);
	philo_state_set(philo, LFORK, 0);
	pthread_mutex_lock(philo->mut_rfork);
	philo_state_set(philo, RFORK, 0);
	philo_state_set(philo, EAT, 0);
	philo_eat(philo);
	pthread_mutex_unlock(philo->mut_lfork);
	pthread_mutex_unlock(philo->mut_rfork);
}

static void philo_sleep(t_philo *philo)
{
	if (philo_state_get(philo) != SLEEP)
		return ;
	usleep(philo->delays.sleep * 1000);
	philo_state_set(philo, THINK, 0);
}

void	*__philo_thread(t_philo *philo)
{
	state_print(philo, FWAIT | FSLOCK);
	pthread_mutex_lock(philo->mut_start);
	pthread_mutex_unlock(philo->mut_start);
	while (philo_state_get(philo) != DEAD)
	{
		philo_think(philo);
		philo_eat(philo);
		philo_sleep(philo);
		usleep(100);
	}
	return (NULL);
}
