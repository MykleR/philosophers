/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:44:13 by mrouves           #+#    #+#             */
/*   Updated: 2025/01/31 02:30:03 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static t_state	philo_state_access(t_philo *philo)
{
	t_state val;

	pthread_mutex_lock(philo->state_check);
	val = philo->state;
	pthread_mutex_unlock(philo->state_check);
	return (val);
}

static void philo_state_set(t_philo *philo, t_state s, bool log)
{
	pthread_mutex_lock(philo->state_check);
	if (philo->state != DEAD && philo->state != s)
	{
		philo->state = s;
		if (log)
		{
			pthread_mutex_lock(philo->print_check);
			printf("%d %s", philo->id, (char *)STATE_MSGS + philo->state);
			pthread_mutex_unlock(philo->print_check);
		}
	}
	pthread_mutex_unlock(philo->state_check);
}

static  void philo_think_eat(t_philo *philo)
{
	if (philo_state_access(philo) == DEAD)
		return ;
	pthread_mutex_lock(philo->fork_l);
	pthread_mutex_lock(philo->fork_r);
	philo_state_set(philo, EAT, true);
	usleep(philo->delays.eat * 1000);
	philo_state_set(philo, SLEEP, true);
	pthread_mutex_unlock(philo->fork_l);
	pthread_mutex_unlock(philo->fork_r);
}

static	void philo_sleep(t_philo *philo)
{
	if (philo_state_access(philo) == DEAD)
		return ;
	usleep(philo->delays.sleep * 1000);
	philo_state_set(philo, THINK, true);
}

static void	*__philo_thread(t_philo *philo)
{
	pthread_mutex_lock(philo->print_check);
	printf("%d %s", philo->id, (char *)STATE_MSGS + philo->state);
	pthread_mutex_unlock(philo->print_check);
	pthread_mutex_lock(philo->start_check);
	pthread_mutex_unlock(philo->start_check);
	while (philo_state_access(philo) != DEAD)
	{
		if (philo_state_access(philo) == SLEEP)
			philo_sleep(philo);
		if (philo_state_access(philo) == THINK)
			philo_think_eat(philo);
	}
	return (NULL);
}

static bool	table_init(t_table *table)
{
	uint32_t	i;
	
	i = -1;
	pthread_mutex_lock(&table->start_check);
	while (++i < table->philo_nb)
	{
		table->philos[i].id = i;
		table->philos[i].delays = table->delays;
		table->philos[i].fork_l = table->forks + i;
		table->philos[i].fork_r = table->forks + ((i + 1) % table->philo_nb);
		table->philos[i].state_check = table->states + i;
		table->philos[i].print_check = &table->print_check;
		table->philos[i].start_check = &table->start_check;
		if (pthread_create(&(table->philos[i].id_thread), NULL,
				 (void *(*)(void *))__philo_thread, table->philos + i) < 0)
			break;
	}
	usleep(1000);
	pthread_mutex_unlock(&table->start_check);
	return (i >= table->philo_nb);
}

static void	table_destroy(t_table *table)
{
	uint32_t	i;

	i = -1;
	while (++i < table->philo_nb)
		philo_state_set(table->philos + i, DEAD, false);
	i = -1;
	while (++i < table->philo_nb)
		pthread_join(table->philos[i].id_thread, NULL);
	pthread_mutex_destroy(&table->start_check);
	pthread_mutex_destroy(&table->print_check);
	i = -1;
	while (++i < table->philo_nb)
	{
		pthread_mutex_destroy(table->forks + i);
		pthread_mutex_destroy(table->states + i);
	}
}

static bool table_check_death(t_table *table, t_philo *philo)
{
	if (philo_state_access(philo) == EAT)
		philo->counts.die = 0;
	philo->counts.die += philo_state_access(philo) == THINK
		|| philo_state_access(philo) == SLEEP;
	if (philo->counts.die >= table->delays.die)
		philo_state_set(philo, DEAD, true);
	return (philo->counts.die >= table->delays.die);
}

static void	table_simulate(t_table *table)
{
	uint32_t	i;

	while (!table->philo_died)
	{
		i = -1;
		usleep(1000);
		while (++i < table->philo_nb && !table->philo_died)
			table->philo_died = table_check_death(table, table->philos + i);
	}
}

int	main(int ac, char **av)
{
	static t_table	table = {0};

	if ((ac != 6 && ac != 5)
		|| !safe_atou(av[1], &(table.philo_nb))
		|| !safe_atou(av[2], &(table.delays.die))
		|| !safe_atou(av[3], &(table.delays.eat))
		|| !safe_atou(av[4], &(table.delays.sleep))
		|| (ac == 6 && !safe_atou(av[5], &table.delays.eaten)))
		return (1);
	table_init(&table);
	table_simulate(&table);
	table_destroy(&table);
}
