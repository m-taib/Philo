#include "philo.h"

void	*supervisor(void *philo)
{
	t_philo *arg;
	int	i;

	i = -1;	
	arg = (t_philo *)philo;
	while (1)
	{
		if (ft_interval(arg->eating_time,
				get_time()) >= arg->info[time_to_die])
		{
			i++;
			if (!i)
			{
				printf("%ld %d died\n",
					ft_interval(arg->init_time, get_time()), arg->i + 1);
				sem_post(arg->sem_death);
				i = -1;
				while (++i < arg->info[philo_n])
					sem_post(arg->sem_eat);
			}
			//exit(1);
		}
		if (arg->info[ntp_must_eat] && arg->nb_to_eat >= arg->info[ntp_must_eat])
		{
			sem_post(arg->sem_eat);
			return (NULL);
		}
	}
	return (NULL);

}

void    philo_routine(t_philo *arg)
{
	int	i;
	
	i = arg->i;
	pthread_create(&arg->th, NULL, supervisor, arg);
	while (1)
	{
		printf("%ld %d is thinking\n",
			ft_interval(arg->init_time, get_time()), i + 1);
		sem_wait(arg->sem);
		printf("%ld %d has taken a fork\n",
			ft_interval(arg->init_time, get_time()), i + 1);
		if (arg->info[philo_n] > 1)
		{
			sem_wait(arg->sem);
			printf("%ld %d has taken a fork\n",
				ft_interval(arg->init_time, get_time()), i + 1);
			printf("%ld %d is eating\n",
				ft_interval(arg->init_time, get_time()), i + 1);
			ft_usleep(arg, arg->info[time_to_eat]);
			if (arg->info[ntp_must_eat])
				arg->nb_to_eat++;
			arg->eating_time = get_time();
			sem_post(arg->sem);
			sem_post(arg->sem);
			printf("%ld %d is sleeping\n",
				ft_interval(arg->init_time, get_time()), i + 1);
			ft_usleep(arg, arg->info[time_to_sleep]);
		}	
	}
}

void	philos_create(t_philo *philo)
{
	int	pid;
	int	i;

	sem_unlink("/FORKS");
	sem_unlink("/EAT");
	sem_unlink("/DEATH");
	sem_unlink("/PRINT");
	philo->sem = sem_open("/FORKS", O_CREAT | O_EXCL, 0644, philo->info[philo_n]);
	philo->sem_eat = sem_open("/EAT", O_CREAT | O_EXCL, 0644, 0);
	philo->sem_death = sem_open("/DEATH", O_CREAT | O_EXCL, 0644, 0);
	philo->sem_print = sem_open("/PRINT", O_CREAT | O_EXCL, 0644, 1);
	philo->init_time = get_time();
	philo->eating_time = get_time();
	philo->i = -1;
	while (++philo->i < philo->info[philo_n])
	{
		pid = fork();
		if (pid == -1)
			return ;
		if (pid == 0)
			philo_routine(philo);
		if (pid > 0)
			philo->ids[philo->i] = pid;
		
	}
	i = -1;
	if (philo->info[ntp_must_eat])
	{
		while (++i < philo->info[philo_n])
			sem_wait(philo->sem_eat);
	}
	else
		sem_wait(philo->sem_death);
	i = -1;
	while (++i < philo->info[philo_n])
		kill(philo->ids[i], 15);
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
	philo->nb_to_eat = 0;
	philo->info[ntp_must_eat] = 0;
	if (ac == 6 && is_num(&av[5]) && (ft_atoi(av[5]) > 0))
		philo->info[ntp_must_eat] = ft_atoi(av[5]);
	philos_create(philo);
}

