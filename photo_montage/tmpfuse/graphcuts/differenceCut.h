#ifndef DIFFCUT_H
#define DIFFCUT_H

#include "portraitcut.h"

//#pragma unmanaged

class DifferenceCut : public PortraitCut {

 public:

	DifferenceCut(int oWidth, int oHeight, vector<ImageAbs*>* images, ushort* result);

	float BVZ_data_penalty(Coord p, ushort d);
	float BVZ_interaction_penalty(Coord p, Coord np, ushort l, ushort nl);
    void rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered);

	~DifferenceCut() {}
	
	Vec3f _avoidColor;

protected:


};

//#pragma managed

#endif
