run:
	./main

all:
	gcc ./internal/funcionalidades/*.c ./internal/structs/*.c ./internal/utils/*.c ./internal/*.c -o main -w

clean:
	rm *.o *.exe *.bin main