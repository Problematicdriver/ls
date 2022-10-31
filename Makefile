objects = cmp.c print.c ls.c

ls : $(objects)
	cc -Wall -Werror -Wextra -o ls $(objects)

clean:
	rm ls
