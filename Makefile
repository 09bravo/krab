gcc:
	gcc main.c -o eonsh -Wall -Wextra -pedantic

install: 
	su -c 'mv eonsh /bin/eonsh'
uninstall:
	su -c 'rm -Rf /bin/eonsh'
