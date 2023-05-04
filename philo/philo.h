#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct philo
{
	int	*i;
	int 	*info;
	int	*nb_to_eat;
	int	is_dead;
	long	init_time;
	long	eating_time[200];
	pthread_t ptr_t[200];
	pthread_mutex_t ptr_m[200];
	
} t_philo;

void    ft_usleep(t_philo *vars, int time);
int	is_num(char **av);
int	ft_atoi(char *str);
int     check_args(char **av, int ac, t_philo *philo);
void    *philo_routine(void *philo);
void	philos_create(t_philo *ptr);
void	ft_clear(t_philo *philo);
int	is_full(t_philo *philo);
void	supervisor(t_philo *philo);
int	init(t_philo *philo);
long	get_time(void);
long	ft_interval(time_t before, time_t after);

enum
{
	philo_n, 
	time_to_die ,
	time_to_eat ,
	time_to_sleep ,
	ntp_must_eat
};
#endif
