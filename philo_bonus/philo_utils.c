#include "philo.h"

int	ft_atoi(char *str);
int	check_args(char **av, int ac, t_philo *philo);

int	is_num(char **av)
{
	int	i;
	int	j;

	j = 0;
	i = -1;
	while (av[++i])
	{	
		j = -1;
		while (av[i][++j])
		{
			if (!(av[i][j] >= '0' && av[i][j] <= '9'))
			       return (0);	
		}
	}
	return (1);
}

int	check_args(char **av, int ac, t_philo *philo)	
{
	int	i;
	int	*args;

	if (!is_num(av))
		return (0);
	args = malloc(sizeof(int) * (ac - 1));
	if (!args)
		return (0);
	i = -1;
	while (av[++i])
		args[i] = ft_atoi(av[i]);
	i = -1;
	while (av[++i])
	{
		if (args[i] <= 0)
			return (free(args), 0);
	}
	philo->info = args;
	return (1);
}	

int	ft_atoi(char *str)
{
	int		i;
	int		arb;
	int		res;
	arb = 1;
	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f'
				|| str[i] == '\t' || str[i] == '\r'))
		i++;	
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			arb *= -1;
	res = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		res = res * 10 + (str[i] - 48);
		i++;
	}
	return (res * arb);
}
