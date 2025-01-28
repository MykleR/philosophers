/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:44:28 by mrouves           #+#    #+#             */
/*   Updated: 2025/01/28 20:23:16 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>

typedef struct s_philo
{
	uint16_t	id;
	uint32_t	count_die;
	uint32_t	count_eat;
	uint32_t	count_sleep;
}	t_philo;

typedef struct s_manager
{
	uint32_t	delay_die;
	uint32_t	delay_eat;
	uint32_t	delay_sleep;
	uint16_t	nb_eats;
	uint16_t	nb_philo;
}	t_manager;

bool	safe_atou(const char *str, uint32_t *out);

#endif
