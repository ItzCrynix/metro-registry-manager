run:
	./main

all:
	gcc ./internal/structs/*.c ./internal/utils/*.c ./internal/filtro/*.c ./internal/busca/*.c ./internal/binario/*.c ./internal/*.c -o main -w



clean:
	rm *.o *.exe *.bin main