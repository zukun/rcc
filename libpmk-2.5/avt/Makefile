CC = g++
LD = ld
CFLAGS = -Wall -Wno-deprecated -O3
LDFLAGS = -lm
LIBPMK_NAME = ../libpmk2/libpmk.o
LIBPMK_UTIL_NAME = ../libpmk2/libpmk_util.o
LIB_NAME = incremental_vtree.o

TREES_OBJ = \
        trees/inverted-file.o \
        trees/visual-word.o \
        trees/incremental-vocabulary-tree.o \
	trees/vocabulary-tree-retriever.o \
	trees/simple-pyramid-maker.o



TOOLS = \
        tools/grow-vocabulary-tree.out \
	tools/test-vocabulary-tree.out \
	tools/partial-pyramids.out

ALLOBJS = \
        ${TREES_OBJ}

LIBPMK = \
        ${LIBPMK_NAME} \
        ${LIBPMK_UTIL_NAME}


INC = -I. -I../libpmk2

objs: $(ALLOBJS)
tools: $(ALLOBJS) $(TOOLS)

library: $(ALLOBJS)
	$(LD) -r -o $(LIB_NAME) $(ALLOBJS)

.cc.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.cc.out:
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) $(ALLOBJS) ${LIBPMK} $< -o $@

clean:
	rm -f $(ALLOBJS)
	rm -f $(TOOLS)
	rm -f $(TREES_OBJ)
	rm -f $(LIB_NAME)