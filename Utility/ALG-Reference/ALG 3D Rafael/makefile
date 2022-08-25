#==============================================================================#
#                                                                              #
#                            Makefile ALG-3D                                   #
#                                                                              #
#  Author: Maycon da Costa                                                     #
#  Last update:  july 13th, 2009                                               #
#                                                                              #
#==============================================================================#

CPP    = g++
CFLAGS = -O3 -Wall

#ALG directory tree.
ALG       = source/alg
EXCEPTION = $(ALG)/exceptions
GRID      = $(ALG)/grid
CELL      = $(GRID)/cell
MODEL_DIR = source/models

#Model File
MODEL_CPP	   = $(MODEL_DIR)/luo_rudy.cpp

#Object files.
TIME_OBJ = $(OBJ_DIR)/StopWatch.o
MODELS_OBJ	   = $(OBJ_DIR)/cell_model.o
EXCEPTIONS_OBJ = $(OBJ_DIR)/exceptions.o $(OBJ_DIR)/invalidArgumentValue.o $(OBJ_DIR)/nullPointer.o
CELL_OBJ       = $(OBJ_DIR)/cell.o $(OBJ_DIR)/cell_refinement.o $(OBJ_DIR)/cell_derefinement.o $(OBJ_DIR)/cell_draw.o $(OBJ_DIR)/cell_flux.o $(OBJ_DIR)/element.o
GRID_OBJ       = $(OBJ_DIR)/grid.o $(OBJ_DIR)/grid_refinement.o $(OBJ_DIR)/grid_derefinement.o $(OBJ_DIR)/grid_flux.o $(OBJ_DIR)/grid_domain.o $(OBJ_DIR)/grid_wave.o
OBJ            = $(MODELS_OBJ) $(EXCEPTIONS_OBJ) $(CELL_OBJ) $(GRID_OBJ) $(TIME_OBJ) $(OBJ_DIR)/main.o

#Libraries required.
LIBS = -I/usr/include/ -lm

#Executable file name.
EXECUTABLE = alg

#Directory used to store the project executable file.
BIN = bin

#Directory used to store object files.
OBJ_DIR = obj

all: $(EXECUTABLE)

