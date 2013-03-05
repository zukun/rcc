CC = g++
LD = ld
CFLAGS = -Wall -Wno-deprecated -O3  -I../../libpmk_features -I../../libpmk2 -I../ `Magick++-config --cppflags` `Magick++-config --cxxflags`
LDFLAGS = -lm `Magick++-config --libs --ldflags`
LIBPMK_OBJ = ../../libpmk2/libpmk.o
LIBPMK_UTIL_OBJ = ../../libpmk2/libpmk_util.o
LIBPMK_FEATURES_OBJ = ../../libpmk_features/libpmk_features.o
LIBPMK_ISM_OBJ = ../libpmk_ism.o

TOOLS = train-model.out \
	test-model.out

ALLOBJS = $(LIBPMK_OBJ) $(LIBPMK_UTIL_OBJ) $(LIBPMK_FEATURES_OBJ) $(LIBPMK_ISM_OBJ)

INC = -I.

tools: $(TOOLS)

.cc.out:
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) $(ALLOBJS) $(EXTRAOBJS) $< -o $@

clean:
	rm -f $(TOOLS)
