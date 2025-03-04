/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 05:16:40 by mrouves           #+#    #+#             */
/*   Updated: 2025/03/04 19:29:54 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static void	simulation_check(t_table *table)
{
	bool		target;
	t_philo		*philo;
	uint64_t	death;
	uint32_t	i;

	i = -1;
	target = (table->nb_meals != 0);
	while (++i < table->nb_philo && !table->stop)
	{
		philo = table->philos + i;
		pthread_mutex_lock(&philo->mut_lock);
		death = micro_time() - philo->time_lmeal;
		table->stop = philo->state != EAT && death >= table->delays.die * 1000;
		target &= (philo->nb_meals >= table->nb_meals);
		pthread_mutex_unlock(&philo->mut_lock);
		if (table->stop)
			philo_state_set(philo, DEAD, 0);
	}
	table->stop |= target;
}

static bool	philo_init(t_table *table, t_philo *philo, uint32_t id)
{
	bool	is_last;

	is_last = (id % 2 == 1);
	philo->id = id;
	philo->delays = table->delays;
	philo->mut_lfork = table->forks + ((id + is_last) % table->nb_philo);
	philo->mut_rfork = table->forks + ((id + !is_last) % table->nb_philo);
	philo->mut_print = &table->mut_print;
	philo->mut_start = &table->mut_start;
	philo->time_start = micro_time();
	philo->time_lmeal = philo->time_start;
	return (pthread_create(&philo->id_thread, NULL,
			(void *(*)(void *))__philo_thread, philo) == 0);
}

static bool	table_init(t_table *table)
{
	uint64_t	time;
	uint32_t	i;
	uint32_t	j;

	i = -1;
	pthread_mutex_lock(&table->mut_start);
	while (++i < table->nb_philo)
		if (!philo_init(table, table->philos + i, i))
			break ;
	usleep(WAIT_INIT);
	time = micro_time();
	j = -1;
	while (++j <= i)
	{
		pthread_mutex_lock(&table->philos[j].mut_lock);
		table->philos[j].time_start = time;
		table->philos[j].time_lmeal = time;
		pthread_mutex_unlock(&table->philos[j].mut_lock);
	}
	return (i == table->nb_philo);
}

static void	table_destroy(t_table *table)
{
	uint32_t	i;

	i = -1;
	while (++i < table->nb_philo)
		philo_state_set(table->philos + i, DEAD, true);
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
	pthread_mutex_destroy(&table->mut_start);
	pthread_mutex_destroy(&table->mut_print);
}

void	table_simulate(t_table *table)
{
	table->stop = !table_init(table);
	pthread_mutex_unlock(&table->mut_start);
	while (!table->stop)
	{
		simulation_check(table);
		usleep(WAIT_SIMUL);
	}
	table_destroy(table);
}
