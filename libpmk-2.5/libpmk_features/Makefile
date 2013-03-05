CC = g++
CFLAGS = -Wall -O3 -Wno-deprecated `Magick++-config --cppflags --cxxflags`
LDFLAGS = -lm -lstdc++ `Magick++-config --libs --ldflags`

# Fill in the following two lines:
LIBPMK_OBJ_PATH = ../libpmk2
LIBPMK_INC_PATH = ../libpmk2

LIBPMK_FEATURES_NAME = libpmk_features.o

LIBPMK_OBJS = $(LIBPMK_OBJ_PATH)/libpmk.o $(LIBPMK_OBJ_PATH)/libpmk_util.o
INC = -I$(LIBPMK_INC_PATH) -I.


BASE_OBJ = \
	core/interest-point.o \
	images/image-loader.o

DETECTOR_OBJ = \
	detector/detector.o \
	detector/grid-detector.o \
	detector/filtering-grid-detector.o \
	detector/image-detector.o \
	detector/harris-detector.o \
	detector/mser-detector.o \
	detector/surf-detector.o

EXTRACTOR_OBJ = \
	extractor/extractor.o \
	extractor/image-extractor.o \
	extractor/location-extractor.o \
	extractor/oxford-vgg-extractor.o \
	extractor/sift-extractor.o \
	extractor/shape-context-extractor.o \
	extractor/steerable-filter-extractor.o \
	extractor/surf-extractor.o

ALL_OBJS = \
	$(BASE_OBJ) \
	$(DETECTOR_OBJ) \
	$(EXTRACTOR_OBJ)

TOOLS = \
	tools/images-to-pointsets.out

objs: $(ALL_OBJS)
library: $(ALL_OBJS)
	$(LD) -r -o $(LIBPMK_FEATURES_NAME) $(ALL_OBJS)
tools: $(ALL_OBJS) $(TOOLS)
clean:
	rm -f $(ALL_OBJS)
	rm -f $(TOOLS)
	rm -f $(LIBPMK_FEATURES_NAME)

.cc.out:
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) $(LIBPMK_OBJS) $(ALL_OBJS) $< -o $@
.cc.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@