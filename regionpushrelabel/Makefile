# Makefile

CPP = g++
CPPFLAGS = -w
#CPPFLAGS = -g -Wall
LDFLAGS = -lboost_thread -lboost_system -lboost_filesystem

maxflow: *.cpp
	$(CPP) $(CPPFLAGS) *.cpp -o maxflow $(LDFLAGS)

clean:
	rm -f maxflow
