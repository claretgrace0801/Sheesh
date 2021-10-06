all: shell clear run

shell: compilation
	gcc -g -o shell main.o helpers.o prompt.o parser.o execution.o test.o cd.o echo.o pinfo.o ls.o repeat.o history.o process.o
 
compilation:
	gcc -c -Wall -Wextra main.c helpers.c test.c prompt.c parser.c execution.c cd.c echo.c pinfo.c ls.c repeat.c history.c process.c

clear:
	rm main.o helpers.o test.o prompt.o parser.o execution.o cd.o echo.o pinfo.o ls.o repeat.o history.o process.o

run:
	./shell

