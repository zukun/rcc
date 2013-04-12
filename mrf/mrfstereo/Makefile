SRC = mrfstereo.cpp

CC = g++
CPPFLAGS = -O2 -W -Wall -I../MRF -I../imageLib
OBJ = $(SRC:.cpp=.o)
BIN = $(SRC:.cpp=)
EXE = $(SRC:.cpp=.exe)
LDLIBS = -L../MRF -lMRF -L../imageLib -lImg -lpng -lz

all: $(BIN)

clean: 
	rm -f $(OBJ) core *.stackdump

allclean: clean
	rm -f $(BIN) $(EXE)
