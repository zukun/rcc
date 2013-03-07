#include <time.h>
#include <algorithm>
#include "dialCut.h"
#include "colorCloneCut.h"
#include "imageCut.h"

AbstractDataObj* AllObjectives::getObj(const StrokeType st) {

  switch (st) {
  case S_HIGHLIGHT:
    return _highlight;
  case S_SHADOW:
    return _shadow;
  case S_COLOR:
    return _color;
  case S_NUM:
    return _num;
  case S_MAXHIST:
    return _maxhist;
  case S_MINHIST:
    return _minhist;
  case S_MAXCONTRAST:
    return _maxcontrast;
  case S_MINCONTRAST:
    return _mincontrast;
  case S_MAXDIFF:
    return _maxdiff;
  case S_TIMELAPSE:
    return _timelapse;
  case S_PANO:
    return _pano;
  default:
    assert(0);
    return NULL;
  }

}


DialCut::DialCut(int oWidth, int oHeight, vector<ImageAbs*>* images, ushort* result, FILE* fp) : 
  PortraitCut(oWidth, oHeight, images, result, fp)
{  
  memset(_labels, 0, _w*_h*sizeof(ushort));
  //memset(_required, (short)-1, _w*_h*sizeof(short));
  _relevant.insert(0);
  //_currStroke = NULL;
  _L = new ushort[_w*_h];
  setDefaults();
  fprintf(_fp, "Dial cut _n is  %d images\n",_n); fflush(_fp);
  fprintf(_fp, "Dial cut _images length is  %d images\n",_idata->size()); fflush(_fp);
  _allStrokes = NULL;
  //if (_cuttype == C_GRAD)
  //allocateComputeGradMag(0);
  //else if (_cuttype == C_GRAD2)
  //allocateComputeCGrad(0); // MAYBE NOT NECESSARY
  //allocateAuxData(0);
  _allStrokes = new StrokeData[_w*_h];
  _filter = NULL;
  _strokeDmap = NULL;
  makeInertiaIrrelevant();
  //_currObjective = NULL;
  _globalCompute = false;
  _globalObjType = S_MINCONTRAST;


  _allObj._highlight   = new LuminanceCut (_idata, LuminanceCut::Max);
  _allObj._shadow      = new LuminanceCut (_idata, LuminanceCut::Min);
  _allObj._color       = new ColorCloneCut (_idata);
  _allObj._num         = new ImageCut (_idata);
  _allObj._maxhist     = new HistogramCut (_idata, HistogramCut::Max);
  _allObj._minhist     = new HistogramCut (_idata, HistogramCut::Min);
  _allObj._maxcontrast = new ContrastCut (_idata, ContrastCut::Max);
  _allObj._mincontrast = new ContrastCut (_idata, ContrastCut::Min);
  _allObj._maxdiff     = new MaxDiffCut(_idata);
  _allObj._timelapse   = new TimelapseCut (_idata);
  _allObj._pano        = new PanoCut (_idata);

}

void DialCut::setDefaults() {
  _bright = 0.05f; // .05f default, 0.5
  _inertia = 0.01f;  // .01f , 0 for group fixup
  _potts = 0.000001f;  // 0
  _regular_inter = 1.0f;
  _numDial = 5;
  _forceStroke = false;
  _multiRegion = false;
  setCuttype(C_GRAD);
  //allocateComputeAllGradMags();  
  //allocateAllAuxData(); 
}

// this function should only be called if StrokeType is S_COLOR
void DialCut::setDialColor(const Vec3f& v) { 
  _dialColor = v; 
  _allObj._color->_dialColor = v;
}

void DialCut::setDialNum(const int d) { 
  _dialNum=d;
  _allObj._num->_num = d;

}


void DialCut::externalLabellingChange() {
  recreateRelevant();
  for (std::set<ushort>::const_iterator c = _relevant.begin();
       c != _relevant.end(); ++c) {
    allocateAuxData(*c);
  }
}


void DialCut::addImage(ImageAbs* im) {

  PortraitCut::addImage(im);
  allocateAuxData(_n-1);
}

