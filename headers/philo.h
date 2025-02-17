/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:44:28 by mrouves           #+#    #+#             */
/*   Updated: 2025/02/17 22:10:29 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdint.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <inttypes.h>
# include <sys/time.h>

# define MAX_PHILO		200
# define WAIT_UPDATE	1
# define WAIT_SLEEP		10
# define WAIT_INIT		10000

# define FQUIET		0b001

# define STATE_MSGS "\
is thinking\n\0\
has taken a fork\n\0\
has taken a fork\n\0\
is eating\n\0\
is sleeping\n\0\
died\n\0"

typedef struct timeval	t_timeval;
typedef struct s_table	t_table;

typedef enum e_state
{
	THINK = 0,
	LFORK = 78,
	RFORK = 78,
	EAT = 49,
	SLEEP = 60,
	DEAD = 73,
	EXIT = 78,
}	t_state;

typedef struct s_delays
{
	uint32_t		die;
	uint32_t		eat;
	uint32_t		sleep;
}	t_delays;

typedef struct s_philo
{
	pthread_mutex_t	*mut_lfork;
	pthread_mutex_t	*mut_rfork;
	pthread_mutex_t	*mut_print;
	pthread_mutex_t	*mut_start;
	pthread_mutex_t	mut_lock;
	t_timeval		timer_start;
	t_timeval		timer_death;
	t_delays		delays;
	uint32_t		nb_meals;
	uint32_t		id;
	pthread_t		id_thread;
	t_state			state;
}	t_philo;

typedef struct s_table
{
	pthread_mutex_t	mut_start;
	pthread_mutex_t	mut_print;
	pthread_mutex_t	forks[MAX_PHILO];
	t_philo			philos[MAX_PHILO];
	uint32_t		nb_philo;
	uint32_t		nb_meals;
	t_delays		delays;
	t_timeval		start;
	bool			stop;
}	t_table;

t_state		philo_state_get(t_philo *philo);
void		philo_wait(t_philo *philo, uint64_t ms);
void		philo_state_set(t_philo *philo, t_state s, uint8_t flags);
void		*__philo_thread(t_philo *philo);

void		table_simulate(t_table *table);

uint64_t	time_stamp(t_timeval start);
bool		safe_atou(const char *str, uint32_t *out);
void		state_print(pthread_mutex_t *lock, t_state state,
				uint32_t id, t_timeval *time);

#endif
