CC = g++
LD = ld
CFLAGS = -Wall -Wno-deprecated -O3
LDFLAGS = -lm
LIBPMK_NAME = ../libpmk2/libpmk.o
LIBPMK_UTIL_NAME = ../libpmk2/libpmk_util.o

INC = -I. -I../libpmk2

SPATIAL_OBJ = \
	spatial/channelizer.o \
	spatial/spatial-pyramid-maker.o \
	spatial/spatial-pyramid-reader.o \
	spatial/spatial-pyramid-writer.o \
	spatial/normalized-spatial-pyramid-maker.o

TOOLS = \
	tools/channelize-point-set-list.out \
	tools/pointsets-to-spatial-pyramids.out \
	tools/pointsets-to-normalized-spatial-pyramids.out \
	tools/pyramids-to-kernel.out \
	tools/split-into-channels.out

UNITTESTS = \
	spatial/channelizer_unittest.out

ALLOBJS = $(SPATIAL_OBJ)
LIBPMK_OBJS = $(LIBPMK_NAME) $(LIBPMK_UTIL_NAME)

objs: $(ALLOBJS)
tests: $(ALLOBJS) $(UNITTESTS)
tools: $(ALLOBJS) $(TOOLS)
diag: $(ALLOBJS) $(DIAGNOSTIC)

.cc.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.cc.out:
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) $(ALLOBJS) $(LIBPMK_OBJS) $< -o $@

clean:
	rm -f $(ALLOBJS)
	rm -f $(UNITTESTS)
	rm -f $(TOOLS)
	rm -f $(DIAGNOSTIC)

reallyclean: clean
	find . -name "*~" | xargs rm
	find . -name "semantic.cache" | xargs rm