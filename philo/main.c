/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:24:29 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/03/21 21:46:19 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp(struct timeval arg_tv, pthread_mutex_t *lock);

char	die(t_philo	*philo, int forks_index[2], pthread_mutex_t *lock)
{
	suseconds_t			last_time;
	struct timeval		tv;
	int					tmp;

	gettimeofday(&tv, NULL);
	last_time = (tv.tv_sec * 1000) + tv.tv_usec / 1000;
	tmp = philo->args[1];
	while (!check_forks(lock, philo, forks_index))
	{
		gettimeofday(&tv, NULL);
		if (((tv.tv_sec * 1000) + tv.tv_usec / 1000) - last_time
		> tmp)
		{
			pthread_mutex_lock(lock);
			gettimeofday((struct timeval *)philo->args[7], NULL);
			philo->args[5] = philo->number + 1;
			pthread_mutex_unlock(lock);
			return (1);
		}
	}
	return (0);
}

void	eat_then_sleep(t_philo	*philo, int forks_index[2],
		pthread_mutex_t *lock)
{
	struct timeval		tv;
	int					arg;

	gettimeofday(&tv, NULL);
	printf("%li %i has taken a fork\n", timestamp(tv, lock), philo->number + 1);
	pthread_mutex_lock(lock);
	philo->forks[forks_index[1]] = 'u';
	philo->forks[forks_index[0]] = 'u';
	arg = philo->args[2];
	pthread_mutex_unlock(lock);
	printf("%li %i is eating\n", timestamp(tv, lock), philo->number + 1);
	usleep(arg * 1000);
	pthread_mutex_lock(lock);
	philo->forks[forks_index[1]] = 'a';
	philo->forks[forks_index[0]] = 'a';
	arg = philo->args[3];
	pthread_mutex_unlock(lock);
	gettimeofday(&tv, NULL);
	printf("%li %i is sleeping\n", timestamp(tv, lock), philo->number + 1);
	usleep(arg * 1000);
	gettimeofday(&tv, NULL);
	printf("%li %i is thinking\n", timestamp(tv, lock), philo->number + 1);
}

void	*philosopher(void	*arg)
{
	t_philo				*philo;
	int					forks_index[2];
	int					i;
	pthread_mutex_t		*lock;

	philo = (t_philo *)arg;
	if (philo->number == 0)
		forks_index[0] = philo->args[0] - 1;
	else
		forks_index[0] = philo->number - 1;
	forks_index[1] = philo->number;
	i = 0;
	while (i < philo->args[4])
	{
		if (check_forks(philo->lock, philo, forks_index) == 1)
			(eat_then_sleep(philo, forks_index, philo->lock), i++);
		if (die(philo, forks_index, philo->lock) == 1)
			break ;
	}
	increment(philo, i);
	lock = philo->lock;
	(pthread_mutex_lock(lock), free(arg));
	return (pthread_mutex_unlock(lock), NULL);
}

void	check_death(long long *args, pthread_mutex_t *lock)
{
	int					tmp;

	while (1)
	{
		pthread_mutex_lock(lock);
		if (args[6] == args[0] || args[5] != 0)
		{
			pthread_mutex_unlock(lock);
			break ;
		}
		pthread_mutex_unlock(lock);
	}
	pthread_mutex_lock(lock);
	tmp = args[5];
	pthread_mutex_unlock(lock);
	if (tmp != 0)
		printf("%li %i died\n", timestamp(*(struct timeval *)args[7], lock), tmp);
}

//	args[0] = number_of_philosophers
//	args[1] = time_to_die
//	args[2] = time_to_eat
//	args[3] = time_to_sleep
//	args[4] = number_of_times_each_philosopher_must_eat
//			(optional)(1 if not specifed)
// 	args[5] = the dead philosopher
//	args[6] = The number of philosophers who have
//			finished eating (it equals 0 in the beginning)
// 	args[7] = time of death of a philosopher (is a timeval pointer casted to long long)

int	main(int argc, char **argv)
{
	long long			*args;
	char				*forks;
	pthread_t			*threads;	
	struct timeval		tv;
	pthread_mutex_t		*lock;

	args = get_args(argc, argv);
	if (args == NULL)
		return (error(), 0);
	forks = malloc(args[0] * sizeof(char));
	threads = malloc(args[0] * sizeof(pthread_t));
	memset(forks, 'a', (args[0] * sizeof(char)));
	if (args[0] == 1)
		return (dying_alone(threads, args, forks), 0);
	args[7] = (long long)&tv;
	lock = make_threads(threads, args, forks);
	usleep(1000);
	lock = make_threads(threads, args, forks);
	check_death(args, lock);
	return (finish(lock, args, forks, threads));
}
