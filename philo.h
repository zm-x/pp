/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:24:53 by zael-mou          #+#    #+#             */
/*   Updated: 2025/02/28 16:53:06 by zael-mou         ###   ########.fr       */
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
    int place;
    int left_fork;
    int right_fork;
    pthread_mutex_t *forks;
    pthread_mutex_t sleep;
    pthread_mutex_t *print;
    pthread_mutex_t think;
    pthread_mutex_t eat;
}   data_t;

#endif