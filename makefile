run:
	./main

all:
	gcc ./internal/*.c -o main -w

clean:
	rm *.o *.exe *.out