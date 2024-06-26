/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:40:10 by mde-prin          #+#    #+#             */
/*   Updated: 2024/06/26 22:42:45 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_is_end(t_philo *philo)
{
	int	end;

	sem_wait(philo->sem_flag);
	end = philo->end;
	sem_post(philo->sem_flag);
	return (end);
}

int	ft_is_dead(t_philo *philo)
{
	int	dead;

	sem_wait(philo->sem_flag);
	dead = (ft_time_ms() >= philo->time_last_meal + philo->time_to_die);
	sem_post(philo->sem_flag);
	return (dead);
}

void	ft_open_sem(t_philo *philo)
{
	philo->sem_1 = sem_open("/one", O_CREAT | O_EXCL, S_IRWXU,
			philo->nbr_philo / 2 + (philo->nbr_philo == 1));
	if (philo->sem_1 == SEM_FAILED)
		exit(ft_close_sem(philo, 0));
	philo->sem_2 = sem_open("/two", O_CREAT | O_EXCL, S_IRWXU,
			philo->nbr_philo / 2);
	if (philo->sem_2 == SEM_FAILED)
		exit(ft_close_sem(philo, 1));
	philo->sem_fork = sem_open("/fork", O_CREAT | O_EXCL, S_IRWXU,
			philo->nbr_philo);
	if (philo->sem_fork == SEM_FAILED)
		exit(ft_close_sem(philo, 2));
	philo->sem_dead = sem_open("/dead", O_CREAT | O_EXCL, S_IRWXU, 0);
	if (philo->sem_dead == SEM_FAILED)
		exit(ft_close_sem(philo, 3));
	philo->sem_full = sem_open("/full", O_CREAT | O_EXCL, S_IRWXU, 0);
	if (philo->sem_full == SEM_FAILED)
		exit(ft_close_sem(philo, 4));
	philo->sem_print = sem_open("/print", O_CREAT | O_EXCL, S_IRWXU, 1);
	if (philo->sem_print == SEM_FAILED)
		exit(ft_close_sem(philo, 5));
}

int	ft_close_sem(t_philo *philo, int n)
{
	if (n > 0)
		sem_close(philo->sem_1);
	if (n > 1)
		sem_close(philo->sem_2);
	if (n > 2)
		sem_close(philo->sem_fork);
	if (n > 3)
		sem_close(philo->sem_dead);
	if (n > 4)
		sem_close(philo->sem_full);
	if (n > 5)
		sem_close(philo->sem_print);
	return (1);
}
