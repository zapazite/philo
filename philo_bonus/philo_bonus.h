/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 09:09:28 by mde-prin          #+#    #+#             */
/*   Updated: 2024/06/26 22:58:08 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <sys/time.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/wait.h>

typedef struct s_philo
{
	int				id;
	int				end;
	int				dead;
	int				nbr_philo;
	int				nbr_must_eat;
	sem_t			*sem_1;
	sem_t			*sem_2;
	sem_t			*sem_fork;
	sem_t			*sem_dead;
	sem_t			*sem_full;
	sem_t			*sem_flag;
	sem_t			*sem_print;
	pthread_t		tid;
	pthread_t		ltid;
	pthread_t		ptid;
	long long		start_time;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	long long		time_last_meal;
}	t_philo;

//=======philo_bonus=======
int			ft_atoui(const char *nptr);
void		ft_philo_proc(t_philo philo, const int NBR);
void		ft_start(long long args[]);
int			ft_check(int argc, char *argv[], long long *args);
//=========threads=========
void		*ft_monitor(void *arg);
void		*ft_listen(void *arg);
void		*ft_routine(void *arg);
void		ft_eat_sleep(t_philo *philo);
void		ft_thread(t_philo philo);
//==========utils==========
void		wr_nbr(char *num, long nbr, size_t *i);
int			ft_loga(int n);
char		*ft_itoa(int n);
long long	ft_time_ms(void);
void		ft_log(char *s, t_philo *philo);
//==========utils2=========
int			ft_is_end(t_philo *philo);
int			ft_is_dead(t_philo *philo);
void		ft_open_sem(t_philo *philo);
int			ft_close_sem(t_philo *philo, int n);
