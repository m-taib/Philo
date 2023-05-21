#include "philo.h"

void    *philo_routine(void *philo)
{
	t_philo *arg;
	int	i;

	arg = (t_philo *)philo;
	pthread_mutex_lock(arg->assign);
	i = (arg->i);
	pthread_mutex_unlock(arg->assign);
	if (i % 2 == 1)
		usleep(100);
	//printf("%d\n",i);
	//return NULL;
	while (1)
	{
		pthread_mutex_lock(arg->dead);
		if (arg->is_dead)
		{
			pthread_mutex_unlock(arg->dead);
			return (0);
		}
		pthread_mutex_unlock(arg->dead);
		printf("%ld %d is thinking\n",
			ft_interval(arg->init_time, get_time()), i + 1);
		pthread_mutex_lock(&arg->ptr_m[i]);
		printf("%ld %d has taken a fork\n",
			ft_interval(arg->init_time, get_time()), i + 1);
		if (arg->info[philo_n] > 1)
		{
			pthread_mutex_lock(&arg->ptr_m[(i + 1) % arg->info[philo_n]]);
			printf("%ld %d has taken a fork\n",
				ft_interval(arg->init_time, get_time()), i + 1);
			printf("%ld %d is eating\n",
				ft_interval(arg->init_time, get_time()), i + 1);
			ft_usleep(philo, arg->info[time_to_eat]);
			//usleep(arg->info[time_to_eat] * 1000);
			if (arg->nb_to_eat)
				arg->nb_to_eat[i]++;
			arg->eating_time[i] = get_time();
			pthread_mutex_unlock(&arg->ptr_m[i]);
			pthread_mutex_unlock(&arg->ptr_m[(i + 1) % arg->info[philo_n]]);
			printf("%ld %d is sleeping\n",
				ft_interval(arg->init_time, get_time()), i + 1);
			ft_usleep(philo, arg->info[time_to_sleep]);
			//usleep(arg->info[time_to_sleep] * 1000);
		}	
	}
	return (NULL);
}

void	philos_create(t_philo *philo)
{
	int	i;
	int n_of_philo;
	philo->init_time = get_time();
	i = -1;
	/*philo->eating_time = malloc(sizeof(long) * philo->info[philo_n]);
	if (!philo->eating_time)
		return ;
	*/while (philo->nb_to_eat && ++i < philo->info[philo_n])
		philo->nb_to_eat[i] = 0;
	i = -1;
	while (++i < philo->info[philo_n])
		philo->eating_time[i] = get_time();
	i = -1;
	philo->is_dead = 0;
	int	index;

	index = -1;
	n_of_philo =  philo->info[philo_n];

	while (++index < n_of_philo)
	{
		pthread_mutex_lock(philo->assign);
		philo->i = index;
		pthread_mutex_unlock(philo->assign);

		// pthread_mutex_lock(philo->n_of_philo);
		// index++;
		// pthread_mutex_unlock(philo->n_of_philo);
		
		if (pthread_create(&philo->ptr_t[index], NULL, philo_routine, philo))
			return ;
		// usleep(200);
	}
	supervisor(philo);
}

void	supervisor(t_philo *philo)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < philo->info[philo_n])
		{
			if (ft_interval(philo->eating_time[i], 
				get_time()) >= philo->info[time_to_die] || is_full(philo))
			{
				if (is_full(philo))
					printf("%ld every one ate\n",
						ft_interval(philo->init_time, get_time()));
				else
					printf("%ld %d died\n",
						ft_interval(philo->init_time, get_time()), i + 1);
				
				pthread_mutex_lock(philo->dead);
				philo->is_dead = 1;
				pthread_mutex_unlock(philo->dead);
				ft_clear(philo);
				return ;
			}
		}
		usleep(100);
	}

}

int	init(t_philo *philo)
{
	int	i;
	int	j;

	j = -1;
	/*philo->ptr_t = malloc(sizeof(pthread_t) * philo->info[philo_n]);
	if (!philo->ptr_t)
		return (0);
	philo->ptr_m = malloc(sizeof(pthread_mutex_t) * philo->info[philo_n]);
	if (!philo->ptr_m)
		return (0);
	*/
	philo->assign = malloc(sizeof(pthread_mutex_t));
	philo->dead = malloc(sizeof(pthread_mutex_t));
	philo->lte = malloc(sizeof(pthread_mutex_t));
	philo->n_of_meals = malloc(sizeof(pthread_mutex_t));
	philo->n_of_philo = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(philo->assign, NULL);
	pthread_mutex_init(philo->dead, NULL);
	pthread_mutex_init(philo->lte, NULL);
	pthread_mutex_init(philo->n_of_meals, NULL);
	pthread_mutex_init(philo->n_of_philo, NULL);
	i = -1;
	while (++i < philo->info[philo_n])
	{
		if (pthread_mutex_init((&(philo->ptr_m[i])), NULL))
		{
			while (++j < i)
				pthread_mutex_destroy(&(philo->ptr_m[j]));
			return (0);
		}
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_philo *philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (0);
	if (ac < 5 || ac > 6)
		return (1);
	if (!check_args(&av[1], ac, philo))
		return (free(philo),1);
	philo->nb_to_eat = NULL;
	if (ac == 6 && is_num(&av[5]) && (ft_atoi(av[5]) > 0))
	{
		philo->nb_to_eat = malloc(sizeof(int) * philo->info[philo_n]);
		if (!philo->nb_to_eat)
			return (0);
	}
	if (!init(philo))
	{
		if (philo->nb_to_eat)
			free(philo->nb_to_eat);
		return (free(philo->info), free(philo) ,1);
	}
	philos_create(philo);
	return (0);
}

