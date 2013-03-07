#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_
#include <vector>
#include "imageData.h"
//#include <wx/plot/plot.h>

#define NUM_BINS	20

class ChannelHisto {
  
 public:
  ChannelHisto () {_bins = 0; _min = 0; _max = 0; _total = 0; _binSize = 0; _data = NULL; _peakBin = -1;}
  ~ChannelHisto () { if (_data != NULL) delete [] _data; }
  void Init (int min, int max, int bins);
  bool AddDataPoint (int val);
  void ComputeVariance ();
  float GetProbability (int val);
  float GetVariance () { return _var; }
  float GetMean () { return _mean; }
  int GetMax () { return _max; }
  int GetMin () { return _min; }
  int GetNumBins () { return _bins; }
  float GetBinSize () { return _binSize; }
  int GetValueFor (int bin) { if (bin >= 0 && bin < _bins) return _data[bin]; return -1; }
  int GetBinFor (int val) { if (val >= _min && val < _max) return (int) (val/_binSize); return -1; }
  int GetPeak () { return _peakBin; }
  int GetNumImages () { return _total; }
  void Dump ();

 protected:
  int* _data;
  int _bins;
  float _binSize;
  int _min;
  int _max;
  int _total;
  float _mean;
  float _var;
  int _peakBin;
};

class PixelHisto {
 public:
  
  PixelHisto ();
  bool AddDataPoint (int r, int g, int b);
  float GetProbability (int r, int g, int b);
  void Dump ();
  
  ChannelHisto _rc;
  ChannelHisto _gc;
  ChannelHisto _bc;
};
/*
class ChannelHistoCurve : public wxPlotCurve {
public:

	ChannelHistoCurve (ChannelHisto* ch) : wxPlotCurve(10, -1, ch->GetNumImages ()) { _data = ch; }
	wxInt32 GetStartX () { return (wxInt32)_data->GetMin (); }
	wxInt32 GetEndX () { return (wxInt32)(_data->GetMax ()-1); }
	double GetY (wxInt32 x);

private:
   ChannelHisto* _data;
};
*/

class Histogram {
 public:
  
  Histogram (const ImageData* im);
  ~Histogram () { if (_histograms != NULL) delete [] _histograms; }
  PixelHisto* GetDataForPixel(int x, int y) const { return &_histograms[y*_id->w() + x]; }
  
 private:
  PixelHisto* _histograms;
  const ImageData*  _id;
};
#endif
