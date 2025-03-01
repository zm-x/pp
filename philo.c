/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:28:39 by zael-mou          #+#    #+#             */
/*   Updated: 2025/03/01 14:28:02 by zael-mou         ###   ########.fr       */
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

void    error_func()
{
    exit((printf("wrong arguments\n") , 1));
}

int	ft_atoi(char *nptr)
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
        error_func();
        r = r * 10 + nptr[i] - '0';
        i++;
    }
	return (r * sign);
}

void    give_value(data_t *philo, char **av, int ac)
{
    int i;
    
    i = 1;
    philo->nphilo = ft_atoi(av[i++]);
    philo->ndie = ft_atoi(av[i++]);
    philo->neat = ft_atoi(av[i++]);
    philo->nsleep = ft_atoi(av[i++]);
    if (ac == 6)
        philo->ntimes = ft_atoi(av[i++]);
}

long get_time()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return((time.tv_sec * 1000) + (time.tv_usec / 1000)); 
}

void    *all(void *p)
{
    data_t *philo;
    int dead;

    philo = (data_t *)p;
    dead = 0;
    philo->left_fork = philo->place % philo->nphilo;
    philo->right_fork = (philo->left_fork + 1) % philo->nphilo;
    philo->last_eat = get_time();
    while (1)
    {
        pthread_mutex_lock(philo->print);
        if (*philo->is_dead)
        {
            pthread_mutex_unlock(philo->print);
            break;
        }
        pthread_mutex_unlock(philo->print);
        if (get_time() - philo->last_eat > philo->ndie)
        {
            pthread_mutex_lock(philo->print);
            printf("%ld %d is died\n",  get_time() - philo->curent_time, philo->place + 1);
            *philo->is_dead = 1;
            pthread_mutex_unlock(philo->print);
            break;
        }
        pthread_mutex_lock(philo->print);
        printf("%ld %d is thinking\n", get_time() - philo->curent_time, philo->place + 1);
        pthread_mutex_unlock(philo->print);
        if (philo->place % 2 != 0)
            usleep(500);
        pthread_mutex_lock(&philo->forks[philo->left_fork]);
        pthread_mutex_lock(&philo->forks[philo->right_fork]);
        pthread_mutex_lock(philo->print);
        printf("%ld %d has taken a fork\n", get_time() - philo->curent_time, philo->place + 1);
        printf("%ld %d has taken a fork\n", get_time() - philo->curent_time, philo->place + 1);
        pthread_mutex_unlock(philo->print);
        pthread_mutex_lock(philo->print);
        printf("%ld %d is eating\n", get_time() - philo->curent_time, philo->place + 1);
        pthread_mutex_unlock(philo->print);
        philo->last_eat = get_time();
        usleep(philo->neat * 1000);
        pthread_mutex_unlock(&philo->forks[philo->left_fork]);
        pthread_mutex_unlock(&philo->forks[philo->right_fork]);
        pthread_mutex_lock(philo->print);
        printf("%ld %d is sleeping\n", get_time() - philo->curent_time, philo->place + 1);
        pthread_mutex_unlock(philo->print);
        usleep(philo->nsleep * 1000);
    }
    return (NULL);
}

int main(int ac, char **av)
{
    int i;
    data_t philo[200];
    pthread_t thread[200];
    pthread_mutex_t forks[200];
    pthread_mutex_t print;
    long start;
    int is_dead;

    i = 0;
    if (ac < 5 || ac > 6)
        error_func();
    start = get_time();
    pthread_mutex_init(&print, NULL);
    is_dead = 0;
    while (i < ft_atoi(av[1]))
        pthread_mutex_init(&forks[i++], NULL);
    i = 0;
    while (i < ft_atoi(av[1]))
    {
        philo[i].is_dead = &is_dead;
        philo[i].av = ft_atoi(av[2]);
        philo[i].place = i;
        philo[i].forks = forks;
        philo[i].print = &print;
        give_value(&philo[i], av, ac);
        philo[i].curent_time = start;
        i++;
    }
    i = 0;
    while(i < philo[0].nphilo)
    {
        pthread_create(&thread[i], NULL, all, &philo[i]);
        i++;
    }
    i = 0;
    while (i < philo[0].nphilo)
    {
        if (is_dead)
            break;
        pthread_join(thread[i], NULL);
        i++;
    }
    pthread_mutex_destroy(&print);
    for (int i = 0; i < ft_atoi(av[1]); i++)
        pthread_mutex_destroy(&forks[i]);
}