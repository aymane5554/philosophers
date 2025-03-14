/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayel-arr <ayel-arr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:24:29 by ayel-arr          #+#    #+#             */
/*   Updated: 2025/03/13 17:55:18 by ayel-arr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	die(t_philo	*philo, pthread_mutex_t *lock, int forks_index[2])
{
	suseconds_t			last_time;
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	last_time = (tv.tv_sec * 1000000) + tv.tv_usec;
	while (philo->forks[forks_index[0]] != 'a'
		&& philo->forks[forks_index[1]] != 'a')
	{
		gettimeofday(&tv, NULL);
		if (((tv.tv_sec * 1000000) + tv.tv_usec) - last_time
			> philo->args[1] * 1000)
		{
			printf("%i died\n", philo->number + 1);
			pthread_mutex_lock(lock);
			philo->args[5]--;
			pthread_mutex_unlock(lock);
			return ;
		}
	}
}

void	eat_then_sleep(t_philo	*philo, int forks_index[2],
		pthread_mutex_t *lock)
{
	pthread_mutex_init(lock, NULL);
	printf("%i is eating\n", philo->number + 1);
	pthread_mutex_lock(lock);
	philo->forks[forks_index[1]] = 'u';
	philo->forks[forks_index[0]] = 'u';
	pthread_mutex_unlock(lock);
	usleep(philo->args[2] * 1000);
	pthread_mutex_lock(lock);
	philo->forks[forks_index[1]] = 'a';
	philo->forks[forks_index[0]] = 'a';
	pthread_mutex_unlock(lock);
	printf("%i is sleeping\n", philo->number + 1);
	usleep(philo->args[3] * 1000);
	printf("%i is thinking\n", philo->number + 1);
}

void	*philosopher(void	*arg)
{
	t_philo				*philo;
	pthread_mutex_t		lock;
	int					forks_index[2];
	int					i;

	philo = (t_philo *)arg;
	if (philo->number == 0)
		forks_index[0] = philo->args[0] - 1;
	else
		forks_index[0] = philo->number - 1;
	forks_index[1] = philo->number;
	i = 0;
	pthread_mutex_init(&lock, NULL);
	while (i < philo->args[4])
	{
		if (philo->forks[forks_index[0]] == 'a'
			&& philo->forks[forks_index[1]] == 'a')
		{
			eat_then_sleep(philo, forks_index, &lock);
			i++;
		}
		die(philo, &lock, forks_index);
	}
	pthread_mutex_lock(&lock);
	philo->args[6]++;
	return (pthread_mutex_unlock(&lock), free(arg), NULL);
}

void	death_check(int *args)
{
	while (args[5] == args[0] && args[4] != 0 && args[6] != args[0])
		continue ;
}

//	args[0] = number_of_philosophers
//	args[1] = time_to_die
//	args[2] = time_to_eat
//	args[3] = time_to_sleep
//	args[4] = number_of_times_each_philosopher_must_eat
//			(optional)(1 if not specifed)
// 	args[5] = number_of_philosophers_alive
//	args[6] = The number of people who have
//			finished eating (it equals 0 in the beginning)

int	main(int argc, char **argv)
{
	int			*args;
	char		*forks;
	pthread_t	*threads;

	args = get_args(argc, argv);
	if (args == NULL)
		return (error(), 0);
	forks = malloc(args[0] * sizeof(char));
	threads = malloc(args[0] * sizeof(pthread_t));
	memset(forks, 'a', (args[0] * sizeof(char)));
	if (args[0] == 1)
		return (printf("1 died\n"), free(threads), free(args), free(forks), 0);
	make_threads(threads, args, forks, 0);
	make_threads(threads, args, forks, 1);
	return (death_check(args), free(args), free(forks), 0);
}
