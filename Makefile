gcc:
	gcc main.c -o krab -Wall -Wextra -pedantic

install: 
	su -c 'mv krab /bin/krab'
uninstall:
	su -c 'rm -Rf /bin/krab'
