run:
	./main

all:
	gcc ./internal/structs/*.c ./internal/utils/*.c ./internal/busca/*.c ./internal/binario/*.c ./internal/*.c -o main -w

clean:
	rm *.o *.exe *.out *.bin main