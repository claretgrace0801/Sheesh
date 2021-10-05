all: shell clear run

shell: compilation
	gcc -g -o shell main.o helpers.o prompt.o parser.o jobs.o test.o cd.o echo.o pinfo.o ls.o repeat.o history.o
 
compilation:
	gcc -c -Wall -Wextra main.c helpers.c test.c prompt.c parser.c jobs.c cd.c echo.c pinfo.c ls.c repeat.c history.c

clear:
	rm main.o helpers.o test.o prompt.o parser.o jobs.o cd.o echo.o pinfo.o ls.o repeat.o history.o

run:
	./shell

