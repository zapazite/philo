/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <@student.42luxembourg.lu>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 18:48:05 by mde-prin          #+#    #+#             */
/*   Updated: 2024/06/17 11:10:17 by mde-prin         ###   ########.fr       */
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
	long long		start_time;
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

long long	ft_time_ms(void);
long long	ft_get_last_meal(t_philo *philo);
int			ft_is_dead(t_philo *philo);
int			ft_starve(t_philo *philo);
int			ft_full(t_philo *philo, int *philo_full);

void		ft_log(char *s, t_philo *philo);
void		*ft_monitor(void *arg);
void		ft_odd_eating(t_philo *philo);
void		ft_even_eating(t_philo *philo);
void		*ft_routine(void *arg);

void		ft_init_philo(t_philo *philo, long long *args, int i);
void		ft_start(long long *args, const int NBR);
int			ft_check(int ac, char **av, long long *args);
int			ft_atopi(const char *nptr);
