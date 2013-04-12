SRC = denoise.cpp
BIN = denoise
TESTS = house penguin

CC = g++
WARN = -W -Wall
OPT ?= -O3
CPPFLAGS = $(OPT) $(WARN) -I../MRF -I../imageLib -I../mrfstereo
LDLIBS = -L../MRF -lMRF -L../imageLib -lImg -lpng -lz

all: $(BIN)

denoise: denoise.o ../mrfstereo/runalgs.o

clean: 
	rm -f $(BIN) *.o *.exe core *.stackdump *.bak

runclean:
	for t in $(TESTS); do (rm -rf results_$$t; mkdir results_$$t) ; done

run: $(BIN) runclean
	(cd results_house;   ../denoise 5  0   ../data/house-input.png   house   ../data/house-mask.png)
	(cd results_penguin; ../denoise 25 200 ../data/penguin-input.png penguin ../data/penguin-mask.png)

depend:
	@makedepend -Y -- $(CPPFLAGS) -- $(SRC) 2>> /dev/null

# DO NOT DELETE THIS LINE -- make depend depends on it.

denoise.o: ../imageLib/imageLib.h ../imageLib/Error.h ../imageLib/Image.h
denoise.o: ../imageLib/RefCntMem.h ../imageLib/ImageIO.h
denoise.o: ../imageLib/Convert.h ../MRF/mrf.h ../MRF/ICM.h
denoise.o: ../MRF/LinkedBlockList.h ../MRF/GCoptimization.h ../MRF/graph.h
denoise.o: ../MRF/block.h ../MRF/energy.h ../MRF/MaxProdBP.h
denoise.o: ../MRF/regions-new.h ../MRF/TRW-S.h ../MRF/BP-S.h
denoise.o: ../mrfstereo/runalgs.h
