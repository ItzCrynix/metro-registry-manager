FROM gcc

WORKDIR /src

COPY ./internal .
COPY estacoes.csv .
RUN gcc ./*.c ./structs/*.c ./utils/*.c -o main -w

CMD ["/src/main"]