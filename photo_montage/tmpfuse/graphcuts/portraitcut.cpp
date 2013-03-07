#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "portraitcut.h"
#include "graph.h"
#include <wx/wx.h>

extern wxCondition* finishedCondition;
//extern wxMutex* labelingMutex;
//extern wxMutex* finishedMutex;
//extern wxMutex* dataMutex;

DEFINE_EVENT_TYPE (TH_REFRESH_VIEWER)
DEFINE_EVENT_TYPE (END_GLOBAL_COMPUTE)


PortraitCut::PortraitCut(int oWidth, int oHeight, vector<ImageAbs*>* images, ushort* result, FILE* fp) 
  /*: wxThread(wxTHREAD_JOINABLE)*/ {
  _n = images->size();
  _wh = oWidth*oHeight;
  _w = oWidth; _h = oHeight;
  _size.x = _w; _size.y=_h;
  //_required = new short[_w*_h];
  //_ownRequired = true;
  _idata = new ImageData(images,_w, _h);
  _labels = result;
  if (fp)
    _fp = fp;
  else {
    _fp = fopen("notspecified.txt","w");
  }
  assert(_fp);

  mutualInit();

}

PortraitCut::PortraitCut(int oWidth, int oHeight, vector<ImageAbs*>* images, short* required, ushort* result, FILE* fp) {

  _n = images->size();
  _wh = oWidth*oHeight;
  _w = oWidth; _h = oHeight;
  _size.x = _w; _size.y=_h;
  //_required = required;
  //_ownRequired = false;
  _idata = new ImageData(images,_w, _h);
  _labels = result;
 
  if (fp)
    _fp = fp;
  else {
    _fp = fopen("notspecified.txt","w");
  }

  mutualInit();
  //for (int i=0; i<_images.size(); ++i)
  //_images[i] = new ImageAbs(*((*images)[i]));


  //saveLabels("out.pgm");


  //int w = (*images)[0]->_size.x, h = (*images)[0]->_size.y;
  //RGBImage im = (RGBImage) imNew(IMAGE_RGB, w, h);
  //memcpy(im->data, (*images)[0]->_data,w*h*3);

  //int res = imSave(im,"test.ppm");
  //assert(res==0);
  //imFree(im);
}

void PortraitCut::mutualInit() {
  //_cgradsComputed = false;
  _th_frame = NULL;
  _cuttype = C_NORMAL; //C_GRAD2; 


  indeces_a = (PtrImage) imNew(IMAGE_PTR, _w, _h);
  D_a = (DoubleImage) imNew(IMAGE_DOUBLE, _w, _h);


  
  //allocateComputeAllCGrads(); // 
}

void PortraitCut::allocateAuxData(const ushort i) {
  printf("allocating aux data for %u\n", i);
  if (_cuttype == C_GRAD)
    _idata->allocateComputeGradMag(i);
  else if (_cuttype == C_GRAD2 || _cuttype == C_BOTH)
    _idata->allocateComputeCGrad(i);  
}

void PortraitCut::allocateAllAuxData() {
  for (int i=0; i<_n; ++i) 
    allocateAuxData(i);
}


void PortraitCut::addImage(ImageAbs* im) {
  ++_n;
  _idata->addImage(im);
}

void PortraitCut::removeLastImage() {
  --_n;
  _idata->removeLastImage();
}

void PortraitCut::compute() {
  _myStop = false;
  int iter;
  int step_counter = 0, step;
  double E, E_old;

  /*initializeResult();
  for (step=0; step<_n; ++step) {
  float score = 0;
  for (int j=0; j<_h; ++j)
  for (int i=0; i<_w; ++i) 
  score += BVZ_data_penalty(Coord(i,j), step);
  fprintf(_fp,"Step %d scores %f\n",step, score); fflush(_fp);
  }
  return;*/


  fprintf(_fp, "%d images:\n",_n);
  //for (iter=0; iter<_n; ++iter)
  //fprintf(_fp,"dim: %d %d, displace: %d %d\n",
  //(*_images)[iter]->_size.x,(*_images)[iter]->_size.y,
  //  (*_images)[iter]->_displacement.x,(*_images)[iter]->_displacement.y);
  fflush(_fp);


  //initializeResult();  
  E = BVZ_ComputeEnergy();
  fprintf(_fp,"Starting E = %f\n",E);
  //exit(0);
  for (iter=0; iter<_ITER_MAX_ && step_counter<_n; ++iter) { 
    
    for (step=0; step<_n && step_counter<_n; step++) { 
      E_old = E;

	  
      //while (!SINGLE_THREAD && dataMutex->Lock () != wxMUTEX_NO_ERROR)
      //Sleep (10);
      
      E = BVZ_Expand(step, E_old);
      //if(!SINGLE_THREAD)
      //dataMutex->Unlock ();

      fprintf(_fp,"new %f\n",E);
      //assert (E <= E_old);
      if (E_old == E) step_counter ++;
      else            step_counter = 0;      
      fprintf(_fp,"iteration %d, label %d, E = %f\n",iter, step, E);
      fflush(_fp);


      /*if (E < E_old && !SINGLE_THREAD) {
	wxCommandEvent evt(TH_REFRESH_VIEWER);
	wxPostEvent (_th_frame, evt);
	wxTheApp->Yield();
	//Sleep (10);
      }

      if (!SINGLE_THREAD && _myStop)
	return;
      */

      if (_myStop) return;
      wxCommandEvent evt(TH_REFRESH_VIEWER);
      wxPostEvent (_th_frame, evt);
      wxTheApp->Yield();
    }
    
  }
  /*
  if (!SINGLE_THREAD) {
    wxCommandEvent evt(TH_REFRESH_VIEWER);
    _th_frame->ProcessEvent(evt);
    wxCommandEvent evt2(END_GLOBAL_COMPUTE);
    wxPostEvent(_th_frame, evt2);
  }
  */
}



