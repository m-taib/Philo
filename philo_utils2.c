#include "philo.h"

long	get_time(void)
{
	struct timeval curr_time;

	gettimeofday(&curr_time, NULL);
	return (curr_time.tv_sec * 1000 + curr_time.tv_usec / 1000);
}

long	ft_interval(time_t before, time_t after)
{
	return (after - before);
}

void	ft_usleep(t_philo *vars, int time)
{
	time_t	waiting_until;

	waiting_until = get_time() + time;
	while (waiting_until > get_time() && !vars->is_dead)
		usleep(50);
}