void DialCut::removeLastImage() {

  PortraitCut::removeLastImage();
}

void DialCut::makeInertiaRelevant(const vector<RowSpan>& spans) {
  _inertiaRelevant = true;
  if (_strokeDmap) delete _strokeDmap;
  _strokeDmap = new DistanceMap(_w, _h, spans);
}

void DialCut::makeInertiaIrrelevant() {
  _inertiaRelevant = false;
  if (_strokeDmap) {
    delete _strokeDmap;
    _strokeDmap = NULL;
  }
}

void DialCut::takeStroke2(const short* stroked, StrokeType st, std::vector<CutResult>& cr) {

  allocateAuxData(0);
  // convert stroke to compactStroke
  vector<RowSpan> spans;
  compactStroke(stroked,spans);

  assert(!_multiRegion && !_filter);

  if (st==S_ERASE) {
    addStrokeToAll(spans, st);
    recreateRelevant();
    return;
  }

  fillNewRelevant(spans, st);
  // Init _L, initial state
  memcpy(_L, _labels, _w*_h*sizeof(ushort));
  double spatial = BVZ_ComputeSpatialOnly(), E;
  for (std::vector<ushort>::iterator c = _newRelevant.begin(); 
       c != _newRelevant.end(); ++c) {
    _dialNum = *c;
    addStrokeToAll(spans, S_NUM);
    //if (E < 0) {
    makeInertiaRelevant(spans);
    E = spatial + BVZ_ComputeDataOnly();
    //E = BVZ_ComputeEnergy();
    printf("starting energy %f\n",E);
    //}
    CutResult newcr;
    newcr._num = *c;
    newcr._im = _idata->images(*c);  assert(newcr._im);
    newcr._energy = BVZ_Expand(*c, E);
    if (st!=S_NUM) {
      addStrokeToAll(spans, st);
      newcr._energy += strokeCost(spans); // assumes S_NUM constraint completely satisfied
    }
    printf("computed energy %d as %f\n",*c, newcr._energy);
    //assert(newcr._energy <= E);
	if (newcr._energy <= E) {
	    newcr._mask = new bool[_w*_h];  
		createMask(newcr._mask, *c);
		cr.push_back(newcr);
	}
    memcpy(_labels,  _L, _w*_h*sizeof(ushort));
  }

  //addStrokeToAll(spans, st);   //not sure if we need this here
  //sort(cr.begin(), cr.end());  // sorts in ascending order
  makeInertiaIrrelevant();
}

double DialCut::strokeCost(const std::vector<RowSpan>& spans) {
  int x, y, s, index;
  double E = 0;
  for (unsigned int j=0; j<spans.size(); ++j) {
    x = spans[j]._x; y = spans[j]._y;
    for (s=0; s<spans[j]._num; ++s) {
      Coord p(x+s, y);
      index = CINDEX(p);
      E += BVZ_data_penalty(p, _labels[index]);
      //const StrokeData& sd = _allStrokes[index];
      //E += _allObj.getObj(sd._st)->getDataCost(&sd, p, _labels[index]);
    }
  }
  return E;
}

void DialCut::createMask(bool* mask, const int which) {
  int i,j,index=0;
  for (j=0; j<_h; ++j)
    for (i=0; i<_w; ++i, ++index) {
      mask[index]  = ( _labels[index] == which && _L[index] != which );
    }
}

void DialCut::takeMaskAsNumBrush(const bool* mask, const int c) {
  int i,j,index=0;
  for (j=0; j<_h; ++j)
    for (i=0; i<_w; ++i, ++index) 
      if (mask[index]) {
	_allStrokes[index]._st = S_NUM;
	_allStrokes[index]._num = c;
	_labels[index] = c;
      }
  recreateRelevant();
}

void DialCut::takeMaskAndStencil(const bool* mask, const short* stencil, const int c) {
  int i,j,index=0;
  for (j=0; j<_h; ++j)
    for (i=0; i<_w; ++i, ++index) {
      if (mask[index])
	_labels[index] = c;
      if (stencil[index]) {
	_allStrokes[index]._st = S_NUM;
	_allStrokes[index]._num = c;
      }
    }
  recreateRelevant();
}