PortraitCut::~PortraitCut() {
  imFree(indeces_a); imFree(D_a);
  //if (_ownRequired) delete[] _required;
  //delete[] _labels;
}


void PortraitCut::loadLabeling(const char* name) {
  fprintf(_fp, "Trying to load file %s\n",name); fflush(_fp);
  FILE* fp;
  
  if ( (fp = fopen(name, "rb")) == NULL) {
    assert(0);
    exit(0);
  }
  
  int width, height, maxComp;
  char cookie[3];
  fscanf(fp, "%2s", &cookie);
  if (strcmp("P5", cookie)) { assert(0);exit(0);} /* not right file type */
  //skipComment(fin);
  fscanf(fp, "%d", &width);
  //skipComment(fin);	
  fscanf(fp, "%d", &height);
  //skipComment(fin);		
  fscanf(fp, "%d", &maxComp);
  fread(cookie, 1, 1, fp); /* Read newline which follows maxval */  

  fprintf(_fp,"width %d height %d numComp %d\n",width,height,maxComp); fflush(_fp);
  
  if (maxComp <1 || maxComp > 255) {assert(0);exit(0);}
  if (_w != width || _h != height) {assert(0);exit(0);}
  unsigned int wh = width*height;
  unsigned char* data = new unsigned char[wh];
  size_t res = fread(data, sizeof(unsigned char), wh, fp);
  if (res!=wh) {assert(0);exit(0);}
  
  fclose(fp);
 
  for (int i=0; i<(int)wh; ++i)
     _labels[i] = (ushort) data[i];
  delete[] data;

}

float PortraitCut::getCurrDataTerm(const int i, const int j) {
  assert(i>=0 && i<_w && j>=0 && j<_h);
  Coord p(i,j);
  return BVZ_data_penalty(p, _labels[j*_w+i]);
}

float PortraitCut::getCurrMaxInterTerm(const int i, const int j) {
  assert(i>=0 && i<_w && j>=0 && j<_h);
  float max = FLT_MIN;
  Coord p(i,j);
  if (i>0)
    max = MAX(max, BVZ_interaction_penalty(p, Coord(i-1,j), getLabel(i,j), getLabel(i-1,j)));
  if (i<_w-1)
    max = MAX(max, BVZ_interaction_penalty(p, Coord(i+1,j), getLabel(i,j), getLabel(i+1,j)));
  if (j>0)
    max = MAX(max, BVZ_interaction_penalty(p, Coord(i,j-1), getLabel(i,j), getLabel(i,j-1)));
  if (j<_h-1)
    max = MAX(max, BVZ_interaction_penalty(p, Coord(i,j+1), getLabel(i,j), getLabel(i,j+1)));
  return max;
}

void PortraitCut::saveLabels(const char* name) {
  fprintf(_fp, "Trying to save file %s\n",name); fflush(_fp);
  GrayImage im = (GrayImage) imNew(IMAGE_GRAY, _w, _h);
  int i,  j,index=0;
  for (j=0; j<_h; j++)
    for (i=0; i<_w; i++, ++index) { 
      Coord p(i,j);
      IMREF(im,p) = (unsigned char) _labels[index];
    }

  imSave(im,name);
  imFree(im);
}

