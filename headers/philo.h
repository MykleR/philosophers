/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:44:28 by mrouves           #+#    #+#             */
/*   Updated: 2025/03/23 23:00:23 by mrouves          ###   ########.fr       */
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
# define WAIT_SIMUL		100
# define WAIT_PHILO		100
# define WAIT_SLEEP		10

# define MSG_THINK	"[%lu] %u: 🤔 is thinking\n\0"
# define MSG_LFORK	"[%lu] %u: 🍴 left fork\n\0"
# define MSG_RFORK	"[%lu] %u: 🍴 right fork\n\0"
# define MSG_EAT	"[%lu] %u: 🍔 is eating\n\0"
# define MSG_SLEEP	"[%lu] %u: 😴 is sleeping \n\0"
# define MSG_DEAD	"[%lu] %u: 💀 died\n\0"

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
	bool			*stop;
	pthread_mutex_t	*mut_lfork;
	pthread_mutex_t	*mut_rfork;
	pthread_mutex_t	*mut_print;
	pthread_mutex_t	*mut_global;
	pthread_mutex_t	mut_lock;
	uint64_t		time_start;
	uint64_t		time_lmeal;
	t_delays		delays;
	uint32_t		nb_meals;
	uint32_t		id;
	pthread_t		id_thread;
}	t_philo;

typedef struct s_table
{
	pthread_mutex_t	mut_global;
	pthread_mutex_t	mut_print;
	pthread_mutex_t	forks[MAX_PHILO];
	t_philo			philos[MAX_PHILO];
	uint64_t		time_start;
	uint32_t		nb_philo;
	uint32_t		nb_meals;
	t_delays		delays;
	bool			stop;
}	t_table;

bool		get_value(pthread_mutex_t *mut, bool *val);
void		*__philo_thread(t_philo *philo);
void		table_simulate(t_table *table);

uint64_t	micro_time(void);
void		state_print(t_state state, pthread_mutex_t *mut_print,
				uint32_t id, uint64_t time_start);

#endif
