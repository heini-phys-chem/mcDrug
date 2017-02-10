CXX = g++

OBDIR = /home/stefan/openbabel

INCLUDE = -I$(OBDIR)/include/openbabel-2.0/
LIBS = -L$(OBDIR)/lib

all: main

main: mcGrow.o mcDock.o mcGif.o mcDrug.cpp
	$(CXX) $(INCLUDE) $(LIBS) -std=c++11 -O3 -march=native mcDrug.cpp mcGrow.o mcDock.o mcGif.o -o main -lopenbabel

main.o: main.o
	$(CXX) $(INCLUDE) $(LIBS) -std=c++11 -O3 -march=native -c mcDrug.cpp -lopenbabel

mcGrow.o: mcFunc.hpp mcGrow.cpp
	$(CXX) $(INCLUDE) $(LIBS) -std=c++11 -O3 -march=native -c mcGrow.cpp -lopenbabel

mcDock.o: mcFunc.hpp mcDock.cpp utils.hpp
	$(CXX) $(INCLUDE) $(LIBS) -std=c++11 -O3 -march=native -c mcDock.cpp -lopenbabel

mcGif.o: mcFunc.hpp mcGif.cpp
	$(CXX) $(INCLUDE) $(LIBS) -std=c++11 -O3 -march=native -c mcGif.cpp -lopenbabel

clean:
	rm -f main
	rm -f *.o
	rm -f *.gch
	rm -f gif/all* gif/test.xyz gif/min/.xyz gif/min.xyz

clean_gif:
	rm -f gif/all* gif/test.xyz gif/min/.xyz gif/min.xyz
