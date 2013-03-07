#ifndef _HDRCUT_H_
#define _HDRCUT_H_
#include <set>
#include "portraitcut.h"
#include "histogram.h"

class HDRCut : public PortraitCut {

public:
	HDRCut(int oWidth, int oHeight, vector<ImageAbs*>* images, ushort* result, FILE* fp, Histogram* hist);

	~HDRCut() { _hist = NULL;}

	float BVZ_data_penalty(Coord p, ushort d);
	float BVZ_interaction_penalty(Coord p, Coord np, ushort l, ushort nl);
	void SetCutType (int c);
	float _interCost, _potts;
	
protected:

	Histogram* _hist;
	set<ushort> _relevant; // images numbers to be searched during graph cut comps
	FILE* _output;
};

#endif
