/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:28:39 by zael-mou          #+#    #+#             */
/*   Updated: 2025/02/27 17:04:39 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long get_time()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return((time.tv_sec * 1000) + (time.tv_usec / 1000)); 
}

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

void    give_init(data_t *philo)
{
    int i;

    i = 0;
    while (i < philo->nphilo)
        pthread_mutex_init(&philo->forks[i++], NULL);
    pthread_mutex_init(&philo->mutex, NULL);
    pthread_mutex_init(&philo->eat , NULL);
    pthread_mutex_init(&philo->print , NULL);
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
    philo->curent_time = get_time();
    give_init(philo);
}

void    *all(void *p)
{
    data_t *philo;

    philo = (data_t *)p;
    //while (1)
    //{
    pthread_mutex_lock(&philo->print);
        philo->left_fork = philo->place % philo->nphilo;
        philo->right_fork = philo->left_fork + 1;
        printf("left is %d\n", philo->left_fork);
        printf("right is %d\n", philo->right_fork);
    pthread_mutex_unlock(&philo->print);
    return (NULL);
    //}
}

int main(int ac, char **av)
{
    int i;
    data_t philo[200];
    pthread_t thread[200];

    i = 0;
    if (ac < 5 || ac > 6)
        error_func();
    while (i < ft_atoi(av[1]))
    {
        philo[i].place = i;
        give_value(&philo[i], av, ac);
        i++;
    }
    i = 0;
    while(i < philo[0].nphilo)
    {
        pthread_create(&thread[i], NULL, all, &philo);
        i++;
    }
    i = 0;
    while (i < philo[0].nphilo)
    {
        pthread_join(thread[i], NULL);
        i++;
    }
}