CXX		= g++
CC		= gcc
DEBUG		= -g -Wall

OPT-GCC		=  -O3 -funroll-loops -ffast-math -march=pentium4 -msse2 -mfpmath=sse -mmmx

INCLUDE		= -I.
LDLIBS		= -lm

CFLAGS		= $(OPT-GCC) -MMD $(OPT) $(INCLUDE) $(WARNING) 
CXXFLAGS	= $(OPT-GCC) -MMD $(OPT) $(INCLUDE) $(WARNING) 
LDFLAGS		= $(OPT) $(LDLIBS)

# ------------------ wxWindows

WX_VERSION	= gtk-2.4
WX_GL_VERSION	= gtk_gl-2.4
WX_BASE		= /homes/gws/aseem
WX_INCLUDE	= -I$(WX_BASE)/include -I$(WX_BASE)/lib/wx/include/$(WX_VERSION)

#		  -I/usr/X11R6/include -I/usr/lib/glib/include \
#		  -I/usr/include/glib-1.2

WX_LIB_DIR	= -L$(WX_BASE)/lib

WX_LIB 		= -lwx_$(WX_VERSION) 
WX_SHLIB 	= -lwx_$(WX_VERSION) #-lwx_gtk_plot-2.4
WX_SHGLLIB	= -lwx_$(WX_GL_VERSION)

WX_EXTRALIBS	= -L/usr/lib -L/usr/X11R6/lib -lgtk -lgdk -rdynamic \
	 	  -lgmodule -lgthread -lglib -lpthread -ldl -lXi -lXext \
	          -lX11 --lpng -ljpeg -ltiff -ldl -lpthread -lz 
WX_OPENGLLIBS	= -lGL -lGLU

WX_LDLIBS	= $(WX_LIB_DIR) $(WX_SHLIB) $(WX_OPENGLLIBS) $(WX_SHGLLIB)

#		  $(WX_EXTRALIBS)

WX_CPPFLAGS	= -D__WXGTK__ -DGTK_NO_CHECK_CASTS
WX_CFLAGS	= $(WX_CPPFLAGS) -D_REENTRANT $(CFLAGS) $(WX_INCLUDE)
WX_CXXFLAGS	= $(WX_CPPFLAGS) -D_REENTRANT -MMD -Wall $(OPT-GCC) $(INCLUDE) $(WARNING) $(WX_INCLUDE)
WX_LDFLAGS	= $(WX_LDLIBS) $(LDFLAGS)


# ------------------ Files

BINNAME		= fuse

C_OBJECTS	= 
CXX_OBJECTS	= graphcuts/graph.o  graphcuts/distanceMap.o \
		graphcuts/maxflow.o graphcuts/image.o linearSolver.o \
		ImageAbs.o graphcuts/imageData.o graphcuts/contrastCut.o \
		graphcuts/histogramCut.o graphcuts/colorCloneCut.o \
		graphcuts/imageCut.o graphcuts/luminanceCut.o graphcuts/timelapseCut.o graphcuts/maxDiffCut.o \
		graphcuts/panoCut.o graphcuts/histogram.o bestFitAffine.o hb.o steppableLinearSolver.o \
		blending/oldBlend.o blending/blendFuncs.o
WX_OBJECTS	= fuse.o myGLCanvas.o baseGLViewer.o compViewer.o stackViewer.o brush.o constantEditor.o \
		  sortedStackViewer.o graphcuts/portraitcut.o graphcuts/dialCut.o myBusyInfo.o blending/blend.o 

ALL_OBJECTS	= $(WX_OBJECTS) $(C_OBJECTS) $(CXX_OBJECTS)

default: $(BINNAME)

# ------------------ Dependencies

.DELETE_ON_ERROR:
-include $(ALL_OBJECTS:.o=.d)

# ------------------ Compilation rules

$(C_OBJECTS):
	touch $@
	$(CC) -c $(CFLAGS) -o $@ $(@:.o=.c)

$(CXX_OBJECTS):
	touch $@
	$(CXX) -c $(CXXFLAGS) -o $@ $(@:.o=.cpp)

$(WX_OBJECTS):
	touch $@
	$(CXX) -c $(WX_CXXFLAGS) -o $@ $(@:.o=.cpp)


$(BINNAME): $(ALL_OBJECTS)
	$(CXX) $(WX_LDFLAGS) $(ALL_OBJECTS) -o $(BINNAME)


.PHONY : clean
clean:
	-rm *.o *.d */*.d */*.o $(BINNAME) >& /dev/null
