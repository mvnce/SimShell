CPPFLAGS=-std=c++11 -g -O3

Shell: main.o
	    g++ main.o -o Shell

main.o: main.c
	g++ $(CPPFLAGS) -c main.c

clean:
	rm *o Shell
