/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:44:28 by mrouves           #+#    #+#             */
/*   Updated: 2025/02/16 18:29:36 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdint.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
#include <inttypes.h>
#include <sys/time.h>

# define MAX_PHILO	200

# define FWAIT	0b010
# define FQUIET	0b001
# define FSLOCK 0b100

# define STATE_MSGS "\
is thinking\n\0\
has taken his left fork\n\0\
has taken his right fork\n\0\
is eating\n\0\
is sleeping\n\0\
died\n\0"

typedef struct timeval	t_timeval;
typedef struct s_table	t_table;

typedef enum e_state
{
	THINK = 0,
	LFORK = 13,
	RFORK = 38,
	EAT = 64,
	SLEEP = 75,
	DEAD = 88,
	EXIT = 93,
}	t_state;

typedef struct s_delays
{
	uint32_t		die;
	uint32_t		eat;
	uint32_t		sleep;
	uint32_t		meals;
}	t_delays;

typedef struct s_philo
{
	pthread_mutex_t	*mut_lfork;
	pthread_mutex_t	*mut_rfork;
	pthread_mutex_t	*mut_print;
	pthread_mutex_t	*mut_start;
	pthread_mutex_t	mut_lock;
	t_delays		delays;
	t_timeval		time_start;
	t_timeval		time_lmeal;
	pthread_t		id_thread;
	uint32_t		id;
	t_state			state;
}	t_philo;

typedef struct s_table
{
	pthread_mutex_t	mut_start;
	pthread_mutex_t	mut_print;
	pthread_mutex_t	forks[MAX_PHILO];
	t_philo			philos[MAX_PHILO];
	uint32_t		philo_nb;
	t_timeval		start;
	t_delays		delays;
	bool			stop;
}	t_table;

t_state		philo_state_get(t_philo *philo);
void		philo_state_set(t_philo *philo, t_state s, uint8_t flags);
void		*__philo_thread(t_philo *philo);
void		philo_wait(t_philo *philo, uint64_t ms);

bool		table_init(t_table *table);
void		table_destroy(t_table *table);
void		table_simulate(t_table *table);
void		table_eat(t_table *table);

uint64_t	timestamp(t_timeval start);
bool		safe_atou(const char *str, uint32_t *out);
void		state_print(t_philo *philo, uint8_t flags);

#endif
