#ifndef DIALCUT_H
#define DIALCUT_H
#include <set>
#include "portraitcut.h"
#include "distanceMap.h"

#include "luminanceCut.h"
#include "colorCloneCut.h"
#include "contrastCut.h"
#include "imageCut.h"
#include "histogramCut.h"
#include "maxDiffCut.h"
#include "timelapseCut.h"
#include "panoCut.h"

struct CutResult {
  CutResult() {}
  ImageAbs* _im; // NOT OWNED
  bool* _mask;  // OWNED
  int _num;
  double _energy; 

  bool operator< (const CutResult& other) const { return _energy < other._energy; }

  /*
  // all should be same length
  std::vector<ImageAbs*> _dialed;  // NOT OWNED
  std::vector<bool*> _masks;       // OWNED
  std::vector<int> _nums;*/
};


struct AllObjectives {
  LuminanceCut*   _highlight;
  LuminanceCut*   _shadow;
  ColorCloneCut*  _color;
  ImageCut*       _num;
  HistogramCut*   _maxhist;
  HistogramCut*   _minhist;
  ContrastCut*    _maxcontrast;
  ContrastCut*    _mincontrast;
  MaxDiffCut*     _maxdiff;
  TimelapseCut*   _timelapse;
  PanoCut*        _pano;

  AbstractDataObj* getObj(const StrokeType st);
};

class DialCut : public PortraitCut {

 public:

  // expects base image as 0th
  DialCut(int oWidth, int oHeight, vector<ImageAbs*>* images, ushort* result, FILE* fp=stdout);
  virtual ~DialCut();

  void takeStroke2(const short* stroked, StrokeType st, std::vector<CutResult>& cr);

  // stroked is w*h of short, 1 where stroke is
  void takeStroke(const short* stroked, StrokeType st);

  void fillNewRelevant(vector<RowSpan> spans, StrokeType st);

 // void lockEdgesToBase();
//  void unlockEdgesToBase();

  void recreateRelevant();

  void addImage(ImageAbs* im);

  // Does single expand of label 1
  //void compute1();

  void computeMulti();  // Does full compute over all _relevant
  int computeSingle(); // returns best single expansion


  void compute();

  void clearAllStrokes ();
  void assertCurrLabelingNotZero() { assert(notAllZero(_labels)); }
  void checkThisLabeling(ushort* l) { assert(notAllZero(l)); }

  void setDialColor(const Vec3f& v); // 0->1
  void setDialNum(const int d);

  void takeMaskAsNumBrush(const bool* mask, const int c);
  void takeMaskAndStencil(const bool* mask, const short* stencil, const int c);


  const StrokeData& getStrokeData(int x, int y) const {
    assert(x>=0 && x<_w && y>=0 && y<_h);
    return (_allStrokes[y*_w + x]);
  }

  StrokeData* getAllStrokes() { return _allStrokes; }
  void takeAllStrokeData(StrokeData* sd) { memcpy(_allStrokes, sd, _w*_h*sizeof(StrokeData)); }

  void saveRGBSamples(const int x, const int y);

  void wipeLabels(const int c);

  void removeLastImage();

  bool relevant(const int l);

  // set filter incorporates the data_penalty of another cutter as a weak prior.
  // The data_penalty is only used where a stroke does not exist, and we 
  // now own the object ( do not delete it yourself).
  // Replaces inertia term!
  void setFilter(PortraitCut* pc);
  bool hasFilter() const { return (_filter != NULL); }

  bool multiRegion() const { return _multiRegion; }

  virtual void loadLabeling(const char* name);

  // weights
  float /*_bright,*/ _inertia;// _potts, _regular_inter;
  int _numDial;
  bool _forceStroke, _multiRegion;

  bool notAllZero(ushort* l);

  void externalLabellingChange(); // Call me if someone other than me changes labeling (like a filter)

  void setGlobalCompute(const bool b) { _globalCompute = b; }
  void setGlobalObjType(const StrokeType s) { _globalObjType = s; }

  AbstractDataObj* getObj(const StrokeType st) { return _allObj.getObj(st); }

  AllObjectives _allObj;
  //PortraitCut* _currObjective;

 protected:


  set<ushort> _relevant; // images numbers to be searched during graph cut comps
  vector<ushort> _newRelevant; // test candidates, only one will be added to _relevant
  ushort* _L; // initial condition before re-labelling
  //short* _currStroke; // 1 under stroke, -1 otherwise
  //StrokeType _st;
  Vec3f _dialColor;
  int _dialNum;
  PortraitCut* _filter;
  bool _inertiaRelevant;
  DistanceMap* _strokeDmap;
  
  bool _globalCompute;
  StrokeType _globalObjType;
  StrokeData* _allStrokes; // accumulates goal strokes

  // METHODS
  float BVZ_data_penalty(Coord p, ushort d);

  float BVZ_interaction_penalty(Coord p, Coord np, ushort l, ushort nl);

  void rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered);
  float getDataCost (const struct StrokeData* s, Coord p, ushort d) { return BVZ_data_penalty (p, d); }

  void setDefaults();
  void addStrokeToAll(const vector<RowSpan>& stroke, StrokeType st);
//  void computeMedianCGrad(const int index, CGrad& cgrad, Vec3i& medColor);
  void addAllToRelevant();
  void orderNegImages(std::vector<RowSpan>& stroke, std::priority_queue<Ranker>& ordered);
  void createMask(bool* mask, const int which);
  void makeInertiaRelevant(const vector<RowSpan>& spans);
  void makeInertiaIrrelevant();
  double strokeCost(const std::vector<RowSpan>& spans);

  void compactStroke(const short* stroked, std::vector<RowSpan>& spans) const;
};


#endif
