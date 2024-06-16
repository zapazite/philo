/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers2.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <@student.42luxembourg.lu>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 18:48:05 by mde-prin          #+#    #+#             */
/*   Updated: 2024/06/16 11:14:48 by Zap              ###   ########          */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

typedef struct s_philo
{
	int				id;
	int				nbr_philo;
	int				*dead;
	int				nbr_must_eat;
	pthread_t		tid;
	long long		*start_time;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	long long		time_last_meal;
	pthread_mutex_t	l_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*mutex_print;
	pthread_mutex_t	*mutex_dead;
	pthread_mutex_t	*mutex_last_meal;
	pthread_mutex_t	*mutex_nbr_must_eat;
}	t_philo;
