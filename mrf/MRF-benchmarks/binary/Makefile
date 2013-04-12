SRC = binary.cpp
BIN = binary
TESTS = flower sponge person

CC = g++
WARN = -W -Wall
OPT ?= -O3
CPPFLAGS = $(OPT) $(WARN) -I../MRF -I../imageLib -I../mrfstereo
LDLIBS = -L../MRF -lMRF -L../imageLib -lImg -lpng -lz

all: $(BIN)

binary: binary.o ../mrfstereo/runalgs.o

clean: 
	rm -f $(BIN) *.o *.exe core *.stackdump *.bak

runclean:
	for t in $(TESTS); do (rm -rf results_$$t; mkdir results_$$t) ; done

run: $(BIN) runclean
	for t in $(TESTS); do (cd results_$$t; ../binary ../data/$$t.txt $$t) ; done

depend:
	@makedepend -Y -- $(CPPFLAGS) -- $(SRC) 2>> /dev/null

# DO NOT DELETE THIS LINE -- make depend depends on it.

binary.o: ../imageLib/imageLib.h ../imageLib/Error.h ../imageLib/Image.h
binary.o: ../imageLib/RefCntMem.h ../imageLib/ImageIO.h ../imageLib/Convert.h
binary.o: ../MRF/mrf.h ../MRF/ICM.h ../MRF/LinkedBlockList.h
binary.o: ../MRF/GCoptimization.h ../MRF/graph.h ../MRF/block.h
binary.o: ../MRF/energy.h ../MRF/MaxProdBP.h ../MRF/regions-new.h
binary.o: ../MRF/TRW-S.h ../MRF/BP-S.h ../mrfstereo/runalgs.h