$(EXECUTABLE): beforeBuild $(OBJ) afterBuild
	$(CPP) $(CFLAGS) $(OBJ_DIR)/*.o $(LIBS) -o $(BIN)/$(EXECUTABLE)
	@echo ' '
	@echo 'Project builted successfuly!'
	@echo ' '
	@echo ' '


#----------------------------------MAIN--------------------------------
$(OBJ_DIR)/main.o: source/main.cpp
	$(CPP) $(CFLAGS) -c source/main.cpp -o $(OBJ_DIR)/main.o

#----------------------------------MODEL--------------------------------
$(OBJ_DIR)/cell_model.o: $(MODEL_CPP)
	$(CPP) $(CFLAGS) -c $(MODEL_CPP) -o $(OBJ_DIR)/cell_model.o

#----------------------------------TIME--------------------------------
$(TIME_OBJ): $(GRID)/StopWatch.cpp
	$(CPP) $(CFLAGS) -c $(GRID)/StopWatch.cpp -o $(OBJ_DIR)/StopWatch.o

#----------------------------------EXCEPTIONS--------------------------------
$(OBJ_DIR)/exceptions.o: $(EXCEPTION)/exceptions.cpp
	$(CPP) $(CFLAGS) -c $(EXCEPTION)/exceptions.cpp -o $(OBJ_DIR)/exceptions.o

$(OBJ_DIR)/invalidArgumentValue.o: $(OBJ_DIR)/exceptions.o $(EXCEPTION)/invalidArgumentValue.cpp
	$(CPP) $(CFLAGS) -c $(EXCEPTION)/invalidArgumentValue.cpp -o $(OBJ_DIR)/invalidArgumentValue.o

$(OBJ_DIR)/nullPointer.o: $(OBJ_DIR)/exceptions.o $(EXCEPTION)/nullPointer.cpp
	$(CPP) $(CFLAGS) -c $(EXCEPTION)/nullPointer.cpp -o $(OBJ_DIR)/nullPointer.o


#------------------------------------CELLS-------------------------------------
$(OBJ_DIR)/element.o:  $(CELL)/element.cpp $(EXCEPTIONS_OBJ)
	$(CPP) $(CFLAGS) -c $(CELL)/element.cpp -o $(OBJ_DIR)/element.o

$(OBJ_DIR)/cell.o:  $(CELL)/cell.cpp $(EXCEPTIONS_OBJ)
	$(CPP) $(CFLAGS) -c $(CELL)/cell.cpp -o $(OBJ_DIR)/cell.o

$(OBJ_DIR)/cell_refinement.o: $(CELL)/cell_refinement.cpp $(OBJ_DIR)/cell.o
	$(CPP) $(CFLAGS) -c $(CELL)/cell_refinement.cpp -o $(OBJ_DIR)/cell_refinement.o

$(OBJ_DIR)/cell_derefinement.o: $(CELL)/cell_derefinement.cpp $(OBJ_DIR)/cell.o
	$(CPP) $(CFLAGS) -c $(CELL)/cell_derefinement.cpp -o $(OBJ_DIR)/cell_derefinement.o

$(OBJ_DIR)/cell_flux.o: $(CELL)/cell_flux.cpp $(OBJ_DIR)/cell.o
	$(CPP) $(CFLAGS) -c $(CELL)/cell_flux.cpp -o $(OBJ_DIR)/cell_flux.o

$(OBJ_DIR)/cell_draw.o: $(CELL)/cell_draw.cpp $(OBJ_DIR)/cell.o
	$(CPP) $(CFLAGS) -c $(CELL)/cell_draw.cpp -o $(OBJ_DIR)/cell_draw.o
#-------------------------------------GRID------------------------------------
$(OBJ_DIR)/grid.o: $(GRID)/grid.cpp $(CELL_OBJ)
	$(CPP) $(CFLAGS) -c $(GRID)/grid.cpp -o $(OBJ_DIR)/grid.o

$(OBJ_DIR)/grid_refinement.o: $(OBJ_DIR)/grid.o $(GRID)/grid_refinement.cpp
	$(CPP) $(CFLAGS) -c $(GRID)/grid_refinement.cpp -o $(OBJ_DIR)/grid_refinement.o

$(OBJ_DIR)/grid_derefinement.o: $(OBJ_DIR)/grid.o $(GRID)/grid_derefinement.cpp
	$(CPP) $(CFLAGS) -c $(GRID)/grid_derefinement.cpp -o $(OBJ_DIR)/grid_derefinement.o

$(OBJ_DIR)/grid_flux.o: $(OBJ_DIR)/grid.o $(GRID)/grid_flux.cpp
	$(CPP) $(CFLAGS) -c $(GRID)/grid_flux.cpp -o $(OBJ_DIR)/grid_flux.o

$(OBJ_DIR)/grid_domain.o: $(OBJ_DIR)/grid.o $(GRID)/grid_domain.cpp
	$(CPP) $(CFLAGS) -c $(GRID)/grid_domain.cpp -o $(OBJ_DIR)/grid_domain.o

$(OBJ_DIR)/grid_wave.o: $(OBJ_DIR)/grid.o
	$(CPP) $(CFLAGS) -c $(GRID)/grid_wave.cpp -o $(OBJ_DIR)/grid_wave.o



#-----------------------------------------------------------------------------
beforeBuild:
	@echo '------------- ALG-3D BUILD process --------------'
	@echo ' '
	@echo 'Creating directory $(BIN) and $(OBJ_DIR):'
	mkdir -m 777 -p $(BIN)
	mkdir -m 777 -p $(OBJ_DIR)
	@echo ' '
	@echo 'Compiling source files:'

afterBuild:
	@echo ' '
	@echo 'Linking object files:'

clean:
#	clear
	@echo 'Cleaning project:'
	rm -rf $(OBJ_DIR) $(BIN) $(CELL)/*~ $(GRID)/*~ $(ALG)/*~ $(EXCEPTION)/*~
	@echo ' '
	@echo 'Project cleaned successfuly!'
	@echo ' '

run: $(EXECUTABLE)
	./$(BIN)/$(EXECUTABLE)

doc:
	@echo 'Creating documentation:'
	doxygen

tar:
	tar -czf alg3d.tar.gz $(ALG) $(SYSTEM) $(INTERFACE) makefile

zip:
	zip alg3d.zip -qr $(ALG) $(SYSTEM) $(INTERFACE) makefile


