program:	main.c funkcje.c
	cc -X -c funkcje.c
	cc -X -c main.c
	cc main.o funkcje.o -lm

clean: 
		rm -f *.o
