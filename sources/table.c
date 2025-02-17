/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 05:16:40 by mrouves           #+#    #+#             */
/*   Updated: 2025/02/17 22:35:24 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static void	simulation_check(t_table *table)
{
	bool		target;
	t_philo		*philo;
	uint32_t	i;

	i = -1;
	target = (table->nb_meals != 0);
	while (++i < table->nb_philo && !table->stop)
	{
		philo = table->philos + i;
		pthread_mutex_lock(&philo->mut_lock);
		table->stop = philo->state != EAT
			&& time_stamp(philo->timer_death) >= table->delays.die * 1000;
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
	return (pthread_create(&philo->id_thread, NULL,
			(void *(*)(void *))__philo_thread, philo) == 0);
}

static bool	table_init(t_table *table)
{
	bool		success;
	uint32_t	i;

	i = -1;
	pthread_mutex_lock(&table->mut_start);
	while (++i < table->nb_philo)
		if (!philo_init(table, table->philos + i, i))
			break ;
	success = (i == table->nb_philo);
	usleep(WAIT_INIT);
	gettimeofday(&table->start, NULL);
	i = -1;
	while (++i < table->nb_philo)
	{
		pthread_mutex_lock(&table->philos[i].mut_lock);
		table->philos[i].timer_start = table->start;
		table->philos[i].timer_death = table->start;
		pthread_mutex_unlock(&table->philos[i].mut_lock);
	}
	return (success);
}

static void	table_destroy(t_table *table)
{
	uint32_t	i;

	i = -1;
	while (++i < table->nb_philo)
		philo_state_set(table->philos + i, DEAD, FQUIET);
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
		usleep(WAIT_UPDATE);
		simulation_check(table);
	}
	table_destroy(table);
}
