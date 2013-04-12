SRC = mrfstereo.cpp runalgs.cpp
BIN = mrfstereo
TESTS = tsukuba venus teddy
CC = g++
WARN   = -W -Wall
OPT    ?= -O3
CPPFLAGS = $(OPT) $(WARN) -I../MRF -I../imageLib
LDLIBS = -L../MRF -lMRF -L../imageLib -lImg -lpng -lz

all: $(BIN)

mrfstereo: mrfstereo.o runalgs.o

clean: 
	rm -f $(BIN) *.o *.exe core *.stackdump *.bak

runclean:
	for t in $(TESTS); do (rm -rf results_$$t; mkdir results_$$t) ; done

run: $(BIN) runclean
	(cd results_tsukuba; ../mrfstereo -a 9 -b -g 8 -o 16 -w -x \
	 ../data/tsukuba-imL.png ../data/tsukuba-imR.png tsu.png)
	(cd results_venus; ../mrfstereo -a 9 -b -n 20 -o 8 -s -t 16 -e 2 -m 7 -l 50 -w -x \
	 ../data/venus-imL.png ../data/venus-imR.png ven.png)
	(cd results_teddy;  ../mrfstereo -a 9 -b -n 60 -o 4 -t 16 -g 10 -p 3 -e 1 -m 1 -l 10 -w -x \
	 ../data/teddy-imL.png ../data/teddy-imR.png ted.png)

depend:
	@makedepend -Y -- $(CPPFLAGS) -- $(SRC) 2>> /dev/null

# DO NOT DELETE THIS LINE -- make depend depends on it.

mrfstereo.o: ../imageLib/imageLib.h ../imageLib/Error.h ../imageLib/Image.h
mrfstereo.o: ../imageLib/RefCntMem.h ../imageLib/ImageIO.h
mrfstereo.o: ../imageLib/Convert.h ../MRF/mrf.h ../MRF/ICM.h
mrfstereo.o: ../MRF/LinkedBlockList.h ../MRF/GCoptimization.h ../MRF/graph.h
mrfstereo.o: ../MRF/block.h ../MRF/energy.h ../MRF/MaxProdBP.h
mrfstereo.o: ../MRF/regions-new.h ../MRF/TRW-S.h ../MRF/BP-S.h runalgs.h
runalgs.o: ../imageLib/imageLib.h ../imageLib/Error.h ../imageLib/Image.h
runalgs.o: ../imageLib/RefCntMem.h ../imageLib/ImageIO.h
runalgs.o: ../imageLib/Convert.h ../MRF/mrf.h runalgs.h
