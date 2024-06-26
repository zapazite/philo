/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 10:54:49 by mde-prin          #+#    #+#             */
/*   Updated: 2024/06/26 23:24:17 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
	int	dead;

	pthread_mutex_lock(philo->mutex_dead);
	dead = *(philo->dead);
	pthread_mutex_unlock(philo->mutex_dead);
	if (dead)
		return (1);
	return (0);
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
	if (ft_time_ms() >= ft_get_last_meal(philo) + philo->time_to_die)
	{
		pthread_mutex_lock(philo->mutex_dead);
		*(philo->dead) = 1;
		pthread_mutex_unlock(philo->mutex_dead);
		pthread_mutex_lock(philo->mutex_print);
		printf("%lld %d died\n", ft_time_ms() - philo->start_time, philo->id);
		pthread_mutex_unlock(philo->mutex_print);
		return (1);
	}
	return (0);
}

int	ft_full(t_philo *philo, int *philo_full)
{
	int	nbr_must_eat;

	pthread_mutex_lock(philo->mutex_nbr_must_eat);
	nbr_must_eat = philo->nbr_must_eat;
	pthread_mutex_unlock(philo->mutex_nbr_must_eat);
	if (nbr_must_eat)
		return (0);
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
