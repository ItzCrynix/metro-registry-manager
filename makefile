run:
	./main

all:
	gcc ./internal/*.c ./internal/structs/*.c -o main -w

clean:
	rm *.o *.exe *.out