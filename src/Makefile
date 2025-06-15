gcc:
	gcc main.c history.c input.c syntx.c raw_mode.c commands.c prompt.c -o krab -Wall -Wextra -pedantic

install: 
	su -c 'mv krab /bin/krab'
uninstall:
	su -c 'rm -Rf /bin/krab'
