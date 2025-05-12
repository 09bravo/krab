gcc:
	gcc main.c -o dish -Wall -Wextra -pedantic

install: 
	su -c 'mv dish /bin/dish'
uninstall:
	su -c 'rm -Rf /bin/dish'
