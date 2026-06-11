run:
	./main

all:
	gcc ./internal/*.c ./internal/structs/*.c ./internal/utils/*.c -o main -w

clean:
	rm *.o *.exe *.out *.bin main