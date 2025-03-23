/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrouves <mrouves@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:44:13 by mrouves           #+#    #+#             */
/*   Updated: 2025/03/23 22:58:03 by mrouves          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

uint64_t	micro_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000000L + time.tv_usec);
}

void	state_print(t_state state, pthread_mutex_t *mut_print,
				uint32_t id, uint64_t time_start)
{
	const char	*msgs = MSG_THINK MSG_LFORK MSG_RFORK \
		MSG_EAT MSG_SLEEP MSG_DEAD;
	uint64_t	ms;

	pthread_mutex_lock(mut_print);
	ms = (micro_time() - time_start) / 1000;
	printf(msgs + state, ms, id + 1);
	pthread_mutex_unlock(mut_print);
}

bool	safe_atou(const char *s, uint32_t *out)
{
	uint64_t	res;

	if (__builtin_expect(!s || !out, 0))
		return (false);
	res = 0;
	while (*s == ' ' || *s == '\t' || *s == '\r')
		s++;
	s += (*s == '+');
	if (!(*s))
		return (false);
	while (*s)
	{
		res = (res << 1) + (res << 3) + (*s) - '0';
		if (*s < '0' || *s++ > '9' || res > UINT32_MAX)
			return (false);
	}
	*out = res;
	return (true);
}

bool	get_value(pthread_mutex_t *mut, bool *val)
{
	bool	ret;

	pthread_mutex_lock(mut);
	ret = *val;
	pthread_mutex_unlock(mut);
	return (ret);
}

int	main(int ac, char **av)
{
	static t_table	table = {0};

	if ((ac != 6 && ac != 5)
		|| !safe_atou(av[1], &(table.nb_philo)) || !table.nb_philo
		|| table.nb_philo > MAX_PHILO
		|| !safe_atou(av[2], &(table.delays.die)) || table.delays.die < 60
		|| !safe_atou(av[3], &(table.delays.eat)) || table.delays.eat < 60
		|| !safe_atou(av[4], &(table.delays.sleep)) || table.delays.sleep < 60
		|| (ac == 6 && !safe_atou(av[5], &table.nb_meals)))
		return (1);
	table_simulate(&table);
}