unsigned char* PortraitCut::comp() {
  //RGBImage im = (RGBImage) imNew(IMAGE_RGB, _w, _h);
  unsigned char* im = new unsigned char[3*_w*_h];
  int i,  j,index=0, cindex=0;
  for (j=0; j<_h; j++)
    for (i=0; i<_w; i++, ++index, cindex+=3) { 
      Coord p(i,j);
      unsigned char* cref = _imptr(_labels[index], p);
      //IMREF(im,p).r = cref[0];
      //IMREF(im,p).g = cref[1];
      //IMREF(im,p).b = cref[2];
      im[cindex] = cref[0];
      im[cindex+1] = cref[1];
      im[cindex+2] = cref[2];
    }

  
  return  im; //(unsigned char* ) im->data;
}

void PortraitCut::saveComp(char* name) {
  RGBImage im = (RGBImage) imNew(IMAGE_RGB, _w, _h);
    int i,  j,index=0;
  for (j=0; j<_h; j++)
    for (i=0; i<_w; i++, ++index) { 
      Coord p(i,j);
      unsigned char* cref = _imptr(_labels[index], p);
      IMREF(im,p).r = cref[0];
      IMREF(im,p).g = cref[1];
      IMREF(im,p).b = cref[2];
    }

  int res = imSave(im,name);
  assert(res==0);
  imFree(im);
}


void PortraitCut::initializeResult() { // random initialization now, not particularily good
  int i,  j,index=0;
  //ushort n, minI;  
  //float minData, tmp;
  fprintf(_fp,"Initializing result\n"); fflush(_fp);
  for (j=0; j<_h; j++)
    for (i=0; i<_w; i++, ++index) { 
      _labels[index] = 0;
      /*
      if (_required[index]==-1) {
	//double rnd = rand();
	_labels[index] = 0; //(unsigned char) (double(_n)*rnd/(RAND_MAX+1.0));
	
	
	minData = FLT_MAX; minI = 0;
	Coord p(i,j);
	for (n=0; n<_n; ++n)
	  if ((tmp=BVZ_data_penalty(p, n)) < minData) {
	    minI = n; minData = tmp;
	  }
	  _labels[index] = minI;

      }
      else
	_labels[index] = _required[index]; 
      */
    }
  fprintf(_fp,"Done initializing result\n"); fflush(_fp);
  
}

//data penalty of pixel index having label d
float PortraitCut::BVZ_data_penalty(Coord p, ushort d) {
  assert(0);
  Coord dp = p;
  _displace(dp,d);

  if (dp>=Coord(0,0) && dp<_idata->images(d)->_size) {  
  
    //int index = CINDEX(p);
    /*if (_required[index] != -1) {
      if (d==_required[index]) // good
	return 0.;
      else                         // bad
	return A_INFINITY;
	}*/
    
    return 500.; // we don't care either way, but want to make sure 0, A_INFINITY meaningful
  } 
  else           // penalize labels which fall outside of displaced images
    return A_INFINITY;
  
}

