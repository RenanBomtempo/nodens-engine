CC = gcc
CFLAGS = -Wall -g
GLFLAGS = -I/usr/include/GL -lglut -lGL -lm
EXENAME = linuxesspp

$(EXENAME): Cell.o solve.o render.o finitedifference.o faces.o refinement.o derefinement.o graphics.o Element.o Grid.o main.o
	$(CC) $(CFLAGS) Cell.o solve.o render.o finitedifference.o faces.o refinement.o derefinement.o graphics.o Element.o Grid.o main.o -o $(EXENAME) $(GLFLAGS)

Cell.o: Cell.cpp
	$(CC) $(CFLAGS) -c Cell.cpp -o Cell.o

solve.o: solve.cpp
	$(CC) $(CFLAGS) -c solve.cpp -o solve.o

render.o: render.cpp
	$(CC) $(CFLAGS) -c render.cpp -o render.o

finitedifference.o: finitedifference.cpp
	$(CC) $(CFLAGS) -c finitedifference.cpp -o finitedifference.o

faces.o: faces.cpp
	$(CC) $(CFLAGS) -c faces.cpp -o faces.o

refinement.o: refinement.cpp
	$(CC) $(CFLAGS) -c refinement.cpp -o refinement.o

derefinement.o: derefinement.cpp
	$(CC) $(CFLAGS) -c derefinement.cpp -o derefinement.o

graphics.o: graphics.cpp
	$(CC) $(CFLAGS) -c graphics.cpp -o graphics.o

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