void DialCut::fillNewRelevant(vector<RowSpan> spans, StrokeType st) {
  // Dial up best MIN(_NUM_DIAL_, n) images
  std::priority_queue<Ranker> dialed;
  _newRelevant.clear();


  if (st == S_DIFF) 
    orderNegImages(spans, dialed);
  
  else if (st==S_NUM) {
    allocateAuxData(_dialNum);
    _newRelevant.push_back(_dialNum);
    return;
  }
  else
    _allObj.getObj(st)->rankImages(spans, dialed);

  // Make sure dialed images have their aux defined
  // Add dialed images to relevant vector
  int ul = MIN(_n, _numDial);
  for (int i=0; i<ul; ++i) {
    //pop_heap
    const Ranker& rank = dialed.top();
    fprintf(_fp,"score %d: %f\n",rank._which,rank._score); fflush(_fp);
    //if (i>0) assert(dialed[i]._score > dialed[i-1]._score);
    allocateAuxData(rank._which);
    _newRelevant.push_back(rank._which);
    dialed.pop();
  }
}


//#define _NUM_DIAL_ 1
void DialCut::takeStroke(const short* stroked, StrokeType st) {

  fprintf(_fp, "Current Settings: \n");
  fprintf(_fp, "Brightness: %f\n",_bright);
  fprintf(_fp, "Inertia: %f\n",_inertia);
  fprintf(_fp, "Potts: %f\n",_potts);
  fprintf(_fp, "Seam: %f\n",_regular_inter);
  fprintf(_fp, "CutType: %d\n",_cuttype);
  fprintf(_fp, "NumDial: %d\n",_numDial);
  fprintf(_fp, "ForceStroke: %d\n\n",_forceStroke);
  fflush(_fp);	
  //int i;


  allocateAuxData(0);
  // convert stroke to compactStroke
  vector<RowSpan> spans;
  compactStroke(stroked,spans);

  if (_multiRegion) {
    // Init _L, initial state
    memcpy(_L, _labels, _w*_h*sizeof(ushort));
    addStrokeToAll(spans, st);
    makeInertiaRelevant(spans);    
    addAllToRelevant();
    computeMulti();
    recreateRelevant();
    makeInertiaIrrelevant();    
    return;
  }
  
  if (_filter) {
    addStrokeToAll(spans, st);
    addAllToRelevant();
    computeMulti();
    recreateRelevant();
    return;
  }


  makeInertiaRelevant(spans);

  if (st==S_ERASE) {
    addStrokeToAll(spans, st);
    //recreateRelevant();
    //computeMulti();
    //recreateRelevant();
    makeInertiaIrrelevant();
    return;
  }


  fillNewRelevant(spans, st);

  // Init _L, initial state
  memcpy(_L, _labels, _w*_h*sizeof(ushort));

  // Prepare whatever for energy terms
  //_currStroke = stroked;
  addStrokeToAll(spans, st);
  //_st = st;

  time_t startTime, endTime;
  startTime = time(NULL);

  if (!_multiRegion) {
    computeSingle();
    recreateRelevant();
  }
  else {
    for (vector<ushort>::iterator c=_newRelevant.begin(); c!=_newRelevant.end(); ++c)
      _relevant.insert(*c);
    //_relevant += _newRelevant;
    computeMulti();
    recreateRelevant();
  }
  
  endTime = time(NULL);
  fprintf(_fp, "Took time %f\n",difftime(endTime, startTime)); fflush(_fp);

  makeInertiaIrrelevant();
  // update relevant vector
  //recreateRelevant();
  //_currStroke = NULL;
}

