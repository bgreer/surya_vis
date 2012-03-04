
vis : main.o snapshot.o header.h
	gcc -o vis main.o snapshot.o header.h -lSDL -lm

main.o : main.c header.h
	gcc -c main.c header.h

snapshot.o : snapshot.c header.h
	gcc -c snapshot.c header.h


clean :
	rm *.o vis
