/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 08:45:24 by mde-prin          #+#    #+#             */
/*   Updated: 2024/06/26 22:57:31 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

void	ft_philo_proc(t_philo philo, const int NBR)
{
	int			i;
	int			status;
	int			pid[NBR];

	i = -1;
	while (++i < NBR)
	{
		pid[i] = fork();
		philo.id = i + 1;
		if (pid[i] < 0)
			exit(1);
		if (pid[i] == 0)
			return (ft_thread(philo));
	}
	i = -1;
	while (++i < NBR)
		sem_wait(philo.sem_full);
	i = -1;
	while (++i < NBR)
		sem_post(philo.sem_dead);
	sem_post(philo.sem_print);
	i = -1;
	while (++i < NBR)
		waitpid(pid[i], &status, 0);
	ft_close_sem(&philo, 6);
}

void	ft_start(long long args[])
{
	t_philo		philo;

	sem_unlink("/one");
	sem_unlink("/two");
	sem_unlink("/fork");
	sem_unlink("/print");
	sem_unlink("/dead");
	sem_unlink("/flag");
	sem_unlink("/full");
	philo.nbr_philo = args[0];
	philo.time_to_die = args[1];
	philo.time_to_eat = args[2];
	philo.time_to_sleep = args[3];
	philo.nbr_must_eat = args[4];
	ft_open_sem(&philo);
	philo.end = 0;
	philo.start_time = ft_time_ms() + (long long)600;
	philo.time_last_meal = philo.start_time;
	ft_philo_proc(philo, (const int)args[0]);
}

int	ft_check(int argc, char *argv[], long long *args)
{
	int	i;

	if (argc < 5 || argc > 6)
		return (1);
	i = 0;
	while (argv[i + 1])
	{
		args[i] = ft_atoui(argv[i + 1]);
		if (args[i++] == -1 || args[0] == 0 || args[0] > 1000)
			return (1);
	}
	if (argc == 5)
		args[4] = -1;
	return (0);
}

int	main(int argc, char *argv[])
{
	static long long	args[5];

	if (ft_check(argc, argv, args))
		return (1);
	if (!args[4])
		return (0);
	ft_start(args);
	return (0);
}
