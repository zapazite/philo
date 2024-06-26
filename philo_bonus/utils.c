/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-prin <mde-prin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 22:37:54 by mde-prin          #+#    #+#             */
/*   Updated: 2024/06/26 23:03:50 by mde-prin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	wr_nbr(char *num, long nbr, size_t *i)
{
	if (nbr < 0)
	{
		num[0] = '-';
		nbr = -nbr;
		*i = 1;
	}
	if (nbr > 9)
	{
		wr_nbr(num, nbr / 10, i);
		*i += 1;
	}
	num[*i] = '0' + nbr % 10;
}

int	ft_loga(int n)
{
	if (n)
		return (ft_loga(n / 10) + 1);
	return (0);
}

char	*ft_itoa(int n)
{
	char	*num;
	long	nbr;
	size_t	i;

	i = 1;
	nbr = (long)n;
	num = (char *)malloc(sizeof(char) * ((n <= 0) + ft_loga(n) + 2));
	if (!num)
		return (NULL);
	wr_nbr(num, nbr, &i);
	num[0] = '/';
	num[i + 1] = '\0';
	return (num);
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

void	ft_log(char *s, t_philo *philo)
{
	long long	log_time;

	log_time = ft_time_ms() - philo->start_time;
	sem_wait(philo->sem_print);
	if (!ft_is_end(philo))
		printf("%lld %d %s\n", log_time, philo->id, s);
	sem_post(philo->sem_print);
}
