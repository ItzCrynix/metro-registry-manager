FROM gcc

WORKDIR /src

COPY ./internal .
COPY estacoes.csv .
RUN gcc *c -o main

CMD ["/src/main"]