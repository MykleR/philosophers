/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 05:16:40 by mrouves           #+#    #+#             */
/*   Updated: 2025/03/23 23:04:22 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static bool	table_monitor(t_table *table)
{
	bool		death;
	uint32_t	completed;
	t_philo		*philo;
	uint32_t	i;

	i = -1;
	death = 0;
	completed = 0;
	while (++i < table->nb_philo && !death)
	{
		philo = table->philos + i;
		pthread_mutex_lock(&philo->mut_lock);
		death = micro_time() - philo->time_lmeal >= table->delays.die * 1000;
		completed += (philo->nb_meals >= table->nb_meals);
		pthread_mutex_unlock(&philo->mut_lock);
		if (death)
			state_print(DEAD, &table->mut_print, i, table->time_start);
	}
	return (death || (table->nb_meals && completed >= table->nb_philo));
}

static bool	philo_init(t_table *table, t_philo *philo, uint32_t id)
{
	bool	is_last;

	is_last = (id % 2 == 1);
	philo->id = id;
	philo->stop = &table->stop;
	philo->delays = table->delays;
	philo->mut_lfork = table->forks + ((id + is_last) % table->nb_philo);
	philo->mut_rfork = table->forks + ((id + !is_last) % table->nb_philo);
	philo->mut_print = &table->mut_print;
	philo->mut_global = &table->mut_global;
	philo->time_start = micro_time();
	philo->time_lmeal = philo->time_start;
	return (pthread_create(&philo->id_thread, NULL,
			(void *(*)(void *))__philo_thread, philo) == 0);
}

static bool	table_init(t_table *table)
{
	uint32_t	i;
	uint32_t	j;

	i = -1;
	pthread_mutex_lock(&table->mut_global);
	while (++i < table->nb_philo)
		if (!philo_init(table, table->philos + i, i))
			break ;
	usleep(WAIT_INIT);
	table->time_start = micro_time();
	j = -1;
	while (++j <= i)
	{
		pthread_mutex_lock(&table->philos[j].mut_lock);
		table->philos[j].time_start = table->time_start;
		table->philos[j].time_lmeal = table->time_start;
		pthread_mutex_unlock(&table->philos[j].mut_lock);
	}
	return (i == table->nb_philo);
}

static void	table_destroy(t_table *table)
{
	uint32_t	i;

	i = -1;
	while (++i < table->nb_philo)
		if (table->philos[i].id_thread)
			pthread_join(table->philos[i].id_thread, NULL);
	i = -1;
	while (++i < table->nb_philo)
	{
		pthread_mutex_destroy(&table->philos[i].mut_lock);
		pthread_mutex_destroy(table->forks + i);
	}
	pthread_mutex_destroy(&table->mut_global);
	pthread_mutex_destroy(&table->mut_print);
}

void	table_simulate(t_table *table)
{
	table->stop = !table_init(table);
	pthread_mutex_unlock(&table->mut_global);
	while (!get_value(&table->mut_global, &table->stop))
	{
		pthread_mutex_lock(&table->mut_global);
		table->stop = table_monitor(table);
		pthread_mutex_unlock(&table->mut_global);
		usleep(WAIT_SIMUL);
	}
	table_destroy(table);
}
