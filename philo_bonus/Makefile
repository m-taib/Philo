SRC = philo_bonus.c philo_utils.c philo_utils2.c
OSRC = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror  

NAME = philo_bonus
%.o : %.c philo.h
	cc -c $(CFLAGS) $<

all : $(NAME)
	
$(NAME) : $(OSRC)
	cc -pthread $(CFLAGS) $^ -o $(NAME)

fclean : 
	@rm -rf *.o $(NAME)
re : fclean all
