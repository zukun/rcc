SRC = familyComposite.cpp familySeamFinder.cpp image.cpp pano.cpp pngio.cpp seamFinder.cpp
BIN = familyComposite pano
TESTS = family pano

CC = g++
WARN = -W -Wall
OPT ?= -O3
CPPFLAGS = $(OPT) $(WARN) -I../MRF -I../imageLib -I../mrfstereo
LDLIBS = -L../MRF -lMRF -L../imageLib -lImg -lpng -lz

all: $(BIN)

familyComposite: seamFinder.o pngio.o image.o familySeamFinder.o ../mrfstereo/runalgs.o
pano: seamFinder.o pngio.o image.o ../mrfstereo/runalgs.o

clean: 
	rm -f $(BIN) *.o *.exe core *.stackdump *.bak

runclean:
	for t in $(TESTS); do (rm -rf results_$$t; mkdir results_$$t) ; done

run: $(BIN) runclean
	(cd results_family; ../familyComposite ../data_family/familydatacost.png 5 ../data_family/small*png)
	(cd results_pano; ../pano 7 ../data_pano/*PNG)

depend:
	@makedepend -Y -- $(CPPFLAGS) -- $(SRC) 2>> /dev/null

# DO NOT DELETE THIS LINE -- make depend depends on it.

familyComposite.o: pngio.h image.h coord.h familySeamFinder.h seamFinder.h
familyComposite.o: includes.h ../MRF/energy.h ../MRF/graph.h ../MRF/block.h
familyComposite.o: ../MRF/mrf.h globals.h
familySeamFinder.o: familySeamFinder.h seamFinder.h image.h coord.h pngio.h
familySeamFinder.o: includes.h ../MRF/energy.h ../MRF/graph.h ../MRF/block.h
familySeamFinder.o: ../MRF/mrf.h globals.h ../MRF/ICM.h
familySeamFinder.o: ../MRF/LinkedBlockList.h ../MRF/GCoptimization.h
familySeamFinder.o: ../MRF/MaxProdBP.h ../MRF/regions-new.h ../MRF/TRW-S.h
familySeamFinder.o: ../MRF/BP-S.h
image.o: image.h coord.h pngio.h
pano.o: pngio.h image.h coord.h seamFinder.h includes.h ../MRF/energy.h
pano.o: ../MRF/graph.h ../MRF/block.h ../MRF/mrf.h globals.h
pngio.o: pngio.h
seamFinder.o: seamFinder.h image.h coord.h pngio.h includes.h ../MRF/energy.h
seamFinder.o: ../MRF/graph.h ../MRF/block.h ../MRF/mrf.h globals.h
seamFinder.o: ../MRF/ICM.h ../MRF/LinkedBlockList.h ../MRF/GCoptimization.h
seamFinder.o: ../MRF/MaxProdBP.h ../MRF/regions-new.h ../MRF/TRW-S.h
seamFinder.o: ../MRF/BP-S.h ../mrfstereo/runalgs.h