void DialCut::addStrokeToAll(const vector<RowSpan>& stroke, StrokeType st) { 
  int x, y, index;
  for (unsigned int j=0; j<stroke.size(); ++j) {
    x = stroke[j]._x; y = stroke[j]._y;
    for (int s=0; s<stroke[j]._num; ++s) {
      index = (y*_w)+x+s;
      if (st==S_ERASE) {
	_allStrokes[index]._st = S_NONE;
	continue;
      }
      _allStrokes[index]._st = st;
      if (st==S_NUM)
	_allStrokes[index]._num = _dialNum;
      else if (st==S_COLOR)
	_allStrokes[index]._dialColor = _dialColor;
      else if (st==S_DIFF) {
	const unsigned char* col = _imptr(_labels[index], Coord(x+s,y));
	Vec3i colv(col[0], col[1], col[2]);
	_allStrokes[index]._avoidColor = colv;
      }
    }
  }
}

void DialCut::orderNegImages(std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered) {
  assert(ordered.empty());
  float score, dist;
  const unsigned char *Itest, *Iavoid;
  Vec3i Vtest, Vavoid;
  int x, y, index;

  for (int i=0; i<_idata->size(); ++i) {
    score = 0;
    ImageAbs* im = _idata->images(i);
    for (unsigned int j=0; j<spans.size(); ++j) {
      x = spans[j]._x; y = spans[j]._y;
      for (int s=0; s<spans[j]._num; ++s) {
	index = y*_w + x + s;
	Itest = im->data(x+s,y);
	Iavoid = _imptr(_labels[index], Coord(x+s,y));
	Vtest.Set(Itest[0], Itest[1], Itest[2]);
	Vavoid.Set(Iavoid[0], Iavoid[1], Iavoid[2]);
	dist = sqrt(Vtest.distanceTo2(Vavoid));
	//printf("%f\n",dist);
	if (dist > 25)
	  score += 25.;
	else
	  score += dist;
      }
    }
    printf("Image %d, score %f\n",i, score);
    ordered.push(Ranker(i,score));
  }
}
/*
  void DialCut::computeMedianCGrad(const int index, CGrad& cgrad, Vec3i& medColor) {
  double* workspace = new double[_n];
  int med = _n/2;
  int c,l;
  //CGrad* dg = new CGrad[_n];
  //for (l=0; l<_n; ++l) 
  //computeCGrad(l,i,j,dg[l]);


  for (c=0; c<6; ++c) {
  for (l=0; l<_n; ++l) {
  workspace[l] = _cgrads[l][index].val(c); //dg[l].val(c);
  }
  nth_element(workspace, workspace+med, workspace+_n);
  cgrad.setVal(c,workspace[med]);
  }
  
  delete[] workspace;

  int i=index%_w, j=index/_w;
  int* cworkspace = new int[_n];
  for (c=0; c<3; ++c) {
  for (l=0; l<_n; ++l) {
  Coord p(i,j);
  const unsigned char* col = _imptr(l, p);
  cworkspace[l] = col[c];
  }
  nth_element(cworkspace, cworkspace+med, cworkspace+_n);
  medColor.data()[c] = cworkspace[med];
  }

  delete[] cworkspace;
    
  }
*/


// try expanding 1->n-1 labels, but each one off initial condition
int DialCut::computeSingle() {
  assert(!_globalCompute);
  vector<ushort>::reverse_iterator c;
  //++c;
  double E, E_old;
  int cutCount = 0;
  E = BVZ_ComputeEnergy();
  ushort* bestLabels = new ushort[_w*_h];   
  int best=0; // is this ok?
  memcpy(bestLabels, _L, _w*_h*sizeof(ushort));
  fprintf(_fp,"Starting E = %f\n",E); fflush(_fp);
  for (c = _newRelevant.rbegin(); c!=_newRelevant.rend(); ++c) {
    E_old = E;
    E = BVZ_Expand(*c, E_old);  ++cutCount;
    //fprintf(_fp,"new %f\n",E);
    assert (E <= E_old);
    if (E<E_old) {
      fprintf(_fp, "Label %d so far has best expansion\n",*c); fflush(_fp);
      best = *c;
      memcpy(bestLabels, _labels, _w*_h*sizeof(ushort));
      //assert(notAllZero(bestLabels));
    }
    fprintf(_fp,"Trying label %d, E = %f\n",*c, E);
    fflush(_fp);
    memcpy(_labels, _L, _w*_h*sizeof(ushort));  // restore initial condition
  }

  memcpy(_labels, bestLabels, _w*_h*sizeof(ushort)); // take best
  //assert(notAllZero(_labels));
  delete[] bestLabels;
  fprintf(_fp,"Computed %d cuts\n",cutCount);
  return best;
}


