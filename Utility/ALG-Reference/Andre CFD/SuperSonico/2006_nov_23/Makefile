all:
	g++ -combine constants.h Cell.h Cell.cpp flux.cpp solve.cpp render.cpp refinement.cpp derefinement.cpp directionalGridVariables.cpp functions.h graphics.cpp finitedifference.cpp Element.h Element.cpp Grid.h Grid.cpp main.cpp -o linuxesspp -I/usr/include/GL -Wall -lglut -lGL -lm -g

clean:
	@echo Cleaning up...
	@rm lesspp
	@echo Done.
