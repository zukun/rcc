#ifndef GLOBALS_H
#define GLOBALS_H

// Unfortunately, EnergyFunction wants a function pointer to SmoothCostFn, which means 
// it can't be a member function.  This means width, height, and sources must be global 
// variables, so SmoothCostFn can access them.  This is painful.

#include <vector>
#include "image.h"

extern int g_width, g_height;
extern std::vector<Image*> g_sources;


#endif
