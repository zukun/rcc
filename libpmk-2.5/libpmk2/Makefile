CC = g++
LD = ld
CFLAGS = -Wall -Wno-deprecated -O3
LDFLAGS = -lm
LIBPMK_NAME = libpmk.o
LIBPMK_UTIL_NAME = libpmk_util.o

POINTSET_OBJ = \
	point_set/point.o \
	point_set/point-set.o \
	point_set/point-set-list.o \
	point_set/point-ref.o \
	point_set/mutable-point-set-list.o \
	point_set/on-disk-point-set-list.o

UTIL_OBJ = \
	util/distance-computer.o \
	util/sparse-vector.o \
	util/labeled-index.o

TREE_OBJ = \
	tree/sparse-tree-node.o \
	tree/sparse-tree.o \
	tree/tree-node.o \
	tree/tree.o \
	tree/point-tree-node.o

HISTOGRAMS_OBJ = \
	histograms/bin.o \
	histograms/multi-resolution-histogram.o \
	histograms/histogram.o

PYRAMIDS_OBJ = \
	pyramids/pyramid-maker.o \
	pyramids/uniform-pyramid-maker.o \
	pyramids/normalized-uniform-pyramid-maker.o \
	pyramids/vg-pyramid-maker.o \
	pyramids/global-vg-pyramid-maker.o \
	pyramids/input-specific-vg-pyramid-maker.o \
	pyramids/pyramid-matcher.o


KERNEL_OBJ = \
	kernel/matrix.o \
	kernel/kernel-matrix.o

SVM_OBJ = \
	svm/svm.o

EXPERIMENT_OBJ = \
	experiment/experiment.o \
	experiment/svm-experiment.o \
	experiment/prob-svm-experiment.o \
	experiment/example-selector.o \
	experiment/random-selector.o \
	experiment/eth-selector.o \
	experiment/nn-experiment.o

CLUSTERING_OBJ = \
	clustering/clusterer.o \
	clustering/k-means-clusterer.o \
	clustering/hierarchical-clusterer.o \
	clustering/rnn-clusterer.o

UNITTESTS = \
	point_set/point_unittest.out \
	point_set/point-set_unittest.out \
	point_set/mutable-point-set-list_unittest.out \
	point_set/on-disk-point-set-list_unittest.out \
	tree/sparse-tree_unittest.out \
	tree/tree_unittest.out \
	clustering/k-means-clusterer_unittest.out \
	clustering/hierarchical-clusterer_unittest.out \
	histograms/bin_unittest.out \
	histograms/multi-resolution-histogram_unittest.out \
	histograms/histogram_unittest.out \
	pyramids/uniform-pyramid-maker_unittest.out \
	pyramids/pyramid-matcher_unittest.out \
	kernel/kernel-matrix_unittest.out \
	kernel/matrix_unittest.out \
	experiment/example-selector_unittest.out \
	clustering/rnn-clusterer_unittest.out


TOOLS = \
	tools/random-sampler.out \
	tools/random-sampling-merger.out \
	tools/pointset-merger.out \
	tools/pointsets-to-uniform-pyramids.out \
	tools/hierarchical-cluster-point-set.out \
	tools/pyramid-match-kernel.out \
	tools/pyramid-match-self-similarity.out \
	tools/pyramid-match-split.out \
	tools/partial-pyramid-match-kernel.out \
	tools/clusters-to-pyramids.out \
	tools/kernel-adder.out \
	tools/normalize-kernel.out \
	tools/exponent-kernel.out \
	tools/psl-sampler.out \
	tools/clean-psl.out \
	tools/extract-classes-from-psl.out \
	tools/extract-subset-from-psl.out \
	tools/reverse-psl.out \
	tools/generate-random-psl.out \
	tools/clean-single-psl.out \
	tools/pyramid-match-chunk.out

DIAGNOSTIC = \
	diagnostic/pointset-viewer.out \
	diagnostic/pyramid-viewer.out \
	diagnostic/hc-viewer.out \
	diagnostic/kernel-viewer.out

LIBPMK_OBJS = \
	$(POINTSET_OBJ) \
	$(UTIL_OBJ) \
	$(TREE_OBJ) \
	$(CLUSTERING_OBJ) \
	$(HISTOGRAMS_OBJ) \
	$(PYRAMIDS_OBJ)


LIBPMK_UTIL_OBJS = \
	$(KERNEL_OBJ) \
	$(SVM_OBJ) \
	$(EXPERIMENT_OBJ)

ALLOBJS = \
	$(LIBPMK_OBJS) \
	$(LIBPMK_UTIL_OBJS) \

INC = -I.

libpmk: $(LIBPMK_OBJS)
	$(LD) -r -o $(LIBPMK_NAME) $(LIBPMK_OBJS)

libpmk_util: $(LIBPMK_UTIL_OBJS)
	$(LD) -r -o $(LIBPMK_UTIL_NAME) $(LIBPMK_UTIL_OBJS)

objs: $(ALLOBJS)
tests: $(ALLOBJS) $(UNITTESTS)
tools: $(ALLOBJS) $(TOOLS)
diag: $(ALLOBJS) $(DIAGNOSTIC)

.cc.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.cc.out:
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) $(ALLOBJS) $< -o $@

clean:
	rm -f $(ALLOBJS)
	rm -f $(UNITTESTS)
	rm -f $(TOOLS)
	rm -f $(LIBPMK_NAME) $(LIBPMK_UTIL_NAME)
	rm -f $(DIAGNOSTIC)

reallyclean: clean
	rm -f histograms/*~ point_set/*~ util/*~ pyramids/*~ kernel/*~ *~
	rm -f svm/*~ experiment/*~ emd/*~ caltech101/*~ eth/*~ clustering/*~
	rm -f diagnostic/*~