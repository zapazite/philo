/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 10:54:21 by mde-prin          #+#    #+#             */
/*   Updated: 2024/06/26 23:38:39 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoui(const char *nptr)
{
	long	n;

	n = 0;
	if ((*nptr < '0' || *nptr > '9') && *nptr != '+')
		return (-1);
	if (*nptr == '+')
		(void)*nptr++;
	if (*nptr < '0' || *nptr > '9')
		return (-1);
	while (*nptr >= '0' && *nptr <= '9')
		n = n * 10 + (*nptr++ - '0');
	if ((*nptr < '0' || *nptr > '9') && *nptr != '\0')
		return (-1);
	if (n > INT_MAX)
		return (-1);
	return ((int)n);
}

void	ft_init_philo(t_philo *philo, long long *args, int i)
{
	int				dead;
	long long		start_time;
	pthread_t		mtid;
	pthread_mutex_t	mutex_dead;
	pthread_mutex_t	mutex_print;

	dead = 0;
	pthread_mutex_init(&mutex_print, NULL);
	pthread_mutex_init(&mutex_dead, NULL);
	start_time = ft_time_ms();
	while (++i < args[0])
	{
		philo[i].dead = &dead;
		philo[i].start_time = start_time;
		philo[i].time_last_meal = start_time;
		philo[i].r_fork = &philo[(i + 1) % args[0]].l_fork;
		philo[i].mutex_dead = &mutex_dead;
		philo[i].mutex_print = &mutex_print;
	}
	while (i--)
		pthread_create(&philo[i].tid, NULL, ft_routine, &philo[i]);
	pthread_create(&mtid, NULL, ft_monitor, philo);
	while (++i < args[0])
		pthread_join(philo[i].tid, NULL);
	pthread_join(mtid, NULL);
}

void	ft_start(long long *args, const int NBR)
{
	int				i;
	t_philo			philo[NBR];
	pthread_mutex_t	mutex_last_meal;
	pthread_mutex_t	mutex_nbr_must_eat;

	pthread_mutex_init(&mutex_last_meal, NULL);
	pthread_mutex_init(&mutex_nbr_must_eat, NULL);
	i = -1;
	while (++i < args[0])
	{
		philo[i].id = i + 1;
		philo[i].nbr_philo = args[0];
		philo[i].time_to_die = args[1];
		philo[i].time_to_eat = args[2];
		philo[i].time_to_sleep = args[3];
		philo[i].nbr_must_eat = args[4];
		philo[i].mutex_last_meal = &mutex_last_meal;
		philo[i].mutex_nbr_must_eat = &mutex_nbr_must_eat;
		pthread_mutex_init(&philo[i].l_fork, NULL);
	}
	ft_init_philo(philo, args, -1);
}

int	ft_check(int ac, char **av, long long *args)
{
	int	i;

	if (ac < 5 || ac > 6)
		return (1);
	i = 0;
	while (av[i + 1])
	{
		args[i] = ft_atoui(av[i + 1]);
		if (args[i++] == -1 || args[0] == 0 || args[0] > 1000)
			return (1);
	}
	if (ac == 5)
		args[4] = -1;
	return (0);
}

int	main(int ac, char *av[])
{
	long long	args[5];

	if (ft_check(ac, av, args))
		return (1);
	if (!args[4])
		return (0);
	ft_start(args, (const int)args[0]);
	return (0);
}
