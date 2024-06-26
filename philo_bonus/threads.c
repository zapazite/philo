/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:33:26 by mde-prin          #+#    #+#             */
/*   Updated: 2024/06/26 22:58:47 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_monitor(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	while (!ft_is_end(philo))
	{
		sem_wait(philo->sem_print);
		if (ft_is_dead(philo) && !ft_is_end(philo))
		{
			printf("%lld %d died\n", ft_time_ms()
				- philo->start_time, philo->id);
			i = 0;
			while (i++ < philo->nbr_philo)
				sem_post(philo->sem_dead);
			return (NULL);
		}
		sem_post(philo->sem_print);
		usleep(400);
	}
	return (NULL);
}

void	*ft_listen(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sem_wait(philo->sem_dead);
	sem_wait(philo->sem_flag);
	philo->end = 1;
	sem_post(philo->sem_flag);
	sem_post(philo->sem_full);
	return (NULL);
}

void	*ft_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	usleep((philo->start_time - ft_time_ms()) * 1000);
	ft_log("is thinking", philo);
	while (!ft_is_end(philo))
	{
		ft_eat_sleep(philo);
		ft_log("is thinking", philo);
	}
	return (NULL);
}

void	ft_eat_sleep(t_philo *philo)
{
	sem_wait(philo->sem_1);
	sem_wait(philo->sem_fork);
	ft_log("has taken a fork", philo);
	if (philo->nbr_philo == 1)
		return ((void)usleep(philo->time_to_die * 2000));
	sem_wait(philo->sem_2);
	sem_wait(philo->sem_fork);
	ft_log("has taken a fork", philo);
	sem_post(philo->sem_2);
	ft_log("is eating", philo);
	sem_wait(philo->sem_flag);
	philo->time_last_meal = ft_time_ms();
	sem_post(philo->sem_flag);
	if (!(--(philo->nbr_must_eat)))
		sem_post(philo->sem_full);
	usleep(philo->time_to_eat * 500);
	sem_post(philo->sem_1);
	usleep(philo->time_to_eat * 500);
	sem_post(philo->sem_fork);
	sem_post(philo->sem_fork);
	ft_log("is sleeping", philo);
	usleep(philo->time_to_sleep * 1000);
}

void	ft_thread(t_philo philo)
{
	char	*sem_flag;

	sem_flag = ft_itoa(philo.id);
	sem_unlink(sem_flag);
	philo.sem_flag = sem_open(sem_flag, O_CREAT, S_IRWXU, 1);
	free(sem_flag);
	if (philo.sem_flag == SEM_FAILED)
		exit(1);
	pthread_create(&philo.ptid, NULL, (void *)ft_routine, &philo);
	pthread_create(&philo.tid, NULL, (void *)ft_monitor, &philo);
	pthread_create(&philo.ltid, NULL, (void *)ft_listen, &philo);
	pthread_join(philo.tid, NULL);
	pthread_join(philo.ltid, NULL);
	pthread_join(philo.ptid, NULL);
	sem_close(philo.sem_flag);
	ft_close_sem(&philo, 6);
}
