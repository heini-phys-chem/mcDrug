CXX = g++

OBDIR = /home/stefan/openbabel

INCLUDE = -I$(OBDIR)/include/openbabel-2.0/
LIBS = -L$(OBDIR)/lib
CXX_FLAGS = -std=c++11 -O3 -march=native -Wall
LINKER_FLAGS = -lopenbabel

all: main

main: mcGrow.o mcDock.o mcGif.o src/mcDrug.cpp
	$(CXX) $(INCLUDE) $(LIBS) $(CXX_FLAGS) src/mcDrug.cpp mcGrow.o mcDock.o mcGif.o -o main $(LINKER_FLAGS)

main.o: main.o
	$(CXX) $(INCLUDE) $(LIBS) $(CXX_FLAGS) -c src/mcDrug.cpp $(LINKER_FLAGS)

mcGrow.o: src/mcFunc.hpp src/mcGrow.cpp
	$(CXX) $(INCLUDE) $(LIBS) $(CXX_FLAGS) -c src/mcGrow.cpp $(LINKER_FLAGS)

mcDock.o: src/mcFunc.hpp src/mcDock.cpp src/utils.hpp
	$(CXX) $(INCLUDE) $(LIBS) $(CXX_FLAGS) -c src/mcDock.cpp $(LINKER_FLAGS)

mcGif.o: src/mcFunc.hpp src/mcGif.cpp
	$(CXX) $(INCLUDE) $(LIBS) $(CXX_FLAGS) -c src/mcGif.cpp $(LINKER_FLAGS)

clean:
	rm -f main
	rm -f *.o
	rm -f *.gch
	rm -f gif/all* gif/test.xyz gif/conformers.xyz gif/min.xyz gif/out.xyz

clean_gif:
	rm -f gif/all* gif/test.xyz gif/conformers.xyz gif/min.xyz gif/out.xyz
