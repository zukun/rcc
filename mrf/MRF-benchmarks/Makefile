# top-level Makefile for MRF benchmarks
# to compile everything, type 'make'
# to run all benchmarks (warning - this will take hours), type 'make run'
# to remove the compiled code, type 'make clean'
# to remove the benchmark results, type 'make runclean'

LIBS = MRF imageLib
TESTS = mrfstereo photomontage binary denoise
ALLDIRS = $(LIBS) $(TESTS)

# optimization flags, export to sub-makes
export OPT = -O3
# export OPT = -DNDEBUG -O3 -funroll-loops --fast-math -mtune=pentium4 -mfpmath=sse -msse2

.PHONY: code $(ALLDIRS) clean depend run runclean

code: $(ALLDIRS)

$(ALLDIRS):
	$(MAKE) -C $@

$(TESTS): $(LIBS)

depend:
	for d in $(ALLDIRS); do (cd $$d ; make depend ); done

clean:
	for d in $(LIBS); do (cd $$d ; make allclean ); done
	for d in $(TESTS); do (cd $$d ; make clean ); done

run: code
	for d in $(TESTS); do (cd $$d ; make run ); done

runclean:
	for d in $(TESTS); do (cd $$d ; make runclean ); done
