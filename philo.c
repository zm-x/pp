/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:28:39 by zael-mou          #+#    #+#             */
/*   Updated: 2025/03/09 21:10:01 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	error_func(ac)
{
	if(ac < 5 || ac > 6)
		return(printf("wrong arguments\n"), 0);
	return (1);
}

int	ft_atoi(char *nptr, utilis_t *useful)
{
	int	i;
	int	r;
	int	sign;

	i = 0;
	r = 0;
	sign = 1;
	while ((nptr[i] == 32) || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if ((nptr[i] == '-') || (nptr[i] == '+'))
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (nptr[i])
	{
		if (nptr[i] < '0' || nptr[i] > '9')
			return((useful->error = 1), 0);
		r = r * 10 + nptr[i] - '0';
		i++;
	}
	return (r * sign);
}

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	check_locks(data_t *philo)
{
	if (philo->check_left)
	{
		pthread_mutex_unlock(&philo->forks[philo->left_fork]);
		philo->check_left = 0;
	}
	if (philo->check_right)
	{
		pthread_mutex_unlock(&philo->forks[philo->right_fork]);
		philo->check_right = 0;
	}
}

void	*check_death(void *p)
{
	data_t	*philo;
	int		i;
	long	eat;
	long	die;

	philo = (data_t *)p;
	while (1)
	{
		i = 0;
		while (i < philo[0].nphilo)
		{
			pthread_mutex_lock(philo[i].eatt);
			eat = philo[i].last_eat;
			pthread_mutex_unlock(philo[i].eatt);
			pthread_mutex_lock(philo[i].dead);
			die = philo[i].ndie;
			pthread_mutex_unlock(philo[i].dead);
			if (get_time() - eat > die)
			{
				pthread_mutex_lock(philo[i].print);
				printf("%ld %d died\n", get_time() - philo[i].curent_time,
					philo[i].place + 1);
				pthread_mutex_unlock(philo[i].print);
				pthread_mutex_lock(philo[i].dead);
				*philo[i].is_dead = 1;
				pthread_mutex_unlock(philo[i].dead);
				return (NULL);
			}
			i++;
		}
	}
	return (NULL);
}

int	thinking(data_t *philo)
{
	pthread_mutex_lock(philo->print);
	pthread_mutex_lock(philo->dead);
	if (*philo->is_dead)
	{
		pthread_mutex_unlock(philo->print);
		pthread_mutex_unlock(philo->dead);
		return (0);
	}
	pthread_mutex_unlock(philo->dead);
	printf("%ld %d is thinking\n", get_time() - philo->curent_time,
		   philo->place + 1);
	pthread_mutex_unlock(philo->print);
	if (philo->place % 2 != 0)
		usleep(500);
	return (1);
}

void	*all(void *p)
{
	data_t	*philo;
	int		dead;

	philo = (data_t *)p;
	dead = 0;
	philo->left_fork = philo->place % philo->nphilo;
	philo->right_fork = (philo->left_fork + 1) % philo->nphilo;
	pthread_mutex_lock(philo->eatt);
	philo->last_eat = get_time();
	pthread_mutex_unlock(philo->eatt);
	while (1)
	{
		if (!thinking(philo))
			break;
		if (philo->place % 2 == 0)
		{
			pthread_mutex_lock(philo->dead);
			if (*philo->is_dead)
			{
				pthread_mutex_unlock(philo->dead);
				break ;
			}
			pthread_mutex_unlock(philo->dead);
			pthread_mutex_lock(&philo->forks[philo->right_fork]);
			philo->check_right = 1;
			pthread_mutex_lock(philo->dead);
			if (*philo->is_dead)
			{
				pthread_mutex_unlock(philo->dead);
				pthread_mutex_unlock(&philo->forks[philo->right_fork]);
				philo->check_right = 0;
				break ;
			}
			pthread_mutex_unlock(philo->dead);
			pthread_mutex_lock(&philo->forks[philo->left_fork]);
			philo->check_left = 1;
		}
		else
		{
			pthread_mutex_lock(philo->dead);
			if (*philo->is_dead)
			{
				pthread_mutex_unlock(philo->dead);
				break ;
			}
			pthread_mutex_unlock(philo->dead);
			pthread_mutex_lock(&philo->forks[philo->left_fork]);
			philo->check_left = 1;
			pthread_mutex_lock(philo->dead);
			if (*philo->is_dead)
			{
				pthread_mutex_unlock(philo->dead);
				pthread_mutex_unlock(&philo->forks[philo->left_fork]);
				philo->check_left = 0;
				break ;
			}
			pthread_mutex_unlock(philo->dead);
			pthread_mutex_lock(&philo->forks[philo->right_fork]);
			philo->check_right = 1;
		}
		pthread_mutex_lock(philo->print);
		pthread_mutex_lock(philo->dead);
		if (*philo->is_dead)
		{
			pthread_mutex_unlock(philo->dead);
			pthread_mutex_unlock(philo->print);
			break ;
		}
		pthread_mutex_unlock(philo->dead);
		printf("%ld %d has taken a fork\n", get_time() - philo->curent_time,
		philo->place + 1);
		pthread_mutex_lock(philo->dead);
		if (*philo->is_dead)
		{
			pthread_mutex_unlock(philo->dead);
			pthread_mutex_unlock(philo->print);
			break ;
		}
		pthread_mutex_unlock(philo->dead);
		printf("%ld %d has taken a fork\n", get_time() - philo->curent_time,
		philo->place + 1);
		pthread_mutex_lock(philo->dead);
		if (*philo->is_dead)
		{
			pthread_mutex_unlock(philo->dead);
			pthread_mutex_unlock(philo->print);
			break ;
		}
		pthread_mutex_unlock(philo->dead);
		printf("%ld %d is eating\n", get_time() - philo->curent_time,
			philo->place + 1);
		pthread_mutex_unlock(philo->print);
		philo->ntimes--;
		pthread_mutex_lock(philo->eatt);
		philo->last_eat = get_time();
		pthread_mutex_unlock(philo->eatt);
		usleep(philo->neat * 1000);
		philo->check_right = 0;
		philo->check_left = 0;
		pthread_mutex_unlock(&philo->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->forks[philo->right_fork]);
		if (philo->ntimes == 0)
		{
			break ;
		}
		pthread_mutex_lock(philo->print);
		pthread_mutex_lock(philo->dead);
		if (*philo->is_dead)
		{
			pthread_mutex_unlock(philo->dead);
			pthread_mutex_unlock(philo->print);
			break ;
		}
		pthread_mutex_unlock(philo->dead);
		printf("%ld %d is sleeping\n", get_time() - philo->curent_time,
			philo->place + 1);
		pthread_mutex_unlock(philo->print);
		usleep(philo->nsleep * 1000);
	}
	if (philo->check_left)
		pthread_mutex_unlock(&philo->forks[philo->left_fork]);
	if (philo->check_right)
		pthread_mutex_unlock(&philo->forks[philo->right_fork]);
	return (NULL);
}

void	give_value(data_t *philo, char **av, int ac, utilis_t *useful)
{
	int	i;

	i = 1;
	philo->nphilo = ft_atoi(av[i++], useful);
	philo->ndie = ft_atoi(av[i++], useful);
	philo->neat = ft_atoi(av[i++], useful);
	philo->nsleep = ft_atoi(av[i++], useful);
	philo->ntimes = -1;
	if (ac == 6)
		philo->ntimes = ft_atoi(av[i++], useful);
	philo->eatt = &useful->eatt;
	philo->dead = &useful->dead;
	philo->is_dead = &useful->is_dead;
	philo->check_left = 0;
	philo->check_right = 0;
	philo->curent_time = useful->start;
	philo->forks = useful->forks;
	philo->print = &useful->print;
	philo->last_eat = get_time();
	philo->av = ft_atoi(av[2], useful);
	philo->place = useful->i;
}

int	init(utilis_t *useful, char **av, int ac)
{
	while (useful->i < ft_atoi(av[1], useful))
		pthread_mutex_init(&useful->forks[useful->i++], NULL);
	pthread_mutex_init(&useful->dead, NULL);
	pthread_mutex_init(&useful->eatt, NULL);
	useful->i = 0;
	while (useful->i < ft_atoi(av[1], useful))
	{
		give_value(&useful->philo[useful->i], av, ac , useful);
		useful->i++;
		if (useful->error)
			return(printf("wrong arguments\n"), 0);
	}
	useful->i = 0;
	return (1);
}
int	main(int ac, char **av)
{
	utilis_t	useful;
	
	useful.error = 0;
	useful.i = 0;
	if (!error_func(ac))
		return (1);
	useful.start = get_time();
	pthread_mutex_init(&useful.print, NULL);
	useful.is_dead = 0;
	if (!init(&useful, av, ac))
		return (1);
	while (useful.i++ < useful.philo[0].nphilo)
		pthread_create(&useful.thread[useful.i - 1], NULL, all, &useful.philo[useful.i - 1]);
	pthread_create(&useful.lm9dm, NULL, check_death, useful.philo);
	useful.i = 0;
	pthread_join(useful.lm9dm, NULL);
	while (useful.i < useful.philo[0].nphilo)
	{
		pthread_join(useful.thread[useful.i], NULL);
		useful.i++;
	}
	return (0);
}
