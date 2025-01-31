/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:44:28 by mrouves           #+#    #+#             */
/*   Updated: 2025/01/31 01:07:46 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdint.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>

# define MAX_PHILO 200

# define STATE_MSGS "\
is thinking\n\0\
is eating\n\0\
is sleeping\n\0\
died\n\0"

typedef enum e_state
{
	THINK = 0,
	EAT = 13,
	SLEEP = 24,
	DEAD = 37,
	EXIT = 42,
}	t_state;

typedef struct s_delays
{
	uint32_t		die;
	uint32_t		eat;
	uint32_t		sleep;
	uint32_t		eaten;
}	t_delays;

typedef struct s_philo
{
	pthread_mutex_t	*fork_l;
	pthread_mutex_t	*fork_r;
	pthread_mutex_t *print_check;
	pthread_mutex_t	*start_check;
	pthread_mutex_t *state_check;
	t_delays		counts;
	t_delays		delays;
	uint32_t		id;
	pthread_t		id_thread;
	t_state			state;
}	t_philo;

typedef struct s_table
{
	pthread_mutex_t	start_check;
	pthread_mutex_t	print_check;
	pthread_mutex_t	forks[MAX_PHILO];
	pthread_mutex_t	states[MAX_PHILO];
	t_philo			philos[MAX_PHILO];
	t_delays		delays;
	uint32_t		philo_nb;
	bool			philo_died;
}	t_table;

bool	safe_atou(const char *str, uint32_t *out);

#endif
