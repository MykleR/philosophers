/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 05:16:28 by mrouves           #+#    #+#             */
/*   Updated: 2025/02/17 18:09:27 by mrouves          ###   ########.fr       */
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

static void	philo_eat(t_philo *philo)
{
	if (philo_state_get(philo) != EAT)
		return ;
	ft_sleep(philo, philo->delays.eat);
	pthread_mutex_lock(&philo->mut_lock);
	philo->nb_meals++;
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
	pthread_mutex_unlock(philo->mut_rfork);
	pthread_mutex_unlock(philo->mut_lfork);
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
		if (philo_state_get(philo) == SLEEP)
			ft_sleep(philo, philo->delays.sleep);
		philo_state_set(philo, THINK, 0);
		usleep(WAIT_UPDATE);
	}
	return (NULL);
}
