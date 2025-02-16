/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:44:13 by mrouves           #+#    #+#             */
/*   Updated: 2025/02/16 19:27:22 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	main(int ac, char **av)
{
	static t_table	table = {0};

	if ((ac != 6 && ac != 5)
		|| !safe_atou(av[1], &(table.nb_philo)) || !table.nb_philo || table.nb_philo > MAX_PHILO
		|| !safe_atou(av[2], &(table.delays.die)) || table.delays.die < 60
		|| !safe_atou(av[3], &(table.delays.eat)) || table.delays.eat < 60
		|| !safe_atou(av[4], &(table.delays.sleep)) || table.delays.sleep < 60
		|| (ac == 6 && !safe_atou(av[5], &table.nb_meals)))
		return (1);
	if (table_init(&table))
		table_simulate(&table);
	table_destroy(&table);
}
