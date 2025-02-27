NAME = philo

src = philo.c

flags = -Wall -Wextra -Werror

obj = ${src:.c=.o}

all: ${NAME}

${NAME}: ${obj}
	cc ${flags} ${src} -o ${NAME}

clean:
	rm -f ${obj}

fclean: clean
	rm -f ${NAME}

ra: all clean