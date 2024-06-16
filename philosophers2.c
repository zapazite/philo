/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <@student.42luxembourg.lu>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 18:45:36 by mde-prin          #+#    #+#             */
/*   Updated: 2024/06/16 12:58:39 by Zap              ###   ########          */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers2.h"
#include <stdlib.h>

int	ft_atopi(const char *nptr)
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
	if (n > INT_MAX)
		return (-1);
	return ((int)n);
}

long long	ft_time_ms(void)
{
	struct timeval	tv;
	long long		sec;
	long long		usec;

	gettimeofday(&tv, NULL);
	sec = (long long)tv.tv_sec;
	usec = (long long)tv.tv_usec;
	return ((long long)(sec * 1000 + usec / 1000));
}

int	ft_is_dead(t_philo *philo)
{
	pthread_mutex_lock(philo->mutex_dead);
	if (*(philo->dead))
	{
		pthread_mutex_unlock(philo->mutex_dead);
		return (1);
	}
	pthread_mutex_unlock(philo->mutex_dead);
	return (0);
}

void	ft_log(char *s, t_philo *philo)
{
	if (!ft_is_dead(philo))
	{
		pthread_mutex_lock(philo->mutex_print);
		printf("%lld %d %s\n", ft_time_ms() - *philo->start_time, philo->id, s);
		fflush(stdout);
		pthread_mutex_unlock(philo->mutex_print);
	}
}

long long	ft_get_last_meal(t_philo *philo)
{
	long long	last_meal;

	pthread_mutex_lock(philo->mutex_last_meal);
	last_meal = philo->time_last_meal;
	pthread_mutex_unlock(philo->mutex_last_meal);
	return (last_meal);
}

int	ft_starve(t_philo *philo)
{
	if (ft_time_ms() > ft_get_last_meal(philo) + philo->time_to_die)
	{
		ft_log("died", philo);
		pthread_mutex_lock(philo->mutex_dead);
		*(philo->dead) = 1;
		pthread_mutex_unlock(philo->mutex_dead);
		return (1);
	}
	return (0);
}

int	ft_full(t_philo *philo, int *philo_full)
{
	pthread_mutex_lock(philo->mutex_nbr_must_eat);
	if (philo->nbr_must_eat)
	{
		pthread_mutex_unlock(philo->mutex_nbr_must_eat);
		return (0);
	}
	pthread_mutex_unlock(philo->mutex_nbr_must_eat);
	(*philo_full)++;
	if (*philo_full >= philo->nbr_philo)
	{
		pthread_mutex_lock(philo->mutex_dead);
		*(philo->dead) = 1;
		pthread_mutex_unlock(philo->mutex_dead);
		return (1);
	}
	return (0);
}

void	*ft_monitor(void *arg)
{
	int			i;
	int			wait;
	int			philo_full;
	t_philo		*philo;
	long long	time;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->mutex_print);
	wait = *philo[0].start_time - ft_time_ms();
	pthread_mutex_unlock(philo->mutex_print);
	usleep(wait * 100);
	while (!ft_is_dead(&philo[0]))
	{
		philo_full = 0;
		i = -1;
		while (++i < philo[0].nbr_philo)
		{
			if (ft_starve(&philo[i]))
				break ;
			if (ft_full(&philo[i], &philo_full))
				break ;
		}
		usleep(500);
	}
	return (NULL);
}

void	ft_odd_eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->l_fork);
	pthread_mutex_lock(philo->r_fork);
	ft_log("is eating", philo);
	pthread_mutex_lock(philo->mutex_last_meal);
	philo->time_last_meal = ft_time_ms();
	pthread_mutex_unlock(philo->mutex_last_meal);
	pthread_mutex_lock(philo->mutex_nbr_must_eat);
	philo->nbr_must_eat -= (philo->nbr_must_eat > 0);
	pthread_mutex_unlock(philo->mutex_nbr_must_eat);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	ft_even_eating(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	pthread_mutex_lock(&philo->l_fork);
	ft_log("is eating", philo);
	pthread_mutex_lock(philo->mutex_last_meal);
	philo->time_last_meal = ft_time_ms();
	pthread_mutex_unlock(philo->mutex_last_meal);
	pthread_mutex_lock(philo->mutex_nbr_must_eat);
	philo->nbr_must_eat -= (philo->nbr_must_eat > 0);
	pthread_mutex_unlock(philo->mutex_nbr_must_eat);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(&philo->l_fork);
}

void	*ft_routine(void *arg)
{
	t_philo		*philo;
	long long	wait;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->mutex_print);
	wait = *philo->start_time - ft_time_ms();
	pthread_mutex_unlock(philo->mutex_print);
	usleep(wait * 1000);
	ft_log("is thinking", philo);
	while (!ft_is_dead(philo))
	{
		if (philo->id % 2)
			ft_odd_eating(philo);
		else
			ft_even_eating(philo);
		ft_log("is sleeping", philo);
		usleep(philo->time_to_sleep * 1000);
		ft_log("is thinking", philo);
		usleep(400);
	}
	return (NULL);
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
	start_time = ft_time_ms() + 200;
	while (++i < args[0])
	{
		philo[i].dead = &dead;
		philo[i].start_time = &start_time;
		philo[i].time_last_meal = start_time;
		philo[i].r_fork = &philo[(i + 1) % args[0]].l_fork;
		philo[i].mutex_dead = &mutex_dead;
		philo[i].mutex_print = &mutex_print;
		pthread_create(&philo[i].tid, NULL, ft_routine, &philo[i]);
	}
	pthread_create(&mtid, NULL, ft_monitor, philo);
	i = -1;
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

	if (ac < 5 || 6 < ac)
		return (1);
	i = 0;
	while (av[i + 1])
	{
		args[i] = ft_atopi(av[i + 1]);
		if (args[i++] == -1)
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
	ft_start(args, (const int)args[0]);
	return (0);
}
