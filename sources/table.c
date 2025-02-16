/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 05:16:40 by mrouves           #+#    #+#             */
/*   Updated: 2025/02/16 18:53:42 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static bool	simulation_check(t_table *table, t_philo *philo)
{
	bool		res;

	pthread_mutex_lock(&philo->mut_lock);
	res = philo->state == EAT || timestamp(philo->time_lmeal) < table->delays.die;
	pthread_mutex_unlock(&philo->mut_lock);
	if (!res)
		philo_state_set(philo, DEAD, 0);
	return (res);
}

static bool	philo_init(t_table *table, t_philo *philo, uint32_t id)
{
	philo->id = id;
	philo->delays = table->delays;
	philo->mut_lfork = table->forks + id;
	philo->mut_rfork = table->forks + ((id + 1) % table->philo_nb);
	philo->mut_print = &table->mut_print;
	philo->mut_start = &table->mut_start;
	return (pthread_create(&philo->id_thread, NULL,
		(void *(*)(void *))__philo_thread, philo) == 0);
}

bool	table_init(t_table *table)
{
	uint32_t	i;

	i = -1;
	pthread_mutex_lock(&table->mut_start);
	while (++i < table->philo_nb)
		if (!philo_init(table, table->philos + i, i))
			break;
	usleep(10000);
	if (i != table->philo_nb)
		pthread_mutex_unlock(&table->mut_start);
	return (i == table->philo_nb);
}

void	table_destroy(t_table *table)
{
	uint32_t	i;

	i = -1;
	while (++i < table->philo_nb)
		philo_state_set(table->philos + i, DEAD, FQUIET);
	i = -1;
	while (++i < table->philo_nb)
		pthread_join(table->philos[i].id_thread, NULL);
	i = -1;
	while (++i < table->philo_nb)
	{
		pthread_mutex_destroy(&table->philos[i].mut_lock);
		pthread_mutex_destroy(table->forks + i);
	}
	pthread_mutex_destroy(&table->mut_start);
	pthread_mutex_destroy(&table->mut_print);
}

void	table_simulate(t_table *table)
{
	uint32_t	i;

	i = -1;
	gettimeofday(&table->start, NULL);
	while (++i < table->philo_nb)
	{
		pthread_mutex_lock(&table->philos[i].mut_lock);
		table->philos[i].time_lmeal = table->start;
		table->philos[i].time_start = table->start;
		pthread_mutex_unlock(&table->philos[i].mut_lock);
	}
	pthread_mutex_unlock(&table->mut_start);
	while (!table->stop)
	{
		i = -1;
		usleep(100);
		while (++i < table->philo_nb && !table->stop)
			table->stop = !simulation_check(table, table->philos + i);
	}
}
