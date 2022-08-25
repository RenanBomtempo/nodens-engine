CC = gcc
CFLAGS = -Wall -g -lpthread 
GLFLAGS = -I/usr/include/GL -lglut -lGL -lm
EXENAME = linuxesspp

$(EXENAME): Cell.o interpolacao.o solve.o pressao.o momento.o energia.o matriz.o fronteira.o render.o refinement.o graphics.o derivadas.o Element.o Grid.o main.o
	$(CC) $(CFLAGS) Cell.o interpolacao.o solve.o pressao.o momento.o energia.o matriz.o fronteira.o render.o refinement.o graphics.o derivadas.o Element.o Grid.o main.o -o $(EXENAME) $(GLFLAGS)

Cell.o: Cell.cpp
	$(CC) $(CFLAGS) -c Cell.cpp -o Cell.o

interpolacao.o: interpolacao.cpp
	$(CC) $(CFLAGS) -c interpolacao.cpp -o interpolacao.o

solve.o: solve.cpp
	$(CC) $(CFLAGS) -c solve.cpp -o solve.o

pressao.o: pressao.cpp
	$(CC) $(CFLAGS) -c pressao.cpp -o pressao.o

momento.o: momento.cpp
	$(CC) $(CFLAGS) -c momento.cpp -o momento.o

energia.o: energia.cpp
	$(CC) $(CFLAGS) -c energia.cpp -o energia.o

matriz.o: matriz.cpp
	$(CC) $(CFLAGS) -c matriz.cpp -o matriz.o

fronteira.o: fronteira.cpp
	$(CC) $(CFLAGS) -c fronteira.cpp -o fronteira.o

render.o: render.cpp
	$(CC) $(CFLAGS) -c render.cpp -o render.o

refinement.o: refinement.cpp
	$(CC) $(CFLAGS) -c refinement.cpp -o refinement.o

graphics.o: graphics.cpp
	$(CC) $(CFLAGS) -c graphics.cpp -o graphics.o

derivadas.o: derivadas.cpp
	$(CC) $(CFLAGS) -c derivadas.cpp -o derivadas.o

Element.o: Element.cpp
	$(CC) $(CFLAGS) -c Element.cpp -o Element.o

Grid.o: Grid.cpp
	$(CC) $(CFLAGS) -c Grid.cpp -o Grid.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

run: $(EXENAME)
	./$(EXENAME)

compile: $(EXENAME)

build:
	make clean
	make compile

clean clear:
	rm *.o $(EXENAME)