void DialCut::computeMulti() {
  assert(!_globalCompute);
  // Cycle through dialed images, expand
  double E, E_old;
  int iter;
  unsigned int step_counter=0;
  E = BVZ_ComputeEnergy();
  fprintf(_fp,"Starting E = %f\n",E); fflush(_fp);
  for (iter=0; iter<_ITER_MAX_ && step_counter<_relevant.size(); ++iter) {
    for (set<ushort>::reverse_iterator c = _relevant.rbegin(); 
	 int(step_counter)<_n && c!=_relevant.rend(); 
	 ++c) {
      //if (c == _relevant.rbegin()) continue; // Don't expand base, 
      E_old = E;
      E = BVZ_Expand(*c, E_old);
      fprintf(_fp,"new %f\n",E);
      //assert (E <= E_old);
      if (E_old == E) step_counter ++;
      else            step_counter = 0;      
      fprintf(_fp,"iteration %d, label %d, E = %f\n",iter, *c, E);
      fflush(_fp); 
    }
  }
  
}

void DialCut::compute() {
  assert(_globalCompute);
  PortraitCut::compute();
  _globalCompute = false;
}

bool DialCut::notAllZero(ushort* l) {
  bool allZero = true;
  for (int i=0; i<_w*_h; ++i) {
    if (l[i]!=0) {
      fprintf(_fp, "NotAllZero test: passed %d %d\n",i%_w,i/_w); fflush(_fp);
      allZero = false;
      break;
    }
  }

  return !allZero;
}

bool DialCut::relevant(const int l) {
  return (find(_relevant.begin(), _relevant.end(), l) != _relevant.end());
}

void DialCut::recreateRelevant() {   
  _relevant.clear();
  for (int j=0; j<_h; ++j)
    for (int i=0; i<_w; ++i)
      _relevant.insert(_labels[j*_w+i]);
}

void DialCut::addAllToRelevant() {
  for (int i=0; i<_n; ++i) {
    allocateAuxData(i);
    _relevant.insert(i);
  }
}

/*
  void DialCut::lockEdgesToBase() {
  // set border pixels to base image
  int i;
  for (i = 0 ; i < _w ; i++)
  _required[i] =  _required[(_h-1)*_w+i] = 0;
  for (i=0; i<_h; i++)
  _required[i*_w] = _required[i*_w + _w-1] = 0;
  }

  void DialCut::unlockEdgesToBase() {
  // set border pixels to no care
  int i;
  for (i = 0 ; i < _w ; i++)
  _required[i] =  _required[(_h-1)*_w+i] = -1;
  for (i=0; i<_h; i++)
  _required[i*_w] = _required[i*_w + _w-1] = -1;
  }
*/

/*
  void DialCut::compute1() {
  double E, E_old;
  initializeResult();  
  E_old = BVZ_ComputeEnergy();
  fprintf(_fp,"Starting E = %f\n",E_old); fflush(_fp);
  E = BVZ_Expand(1, E_old);
  fprintf(_fp,"new %f\n",E); fflush(_fp);
  }*/


