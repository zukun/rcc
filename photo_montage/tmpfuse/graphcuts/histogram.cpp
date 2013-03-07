#include "histogram.h"

FILE* _outFile;

void ChannelHisto::Init (int min, int max, int bins)
{
  _min = min;
  _max = max;
  _bins = bins;
  _binSize = (max - min)/(float)bins;
  _total = 0;
  if (_data != NULL)
    delete [] _data;
  _data = new int[_bins];
  for (int i=0; i < _bins; i++) {
    _data[i] = 0;
  }
  _peakBin = -1;
}

bool ChannelHisto::AddDataPoint (int val)
{
  if (val < _min || val > _max)
    return false;
  
  int bin =(int)  (val/_binSize);
  
  assert (bin >= 0 && bin < _bins);
  
  _data[bin]++;
  _total++;
  
  if (_data[bin] > _peakBin)
    _peakBin = bin;
  
  return true;
}

void ChannelHisto::ComputeVariance ()
{
  _mean = 0;
  for (int i=0; i < _bins; i++) {
    _mean += _data[i]*((i+1)*_binSize);
  }
  _mean /= _total;

  _var = 0;

  for (int i=0; i < _bins; i++) {
    _var += _data[i]*(((i+1)*_binSize - _mean)*(i*_binSize - _mean));
  }
}

float ChannelHisto::GetProbability (int val)
{
  int bin = (int) (val/_binSize);
  
  assert (bin >= 0 && bin < _bins);
  float result = _data[bin]/(float)_total;
 
  return result;
}

void ChannelHisto::Dump ()
{
  fprintf(_outFile, "<");
  for (int i=0; i < _bins; i++) {
    fprintf(_outFile,"%d ", _data[i]);
  }
  
  fprintf(_outFile, "> - %d\n", _total);
}

PixelHisto::PixelHisto ()
{
  _rc.Init (0,256, NUM_BINS);
  _gc.Init (0, 256, NUM_BINS);
  _bc.Init (0, 256, NUM_BINS);
}


bool PixelHisto::AddDataPoint (int r, int g, int b)
{
  if (!_rc.AddDataPoint (r))
    return false;
  
  if (!_gc.AddDataPoint (g))
    return false;

  if (!_bc.AddDataPoint (b))
    return false;
  
  return true;
}

float PixelHisto::GetProbability (int r, int g, int b)
{
  float result = 0;
  result = _rc.GetProbability (r) * _gc.GetProbability (g) * _bc.GetProbability (b);
  return result;
}

void PixelHisto::Dump ()
{
  fprintf(_outFile, "red\n");
  _rc.Dump ();
  fprintf(_outFile, "green\n");
  _gc.Dump ();
  fprintf(_outFile, "blue\n");
  _bc.Dump ();
}
/*
double ChannelHistoCurve::GetY (wxInt32 x)
{
int bin = (int) (x/_data->GetBinSize ());
int result = _data->GetValueFor(bin);
return result;
}
*/

Histogram::Histogram (const ImageData* id) : _id (id) {

  if (_id->size () < 2)
    return;

  _histograms = new PixelHisto[_id->w()*_id->h()];

  for (int i=0; i < _id->w(); i++) {
    for (int j=0; j < _id->h(); j++) {
      for (int k=0; k < id->size (); k++) {
	const unsigned char* I = id->_imptr (k, Coord(i, j));
	bool res = _histograms[j*_id->w() + i]._rc.AddDataPoint (I[0]);
	assert (res);
	res = _histograms[j*_id->w() + i]._gc.AddDataPoint (I[1]);
	assert (res);
	res = _histograms[j*_id->w() + i]._bc.AddDataPoint (I[2]);
	assert (res);
      }
      _histograms[j*_id->w() + i]._rc.ComputeVariance ();
      _histograms[j*_id->w() + i]._gc.ComputeVariance ();
      _histograms[j*_id->w() + i]._bc.ComputeVariance ();
    }
  }
}


