/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 10:54:35 by mde-prin          #+#    #+#             */
/*   Updated: 2024/06/26 23:46:34 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_log(char *s, t_philo *philo)
{
	pthread_mutex_lock(philo->mutex_print);
	if (!ft_is_dead(philo))
		printf("%lld %d %s\n", ft_time_ms() - philo->start_time, philo->id, s);
	pthread_mutex_unlock(philo->mutex_print);
}

void	*ft_monitor(void *arg)
{
	int			i;
	int			philo_full;
	t_philo		*philo;

	philo = (t_philo *)arg;
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
		usleep(400);
	}
	return (NULL);
}

void	*ft_routine(void *arg)
{
	t_philo		*philo;

	philo = (t_philo *)arg;
	ft_log("is thinking", philo);
	if (philo->nbr_philo == 1)
		return (ft_log("has taken a fork", philo), NULL);
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

void	ft_odd_eating(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	ft_log("has taken a fork", philo);
	pthread_mutex_lock(&philo->l_fork);
	ft_log("has taken a fork", philo);
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
	pthread_mutex_lock(&philo->l_fork);
	ft_log("has taken a fork", philo);
	pthread_mutex_lock(philo->r_fork);
	ft_log("has taken a fork", philo);
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