float DialCut::BVZ_data_penalty(Coord p, ushort d) {

  Coord dp = p;
  _displace(dp,d);
  
  if (dp>=Coord(0,0) && dp<_idata->images(d)->_size) {  
    int index = CINDEX(p);

    /*    if (_required[index] != -1) {
	  if (d==_required[index]) // good
	  return 0.f;
	  else                     // bad
	  return A_INFINITY;
	  }*/

    if (_globalCompute) {
      return _allObj.getObj(_globalObjType)->getDataCost(p, d);
    }
    
    else {  // stroke
      assert(_allStrokes);
      // not required, in bounds, do magic
      if (_allStrokes[index]._st !=S_NONE) {// under current stroke
	const StrokeData& sd = _allStrokes[index];
	if (validStrokeType(sd._st))
	  return  _allObj.getObj(sd._st)->getDataCost(&sd, p, d);
	else if (sd._st == S_DIFF) {
	  const unsigned char* col = _imptr(d,p);
	  Vec3i c(col[0], col[1], col[2]);
	  float dist = sqrt(sd._avoidColor.distanceTo2(c));
	  if (dist < 25.)
	    return _bright * ( 25. - dist);
	  else
	    return 0;
	}
	else {
	  assert(0);
	  return 0;
	}
      }    

      // not under stroke
      if (_inertiaRelevant) {
	if (d == _L[index])
	  return 0.f;  // stay same, you did, good
	else {
	  assert(_strokeDmap);
	  return _inertia*.1*_strokeDmap->dist(p.x, p.y);
	}
      }

      return 0.f;

    } // end if stroke

  } // end if in bounds
  else           // penalize labels which fall outside of displaced images
    return A_INFINITY;
}

float DialCut::BVZ_interaction_penalty(Coord p, Coord np, ushort l, ushort nl) {
  if (l==nl) return 0;
  return (_potts + _regular_inter * PortraitCut::BVZ_interaction_penalty(p,np,l,nl));
}


DialCut::~DialCut() {
  delete[] _L;
  if (_allStrokes) delete[] _allStrokes;
  if (_filter) delete _filter;

  // should delete Objectives, but this class is instantiated once
}


void DialCut::wipeLabels(const int c) {
  assert(c>=0 && c<_n);
  int i,j, index=0;
  allocateAuxData(c);
  for (j=0; j<_h; ++j)
    for (i=0; i<_w; ++i, ++index) {
      _labels[index] = c;
    }
  recreateRelevant();
}


void DialCut::setFilter(PortraitCut* pc) {
  _filter = pc;
}

void DialCut::saveRGBSamples(const int x, const int y) {
  if (x<0 || y<0 || x>=_w || y>=_h) return;
  FILE* fpR = fopen("R.txt","w");
  FILE* fpG = fopen("G.txt","w");
  FILE* fpB = fopen("B.txt","w");

  Coord p(x,y);
  for (int i=0; i<_n; ++i) {
    unsigned char* c = _imptr(i, p);
    fprintf(fpR, "%d\n", c[0]);
    fprintf(fpG, "%d\n", c[1]);
    fprintf(fpB, "%d\n", c[2]);
  }

  fclose(fpR); fclose(fpG); fclose(fpB);
}


void DialCut::loadLabeling(const char* name) {
  PortraitCut::loadLabeling(name);
  recreateRelevant();
  for (vector<ushort>::iterator c=_newRelevant.begin(); c!=_newRelevant.end(); ++c) {
    allocateAuxData(*c);
  }
}

//irrelevant for this class
void DialCut::rankImages (std::vector<RowSpan>& spans, std::priority_queue<Ranker>& ordered) {}


// Given stroked region of width _w, height _h, returns compact rep
void DialCut::compactStroke(const short* stroked, std::vector<RowSpan>& spans) const {
  int x,y, index, num;
  for (y=0,index=0; y<_h; ++y) {
    x=0;
    while (x<_w) {
      if (stroked[index]==1) {
	num=0;
	spans.push_back(RowSpan(x,y,0));
	assert(x>=0 && y>=0 &&x<_w && y<_h);
	do {
	  ++x; ++index; ++num;
	} while (x<_w && stroked[index]==1);
	spans[spans.size()-1]._num = num;
      }
      else {
	++x; ++index;
      }
    } // end x<w
  } // end loop over rows
  
}


void DialCut::clearAllStrokes () {
  
  int index = 0;
  for (int x=0; x < _h; ++x) {
    for (int y=0; y < _w; ++y, ++index) {
      _allStrokes[index]._st = S_NONE;
    }
  }
  memset(_labels, 0, _w*_h*sizeof(ushort));
  _relevant.clear ();
}
