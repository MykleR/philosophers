/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:44:28 by mrouves           #+#    #+#             */
/*   Updated: 2025/03/04 19:31:50 by mrouves          ###   ########.fr       */
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
# define WAIT_INIT		10000
# define WAIT_SIMUL		1000
# define WAIT_PHILO		10
# define WAIT_SLEEP		1

# define MSG_THINK		"is thinking\0"
# define MSG_LFORK		"has taken a fork\0"
# define MSG_RFORK		"has taken a fork\0"
# define MSG_EAT		"is eating\0"
# define MSG_SLEEP		"is sleeping\0"
# define MSG_DEAD		"died\0"

typedef struct s_table	t_table;

typedef enum e_state
{
	THINK	= 0,
	LFORK	= THINK + sizeof(MSG_THINK) - 1,
	RFORK	= LFORK + sizeof(MSG_LFORK) - 1,
	EAT		= RFORK + sizeof(MSG_RFORK) - 1,
	SLEEP	= EAT + sizeof(MSG_EAT) - 1,
	DEAD	= SLEEP + sizeof(MSG_SLEEP) - 1,
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
	uint64_t		time_start;
	uint64_t		time_lmeal;
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
	bool			stop;
}	t_table;

t_state		philo_state_get(t_philo *philo);
void		philo_state_set(t_philo *philo, t_state s, bool quiet);
void		*__philo_thread(t_philo *philo);

void		table_simulate(t_table *table);

uint64_t	micro_time(void);
void		state_print(t_philo *philo);

#endif
