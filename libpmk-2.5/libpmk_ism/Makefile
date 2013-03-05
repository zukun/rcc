CC = g++
LD = ld
CFLAGS = -Wall -Wno-deprecated -O3 `pkg-config gtkmm-2.4 --cflags` -I../libpmk_features -I../libpmk2 -g `Magick++-config --cppflags` `Magick++-config --cxxflags`
LDFLAGS = -lm `pkg-config gtkmm-2.4 --libs` `Magick++-config --libs` `Magick++-config --ldflags`
LIBPMK_OBJ = ../libpmk2/libpmk.o
LIBPMK_UTIL_OBJ = ../libpmk2/libpmk_util.o
LIBPMK_FEATURES_OBJ = ../libpmk_features/libpmk_features.o
SHAPE_MODEL_NAME = libpmk_ism.o

UI_OBJ = \
	ui/frame.o \
	ui/featured-image.o \
	ui/image-box.o \
	ui/selecting-image-box.o \
	ui/training-set-view.o \
	ui/label-input-dialog.o \
	ui/image-overlay.o

SHAPE_MODEL_OBJ = \
	shape_model/codebook-entry.o \
	shape_model/implicit-shape-model.o \
	shape_model/object-position-vote.o \
	shape_model/uniform-balloon-density-estimator.o \
	shape_model/hough-accumulator-3d.o \
	shape_model/model-group.o

BINARIES = \
	ui/demo.out

ALLOBJS = $(UI_OBJ) $(SHAPE_MODEL_OBJ)

EXTRAOBJS = $(LIBPMK_OBJ) $(LIBPMK_UTIL_OBJ) $(LIBPMK_FEATURES_OBJ)

INC = -I.

ui: uiclean $(ALLOBJS) $(BINARIES)
objs: $(ALLOBJS)
diag: $(ALLOBJS) $(DIAGNOSTIC)
library: $(SHAPE_MODEL_OBJ)
	$(LD) -r -o $(SHAPE_MODEL_NAME) $(SHAPE_MODEL_OBJ)

.cc.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.cc.out:
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) $(ALLOBJS) $(EXTRAOBJS) $< -o $@

clean:
	rm -f $(ALLOBJS)
	rm -f $(BINARIES)
	rm -f $(TOOLS)

uiclean:
	rm -f $(BINARIES)

reallyclean: clean
	rm -f histograms/*~ point_set/*~ util/*~ pyramids/*~ kernel/*~ *~
	rm -f svm/*~ experiment/*~ emd/*~ caltech101/*~ eth/*~ clustering/*~
	rm -f diagnostic/*~