float PortraitCut::BVZ_interaction_penalty(Coord p, Coord np, ushort l, ushort nl) { 
  int c,  k;
  float a,M=0;

  assert(l<_n);
  assert(l<_n);

  if (l==nl) return 0;


  unsigned char *Il, *Inl;

  if (_cuttype == C_NORMAL || _cuttype == C_GRAD) {
    // difference at p pixel
    a=0;
    Il = _imptr(l,p); 
    Inl = _imptr(nl,p);  
    for (c=0; c<3; ++c) {
      k = Il[c] - Inl[c];
      a += k*k;    
    }
    M = sqrt(a);
    
    // difference at np pixel
    a=0;
    Il = _imptr(l,np); 
    Inl = _imptr(nl,np);  

    for (c=0; c<3; ++c) {
      k = Il[c] - Inl[c];
      a += k*k;    
    }
    M += sqrt(a);
    
    
    M /=6.f;
    
    // gradient denominator
    if (_cuttype == C_GRAD) {
      float G;
      if (p.x!=np.x) {  // vertical cut, vertical Sobel filter
	Coord minp(min(p.x,np.x), p.y);
	if (p.y>0 && p.y<_h-1) {
	  G = .5f*(_idata->vertGradMagLookup(l,minp) + _idata->vertGradMagLookup(nl,minp));
	  //G = MIN(vertGradMagLookup(l,minp), vertGradMagLookup(nl,minp));
	}
	else
	  G = 1.f;
      }
      else {  // horizontal cut, horizontal Sobel filter
	Coord minp(p.x, min(p.y,np.y));
	if (p.x>0 && p.x<_w-1) {
	  G = .5f*(_idata->horizGradMagLookup(l,minp) + _idata->horizGradMagLookup(nl,minp));
	  //G = MIN(horizGradMagLookup(l,minp), horizGradMagLookup(nl,minp));
	}
	else
	  G = 1.f;
      }
      
      
      if (G==0)
	M = A_INFINITY;
      else
	M /= G;
    }
  }

  else if (_cuttype == C_GRAD2) {
    // difference at p pixel    
    const CGrad& acgrad = _idata->cgradLookup(l,p);
    const CGrad& bcgrad = _idata->cgradLookup(nl,p);
    M = acgrad.normFrom(bcgrad);

    // difference at np pixel
    const CGrad& acgrad2 = _idata->cgradLookup(l,np);
    const CGrad& bcgrad2 = _idata->cgradLookup(nl,np);
    M += acgrad2.normFrom(bcgrad2);
  }

  else if (_cuttype == C_BOTH) {
    // difference at p pixel    
    const CGrad& acgrad = _idata->cgradLookup(l,p);
    const CGrad& bcgrad = _idata->cgradLookup(nl,p);
    M = acgrad.normFrom(bcgrad);

    // difference at np pixel
    const CGrad& acgrad2 = _idata->cgradLookup(l,np);
    const CGrad& bcgrad2 = _idata->cgradLookup(nl,np);
    M += acgrad2.normFrom(bcgrad2);
  
    // difference at p pixel
    a=0;
    Il = _imptr(l,p); 
    Inl = _imptr(nl,p);  
    for (c=0; c<3; ++c) {
      k = Il[c] - Inl[c];
      a += k*k;    
    }
    M += sqrt(a);
    
    // difference at np pixel
    a=0;
    Il = _imptr(l,np); 
    Inl = _imptr(nl,np);  
    
    for (c=0; c<3; ++c) {
      k = Il[c] - Inl[c];
      a += k*k;    
    }
    M += sqrt(a);

    M *= .1;
  }

  if (M>A_INFINITY) M = A_INFINITY;
  //printf("%d ",d);
  //assert(_finite(M) && !_isnan(M));
  return M;
}



//--------------------------------------------

double PortraitCut::BVZ_ComputeEnergy() { 
  
  double E=0;
  
  int i,j,index=0, k;
  Coord np;
  for (j=0; j<_h; j++)
    for (i=0; i<_w; i++, ++index) {
      Coord coord(i,j);
      E += BVZ_data_penalty(coord,_labels[index]);
      for (k=0; k<(int)NEIGHBOR_NUM; k++){
	np = coord + NEIGHBORS[k];
	if (np>=Coord(0,0) && np<_size)
	  E += BVZ_interaction_penalty(coord,np,_labels[index], _labels[CINDEX(np)]);	
	
      }

    }

  //assert(_finite(E) && !_isnan(E));
  return E;
}


double PortraitCut::BVZ_ComputeSpatialOnly() { 
  
  double E=0;
  
  int i,j,index=0, k;
  Coord np;
  for (j=0; j<_h; j++)
    for (i=0; i<_w; i++, ++index) {
      Coord coord(i,j);
      for (k=0; k<(int)NEIGHBOR_NUM; k++){
	np = coord + NEIGHBORS[k];
	if (np>=Coord(0,0) && np<_size)
	  E += BVZ_interaction_penalty(coord,np,_labels[index], _labels[CINDEX(np)]);	
	
      }

    }

  //assert(_finite(E) && !_isnan(E));
  return E;
}


double PortraitCut::BVZ_ComputeDataOnly() { 
  
  double E=0;
  
  int i,j,index=0;
  for (j=0; j<_h; j++)
    for (i=0; i<_w; i++, ++index) {
      Coord coord(i,j);
      E += BVZ_data_penalty(coord,_labels[index]);
    }
  
  //assert(_finite(E) && !_isnan(E));
  return E;
}

//-----------------------------


#ifdef BVZ_ALPHA_SINK
#define BVZ_TERM_A Graph::SOURCE
#define BVZ_TERM_B Graph::SINK
#else
#define BVZ_TERM_A Graph::SINK
#define BVZ_TERM_B Graph::SOURCE
#endif

void BVZ_error_function(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	assert(0);
	exit(1);
}




