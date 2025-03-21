/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:25:01 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/03/21 21:39:23 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

int					ft_atoi(const char *ptr);
long long			*get_args(int argc, char **argv);
void				error(void);
pthread_mutex_t		*make_threads(pthread_t	*threads, long long *args, char *forks);
void				*philosopher(void	*arg);

typedef struct s_philo
{
	char				*forks;
	long long			*args;
	int					number;
	pthread_mutex_t		*lock;
}	t_philo;

int		check_forks(pthread_mutex_t *lock, t_philo *philo, int forks_index[2]);
void	dying_alone(pthread_t *threads, long long *args, char *forks);
int		finish(pthread_mutex_t *lock, long long *args, char *forks,
			pthread_t *threads);
long	timestamp(struct timeval arg_tv, pthread_mutex_t *lock);
void	increment(t_philo *philo, int i);

#endif