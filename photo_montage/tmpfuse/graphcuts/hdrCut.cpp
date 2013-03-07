#include "hdrCut.h"


HDRCut::HDRCut(int oWidth, int oHeight, vector<ImageAbs*>* images, ushort* result, FILE* fp, Histogram* hist) :
  PortraitCut(oWidth, oHeight, images, result, fp) {
  _output = fp;
  _hist = hist;
  setCuttype(C_GRAD2);
  //memset(_labels, 0, _w*_h*sizeof(ushort));
  //memset(_required, (short)-1, _w*_h*sizeof(short));

  //allocateComputeAllGradMags();
  allocateComputeAllCGrads();

  /*	for (int i=0; i < _images.size (); i++) {
	_relevant.insert(i);
	allocateComputeGradMag(i);
	}*/
  _potts = 0.;
  _interCost = 1.;
}

float HDRCut::BVZ_data_penalty(Coord p, ushort d) {
  PixelHisto* ph = _hist->GetDataForPixel (p.x, p.y);
  unsigned char* I = _imptr(d,p);

  int totalBins = ph->_rc.GetNumBins ();
  int rPeak = ph->_rc.GetPeak ();
  int gPeak = ph->_gc.GetPeak ();
  int bPeak = ph->_bc.GetPeak ();

  int rBin = ph->_rc.GetBinFor (I[0]);
  int gBin = ph->_gc.GetBinFor (I[1]);
  int bBin = ph->_bc.GetBinFor (I[2]);


  if ((I[0] == 0 && I[1] == 0 && I[2] == 0) ||
      (I[0] == 255 && I[1] == 255 && I[2] == 255))
    return A_INFINITY;
  
  int cost = 0;


  if (rBin < rPeak)
    cost += rBin;
  else if (rBin > rPeak)
    cost += ph->_rc.GetNumBins () - 1 - rBin;
  else 
    cost += ph->_rc.GetNumBins ();

  if (gBin < gPeak)
    cost += gBin;
  else if (gBin > gPeak)
    cost += ph->_gc.GetNumBins () - 1 - gBin;
  else
    cost += ph->_gc.GetNumBins ();

  if (bBin < bPeak)
    cost += bBin;
  else if (bBin > bPeak)
    cost += ph->_bc.GetNumBins () - 1 - bBin;
  else
    cost += ph->_bc.GetNumBins ();

  return cost;

  //		return (ph->_rc.GetBinFor (I[0]) - rMean)*
  //			(ph->_gc.GetBinFor(I[1]) - gMean)*
  //			(ph->_bc.GetBinFor (I[2]) - bMean);
  return 1.f - ph->GetProbability (I[0], I[1], I[2]);
}

float HDRCut::BVZ_interaction_penalty(Coord p, Coord np, ushort l, ushort nl) {
  
  return (_potts + _interCost*PortraitCut::BVZ_interaction_penalty(p,np,l,nl));
}

void HDRCut::SetCutType (int c)
{
  if (c==0)
    setCuttype(C_NORMAL);
  else if (c==1)
    setCuttype(C_GRAD);
  else if (c==2)
    setCuttype(C_GRAD2);
  else
    assert(0);
}