double PortraitCut::BVZ_Expand(ushort a, double E_old) { 

  
  Coord p, np;
  ushort l, nl;
  Graph *g;
  float delta, P_00, P_0a, P_a0;
  Graph::node_id index, nindex;
  int k, ind=0;
  double E;
  
  /* indeces_a stores node indeces */
  /* D_a sto res penalties for assigning label a */
  
  g = new Graph(BVZ_error_function);
  
  /* initializing */
  E = 0.;
  for (p.y=0; p.y<_h; p.y++)
    for (p.x=0; p.x<_w; p.x++, ++ind)
      {
	l = _labels[ind];
	if (a == l)
	  {
	    IMREF(indeces_a, p) = INDEX_ACTIVE;
	    E += BVZ_data_penalty(p, l);
	    continue;
	  }
	
	IMREF(indeces_a, p) = g -> add_node();
	delta = BVZ_data_penalty(p, l);
	IMREF(D_a, p) = BVZ_data_penalty(p, a) - delta;
	E += delta;
      }
  
  ind=0;
  for (p.y=0; p.y<_h; p.y++)
    for (p.x=0; p.x<_w; p.x++, ++ind)
      {
	l = _labels[ind];
	index = (Graph::node_id) IMREF(indeces_a, p);
	
	/* adding interactions */
	for (k=0; k<(int)NEIGHBOR_NUM; k++)
	  {
	    np = p + NEIGHBORS[k];
	    if ( ! ( np>=Coord(0,0) && np<_size ) ) continue; //HUM
	    nl = _labels[CINDEX(np)];
	    nindex = (Graph::node_id) IMREF(indeces_a, np);
	    
	    if (IS_NODE_ID(index))
	      {
		if (IS_NODE_ID(nindex))
		  {
		    P_00 = BVZ_interaction_penalty(p, np, l, nl);
		    P_0a = BVZ_interaction_penalty(p, np, l,  a);
		    P_a0 = BVZ_interaction_penalty(p, np, a, nl);
		    delta = (P_00 <  P_0a) ? P_00 : P_0a;
 		    if (delta > 0)
		      {
			IMREF(D_a, p) -= delta; E += delta;
			P_00 -= delta;
			P_0a -= delta;
		      }
		    delta = (P_00 < P_a0) ? P_00 : P_a0;
		    if (delta > 0)
		      {
			IMREF(D_a, np) -= delta; E += delta;
			P_00 -= delta;
			P_a0 -= delta;
		      }
		    if (P_00 > 0.0001) { fprintf(_fp, "ERROR: BVZ_interaction_penalty() is non-metric %f!\n",P_00); fflush(_fp); /*assert(0);*/ }
#ifdef BVZ_ALPHA_SINK
		    g -> add_edge(index, nindex, P_0a, P_a0);
#else
		    g -> add_edge(index, nindex, P_a0, P_0a);
#endif
		  }
		else
		  {
		    delta = BVZ_interaction_penalty(p, np, l, a);
		    IMREF(D_a, p) -= delta; E += delta;
		  }
	      }
	    else
	      {
		if (IS_NODE_ID(nindex))
		  {
		    delta = BVZ_interaction_penalty(p, np, a, nl);
		    IMREF(D_a, np) -= delta; E += delta;
		  }
	      }
	  }
      }
  
  
  /* adding source and sink edges */
  for (p.y=0; p.y<_h; p.y++)
    for (p.x=0; p.x<_w; p.x++)
      {
	index = (Graph::node_id) IMREF(indeces_a, p);
	if (IS_NODE_ID(index))
	  {
	    delta = (float) IMREF(D_a, p);
#ifdef BVZ_ALPHA_SINK
	    if (delta > 0) { g -> set_tweights(index, delta, 0); }
	    else           { g -> set_tweights(index, 0, -delta); E += delta; }
#else
	    if (delta > 0) { g -> set_tweights(index, 0, delta); }
	    else           { g -> set_tweights(index, -delta, 0); E += delta; }
#endif
	  }
      }
  
  E += g -> maxflow();

  //fprintf(_fp, "internal E: %f\n",E); fflush(_fp);
  if (E < E_old)
    {
      //fprintf(_fp,"Writing into _labels\n"); fflush(_fp);
      ind=0;
      for (p.y=0; p.y<_h; p.y++)
	for (p.x=0; p.x<_w; p.x++, ++ind)
	  {
	    index = (Graph::node_id) IMREF(indeces_a, p);
	    if (IS_NODE_ID(index) && g->what_segment(index)==BVZ_TERM_B)
	      {
		_labels[ind] = a;
	      }
	  }
      delete g;
      return E;
    }
  
  delete g;
  return E_old;
}


float CGrad::normFrom(const CGrad& o) const {
  return sqrt(SQR(_x[0] - o._x[0]) + 
	      SQR(_x[1] - o._x[1]) + 
	      SQR(_x[2] - o._x[2]) + 
	      SQR(_y[0] - o._y[0]) + 
	      SQR(_y[1] - o._y[1]) + 
	      SQR(_y[2] - o._y[2]) 
	      );
}

//#pragma managed


