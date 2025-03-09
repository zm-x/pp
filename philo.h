/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:24:53 by zael-mou          #+#    #+#             */
/*   Updated: 2025/03/09 20:32:24 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct philos
{
    int nphilo;
    int ndie;
    int neat;
    int nsleep;
    int ntimes;
    long curent_time;
    long last_eat;
    int place;
    int left_fork;
    int right_fork;
    int av;
    int *is_dead;
    int check_left;
    int check_right;
    pthread_mutex_t *forks;
    pthread_mutex_t sleep;
    pthread_mutex_t *print;
    pthread_mutex_t *dead;
    pthread_mutex_t *eatt;
    pthread_mutex_t think;
    pthread_mutex_t eat;
}   data_t;

typedef struct utilis
{
    int				i;
	pthread_t		thread[200];
	pthread_t		lm9dm;
	pthread_mutex_t	forks[200];
	data_t	philo[200];
	pthread_mutex_t	print;
	pthread_mutex_t	eatt;
	pthread_mutex_t	dead;
	long			start;
    int             error;
	int				is_dead;
}   utilis_t;

#endif