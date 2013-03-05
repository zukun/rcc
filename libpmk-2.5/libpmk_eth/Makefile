CC = g++
CFLAGS = -Wall -Wno-deprecated -O3
LDFLAGS = -lm -lstdc++

# Fill in the following two lines:
LIBPMK_OBJ_PATH = ../libpmk2
LIBPMK_INC_PATH = ../libpmk2

LIBPMK_OBJS = $(LIBPMK_OBJ_PATH)/libpmk.o $(LIBPMK_OBJ_PATH)/libpmk_util.o
INC = -I$(LIBPMK_INC_PATH)


all: eth-experiment.out
clean:
	rm eth-experiment.out

.cc.out:
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) $(LIBPMK_OBJS) $< -o $@