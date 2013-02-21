CODEPATH     = ../../code/
OBJS_BASIC   = PlanarException.o Planar.o DynPath.o CutPlanar.o CGraph.o
OBJS_SHAPE   = CutShape.o
OBJS_SEGMENT = CutGrid.o CutSegment.o
OBJS_GRID    = CutGrid.o
RUNS = testCutPlanar testCutShape testCutSegment testCutGrid

FOBJS_BASIC   = $(addprefix $(CODEPATH),$(OBJS_BASIC))
FOBJS_SHAPE   = $(addprefix $(CODEPATH),$(OBJS_SHAPE))
FOBJS_SEGMENT = $(addprefix $(CODEPATH),$(OBJS_SEGMENT))
FOBJS_GRID    = $(addprefix $(CODEPATH),$(OBJS_GRID))

CXXFLAGS= -O2 -Wall
CXX=g++

all: $(RUNS)

testCutPlanar: $(FOBJS_BASIC) testCutPlanar.cpp
	$(CXX) $(CXXFLAGS) testCutPlanar.cpp $(FOBJS_BASIC) -o testCutPlanar

testCutShape: $(FOBJS_BASIC) $(FOBJS_SHAPE) testCutShape.cpp
	$(CXX) $(CXXFLAGS) testCutShape.cpp $(FOBJS_BASIC) $(FOBJS_SHAPE) -o testCutShape

testCutSegment: $(FOBJS_BASIC) $(FOBJS_SEGMENT) testCutSegment.cpp
	$(CXX) $(CXXFLAGS) testCutSegment.cpp $(FOBJS_BASIC) $(FOBJS_SEGMENT) -o testCutSegment

testCutGrid: $(FOBJS_BASIC) $(FOBJS_GRID) testCutGrid.cpp
	$(CXX) $(CXXFLAGS) testCutGrid.cpp $(FOBJS_BASIC) $(FOBJS_GRID) -o testCutGrid

# compile and generate dependency info
%.o: %.cpp %.h Makefile
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $*.o
	@$(CXX) -MM $(CXXFLAGS) $*.cpp > $*.d

# pull in dependency info for *existing* .o files
-include $(OBJS_BASIC:.o=.d) $(OBJS_SHAPE:.o=.d) $(OBJS_SEGMENT:.o=.d) $(OBJS_GRID:.o=.d)


clean:
	-rm $(FOBJS_BASIC) $(FOBJS_SHAPE) $(FOBJS_SEGMENT) $(FOBJS_GRID)
	-rm $(patsubst %.o,%.d,$(FOBJS_BASIC)) $(patsubst %.o,%.d,$(FOBJS_SHAPE)) $(patsubst %.o,%.d,$(FOBJS_SEGMENT)) $(patsubst %.o,%.d,$(FOBJS_GRID)) 
	-rm $(RUNS)

