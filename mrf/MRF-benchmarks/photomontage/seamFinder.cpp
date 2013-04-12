#include "seamFinder.h"
#include "mrf.h"
#include "ICM.h"
#include "GCoptimization.h"
#include "MaxProdBP.h"
#include "TRW-S.h"
#include "BP-S.h"

#include "runalgs.h"

// count energy truncations in energy.h
#ifdef COUNT_TRUNCATIONS
int truncCnt, totalCnt;
#endif

bool allBad(const Coord& p, const std::vector<Image*>& imgs) {
  std::vector<Image*>::const_iterator it;
  for (it=imgs.begin(); it!=imgs.end(); ++it) {
    if (!((*it)->trans(p.x,p.y)))
      return false;
  }
  return true;
}

//-------

MRF::CostVal smoothCostFn(int pix1, int pix2, int dp,int dq) {
  //printf("yo %d %d %d\n",g_width, g_height, g_sources.size());

  Coord p(pix1%g_width, pix1/g_width);
  Coord q(pix2%g_width, pix2/g_width);

  if (dp==dq) return 0;
  if (allBad(p,g_sources) || allBad(q,g_sources)) return 0;

  if (g_sources[dp]->trans(p.x, p.y) && g_sources[dq]->trans(q.x, q.y))
    return 0;
  if (g_sources[dp]->trans(p.x, p.y) || g_sources[dq]->trans(q.x, q.y) ||
      g_sources[dq]->trans(p.x, p.y) || g_sources[dp]->trans(q.x, q.y))
    return A_INFINITY;
  captype M;
  M = (int)(g_sources[dp]->distTo(p,g_sources[dq]) + g_sources[dp]->distTo(q,g_sources[dq]));
  M+=1;
  assert(M <= 885 && M>=0);
  return M;

}

//-------



SeamFinder::SeamFinder(std::vector<Image*> ims) : _imgs(ims) {
  _w = _imgs[0]->width();
  _h = _imgs[0]->height();
  _im_size.x = _w;
  _im_size.y = _h;
  _n = _imgs.size();

  _labels = new int[_w*_h];
  //initializeResult(); //G! or ?
  _node_vars = new Energy::Var[_w*_h];
  //_allBad = NULL;
}

SeamFinder::~SeamFinder() {
  delete[] _labels;
  delete[] _node_vars;
  //if (_allBad) delete[] _allBad;
}

void SeamFinder::initializeResult() {
  memset(_labels,0, _w*_h*sizeof(int));

  Coord p;
  int i;
  int ind;

  for (p.y=0, ind=0; p.y<_h; p.y++)
    for (p.x=0; p.x<_w; p.x++, ++ind) {
      for (i=0; i<_n; ++i) {
	if (!_imgs[i]->trans(p.x, p.y)) {
	  _labels[ind] = i;
	  break;
	}
      }
    }
}

/*void SeamFinder::initializeAllBad() {
  if (_allBad == NULL)
    _allBad = new bool[_w*_h];
  memset(_allBad, true, _w*_h*sizeof(bool));

  Coord p;
  int i;
  int ind;
  
  for (p.y=0, ind=0; p.y<_h; p.y++)
    for (p.x=0; p.x<_w; p.x++, ++ind) {
      for (i=0; i<_n; ++i) {
	if (!_imgs[i]->trans(p.x, p.y)) {
	  _allBad[ind] = false;
	  break;
	}
      }
    }
    }*/

void SeamFinder::comp(Image* outIm) {
  int i,j,index=0;
   for (j=0; j<_h; ++j)
    for (i=0; i<_w; ++i, ++index) { 
      outIm->setPix(i,j,_imgs[_labels[index]]);
    }
}

void SeamFinder::run() {
  int iter;
  int step_counter = 0, step;
  flowtype E_old;
  initializeResult(); 

  computeEnergy();
  printf("Starting E = %d\n",_E);
  //exit(0);
  for (iter=0; step_counter<_n && iter<5; ++iter) { 
    
    for (step=0; step<_n && step_counter<_n; step++) { 
      if (iter==0 && step==0) continue;
      E_old = _E;
	
      expand(step);

      //assert(_E <= E_old);
      if (E_old <= _E) step_counter ++;
      else            step_counter = 0;      
      printf("iteration %d, label %d, E = %d\n",iter, step, _E);
    }
    
  }

  Image* outIm = new Image(_w,_h);
  comp(outIm);
  outIm->write("simple.png");
  delete outIm;
}

EnergyFunction* SeamFinder::getEnergyFunction(MRF::CostVal* dataCostArray) {
  DataCost *dataCostObj = new DataCost(dataCostArray);
  SmoothnessCost *smoothCostObj = new SmoothnessCost(&smoothCostFn);
  return (new EnergyFunction(dataCostObj, smoothCostObj));

}

/* original version
void SeamFinder::runStudy(int argc, char* argv[]) {

  // Build data cost
  MRF::CostVal* dataCostArray = new int[_w*_h*_n];
  memset(dataCostArray, 0, _w*_h*_n*sizeof(int));
  Coord p;
  int ind=0;
  for (p.y=0; p.y<_h; p.y++)
    for (p.x=0; p.x<_w; p.x++)
      for (int n=0; n<_n; ++n, ++ind)
	dataCostArray[ind] = data_penalty(p,n);
    
  MRF::EnergyVal E;
  float t, tot_t;
  int iter;
  Image* outIm = new Image(_w,_h);
  MRF* mrf;

  EnergyFunction *eng = getEnergyFunction(dataCostArray);
  
  printf("\n\\\\\\\\\\\\\\\\\\Started ICM Algorithm\\\\\\\\\\\\\\\\\n");
  mrf = (MRF*) new ICM(_w,_h,_n, eng);
  mrf->initialize();

  initializeResult();
  memcpy(mrf->getAnswerPtr(), _labels, _w*_h*sizeof(int)); // init their answer
  E = mrf->totalEnergy();
  printf("Energy at the Start= %d\n", E);

  tot_t = 0;
  
  for (iter=0; iter<50; iter++) {  
    mrf->optimize(1,t);
    E = mrf->totalEnergy();
    tot_t = tot_t + t;
    printf("energy = %d (%f secs)\n", E, tot_t );
  }
  memcpy(_labels, mrf->getAnswerPtr(),  _w*_h*sizeof(int)); // copy over their answer
  printf("I think my energy is %d\n", computeEnergy()); 
  comp(outIm);
  outIm->write("icm.png");
  delete mrf;

  printf("\n\\\\\\\\\\\\\\\\\\Started Swap Algorithm\\\\\\\\\\\\\\\\\n");
  mrf = (MRF*) new Swap(_w,_h,_n, eng);
  ((Swap*)mrf)->setLabelOrder(0);
  mrf->initialize();

  bool b[1];b[0] = 0;
  mrf->setParameters(1,&b);

  initializeResult();
  memcpy(mrf->getAnswerPtr(), _labels, _w*_h*sizeof(int)); // init their answer
  E = mrf->totalEnergy();
  printf("Energy at the Start= %d\n", E);

  tot_t = 0;
  
  for (iter=0; iter<10; iter++) {  
    mrf->optimize(1,t);
    E = mrf->totalEnergy();
    tot_t = tot_t + t;
    printf("energy = %d (%f secs)\n", E, tot_t );
  }
  memcpy(_labels, mrf->getAnswerPtr(),  _w*_h*sizeof(int)); // copy over their answer
  printf("I think my energy is %d\n", computeEnergy()); 
  comp(outIm);
  outIm->write("swap.png");

  delete mrf;
  

  printf("\n\\\\\\\\\\\\\\\\\\Started Expansion Algorithm\\\\\\\\\\\\\\\\\n");
  mrf = (MRF*) new Expansion(_w,_h,_n, eng);
  ((Expansion*)mrf)->setLabelOrder(0);
  mrf->initialize();

  initializeResult();
  memcpy(mrf->getAnswerPtr(), _labels, _w*_h*sizeof(int)); // init their answer
  E = mrf->totalEnergy();
  printf("Energy at the Start= %d\n", E);

  tot_t = 0;
  
  for (iter=0; iter<5; iter++) {  
    mrf->optimize(1,t);
    E = mrf->totalEnergy();
    tot_t = tot_t + t;
    printf("energy = %d (%f secs)\n", E, tot_t );
  }
  memcpy(_labels, mrf->getAnswerPtr(),  _w*_h*sizeof(int)); // copy over their answer
  printf("I think my energy is %d\n", computeEnergy()); 
  comp(outIm);
  outIm->write("expansion.png");

  delete mrf;
  

  printf("\n\\\\\\\\\\\\\\\\\\Started BP-S Algorithm\\\\\\\\\\\\\\\\\n");
  mrf = (MRF*) new BPS(_w,_h,_n, eng);
  mrf->initialize();
  //mrf->clearAnswer();

  initializeResult();
  memcpy(mrf->getAnswerPtr(), _labels, _w*_h*sizeof(int)); // init their answer
  E = mrf->totalEnergy();
  printf("Energy at the Start= %d\n", E);

  tot_t = 0;
  
  for (iter=0; iter<200; iter++) {  
    mrf->optimize(1,t);
    E = mrf->totalEnergy();
    tot_t = tot_t + t;
    printf("energy = %d (%f secs)\n", E, tot_t );
  }
  memcpy(_labels, mrf->getAnswerPtr(),  _w*_h*sizeof(int)); // copy over their answer
  printf("I think my energy is %d\n", computeEnergy()); 
  comp(outIm);
  outIm->write("LBPvlad.png");

  delete mrf;
  
  
  printf("\n\\\\\\\\\\\\\\\\\\Started LBP Algorithm\\\\\\\\\\\\\\\\\n");
  mrf = (MRF*) new MaxProdBP(_w,_h,_n, eng);
  mrf->initialize();
  ((MaxProdBP*)mrf)->setExpScale(885);
  mrf->clearAnswer();

  initializeResult();
  memcpy(mrf->getAnswerPtr(), _labels, _w*_h*sizeof(int)); // init their answer
  E = mrf->totalEnergy();
  printf("Energy at the Start= %d\n", E);

  tot_t = 0;
  
  for (iter=0; iter<15; iter++) {  
    mrf->optimize(20,t);
    E = mrf->totalEnergy();
    tot_t = tot_t + t;
    printf("energy = %d (%f secs)\n", E, tot_t );
  }
  memcpy(_labels, mrf->getAnswerPtr(),  _w*_h*sizeof(int)); // copy over their answer
  printf("I think my energy is %d\n", computeEnergy()); 
  comp(outIm);
  outIm->write("LBP.png");

  delete mrf;


  printf("\n\\\\\\\\\\\\\\\\\\Started TRWS Algorithm\\\\\\\\\\\\\\\\\n");
  mrf = (MRF*) new TRWS(_w,_h,_n, eng);
  mrf->dontCacheSmoothnessCosts();
  mrf->initialize();

  initializeResult();
  memcpy(mrf->getAnswerPtr(), _labels, _w*_h*sizeof(int)); // init their answer
  E = mrf->totalEnergy();
  printf("Energy at the Start= %d\n", E);

  tot_t = 0;
  
  for (iter=0; iter<5000; iter++) {   // Should be 5000
    mrf->optimize(1,t);
    E = mrf->totalEnergy();
    tot_t = tot_t + t;
    printf("energy = %d (%f secs)\n", E, tot_t );
  }
  memcpy(_labels, mrf->getAnswerPtr(),  _w*_h*sizeof(int)); // copy over their answer
  printf("I think my energy is %d\n", computeEnergy()); 
  comp(outIm);
  outIm->write("trws.png");

  delete mrf;


  //------------------
  delete outIm;
  delete[] dataCostArray;
  delete eng;
}
*/


// new version, uses runalgs
void SeamFinder::runStudy(char *outstem, int argc, char* argv[]) {
    
    int writeParams = 1;
    int writeTimings = 1;
    int verbose = 1;
    FILE *debugfile = createDebugFile(writeParams, outstem, verbose, argc, argv);

    // Build data cost
    MRF::CostVal* dataCostArray = new int[_w*_h*_n];
    memset(dataCostArray, 0, _w*_h*_n*sizeof(int));
    Coord p;
    int ind=0;
    for (p.y=0; p.y<_h; p.y++)
	for (p.x=0; p.x<_w; p.x++)
	    for (int n=0; n<_n; ++n, ++ind)
		dataCostArray[ind] = data_penalty(p,n);
    
    Image* outIm = new Image(_w,_h);

    EnergyFunction *energy = getEnergyFunction(dataCostArray);

    int MRFalg = aRunAll;

    int width = _w, height = _h, nLabels = _n;
    int outerIter, innerIter;
    MRF *mrf = NULL;
    for (int numAlg = aICM; numAlg <= aBPM; numAlg++) {
	outerIter = MAXITER;
	//outerIter = 10000; // make higher to see what TRWS does
	innerIter = 1;
	if (MRFalg < aRunAll && numAlg != MRFalg) continue;

#ifdef COUNT_TRUNCATIONS
	// only re-run Expansion and Swap (and TRW-S)
	if (!(numAlg == aExpansion || numAlg == aSwap || numAlg == aTRWS)) continue;
#endif


	startAlgInitTimer();

	switch (numAlg) {
	case aICM:       mrf = new ICM(width, height, nLabels, energy); innerIter = 5; break;
	case aExpansion: mrf = new Expansion(width, height, nLabels, energy); break;
	case aSwap:      mrf = new Swap(width, height, nLabels, energy); break;
	case aTRWS:      mrf = new TRWS(width, height, nLabels, energy); break;
	case aBPS:       mrf = new BPS(width, height, nLabels, energy); break;
	case aBPM:       mrf = new MaxProdBP(width, height, nLabels, energy); break; 
	default: fprintf(stderr, "unknown algorithm number"); exit(1);
	}
	if (verbose)
	    fprintf(stderr, "******* Running %s\n", algs[numAlg]);
	if (debugfile)
	    fprintf(debugfile, "******* Running %s for up to %d x %d iterations\n",
		    algs[numAlg], outerIter, innerIter);

	mrf->initialize();
	
	initializeResult();
	memcpy(mrf->getAnswerPtr(), _labels, _w*_h*sizeof(int)); // init their answer

	/* special stuff some algs may need??

  printf("\n\\\\\\\\\\\\\\\\\\Started Swap Algorithm\\\\\\\\\\\\\\\\\n");
  ((Swap*)mrf)->setLabelOrder(0);
  bool b[1];b[0] = 0;
  mrf->setParameters(1,&b);

  printf("\n\\\\\\\\\\\\\\\\\\Started Expansion Algorithm\\\\\\\\\\\\\\\\\n");
  ((Expansion*)mrf)->setLabelOrder(0);
  
  printf("\n\\\\\\\\\\\\\\\\\\Started LBP Algorithm\\\\\\\\\\\\\\\\\n");
  ((MaxProdBP*)mrf)->setExpScale(885);

  printf("\n\\\\\\\\\\\\\\\\\\Started TRWS Algorithm\\\\\\\\\\\\\\\\\n");
  mrf = (MRF*) new TRWS(_w,_h,_n, eng);
  mrf->dontCacheSmoothnessCosts();
  mrf->initialize();
	*/



	float initTime = getAlgInitTime();
	    
	FILE *timefile = createTimeFile(writeTimings, outstem, algs[numAlg], debugfile);

#ifdef COUNT_TRUNCATIONS
    truncCnt = totalCnt = 0;
#endif

	runAlg(mrf, numAlg, debugfile, timefile, outerIter, innerIter, initTime);

#ifdef COUNT_TRUNCATIONS
	if (truncCnt > 0)
	    if (debugfile)
		fprintf(debugfile, "***WARNING: %d terms (%.4f%%) were truncated to ensure regularity\n", 
			truncCnt, (float)(100.0 * truncCnt / totalCnt));
#endif

	memcpy(_labels, mrf->getAnswerPtr(),  _w*_h*sizeof(int)); // copy over their answer
	printf("I think my energy is %d\n", computeEnergy()); 
	comp(outIm);

	char fname[500];
	sprintf(fname, "%s-%s.png", outstem, algs[numAlg]);
	outIm->write(fname);
	delete mrf;
    }
    if (writeParams)
	fclose(debugfile);

    //------------------
    delete outIm;
    delete[] dataCostArray;
    delete energy;
}



captype SeamFinder::data_penalty(const Coord& p, const int d) {
  if (_imgs[d]->trans(p.x, p.y) && !allBad(p,_imgs))
    return A_INFINITY;
  else
    return 0;
}

captype SeamFinder::smoothness_penalty(const Coord& p, const Coord& q, const int d, const int dq) {
  if (d==dq) return 0;
  if (allBad(p,_imgs) || allBad(q,_imgs)) return 0;

  if (_imgs[d]->trans(p.x, p.y) && _imgs[dq]->trans(q.x, q.y))
    return 0;
  if (_imgs[d]->trans(p.x, p.y) || _imgs[dq]->trans(q.x, q.y) ||
      _imgs[dq]->trans(p.x, p.y) || _imgs[d]->trans(q.x, q.y))
    return A_INFINITY;
  captype M;
  //M = (int)_imgs[d]->distTo(p,_imgs[dq]);
  //M += (int)_imgs[d]->distTo(q,_imgs[dq]);
  M = (int)(_imgs[d]->distTo(p,_imgs[dq]) + _imgs[d]->distTo(q,_imgs[dq]));
  M+=1;
  //M /= 6.f;
  return M; // + 0.0001f;
}


//-----------------------------------------------------------------------------------------------------


#define ADD_TERM1(var, E0, E1) add_term1(var, E0, E1)
#define ADD_TERM2(var1, var2, E00, E01, E10, E11) add_term2(var1, var2, E00, E01, E10, E11)
#define VALUE0 0
#define VALUE1 1

flowtype SeamFinder::computeEnergy() {
  int k, ind;
  Coord p, q;
  int d, dq;

  _E = 0;

  for (p.y=0, ind=0; p.y<_h; p.y++)
    for (p.x=0; p.x<_w; p.x++, ++ind) {
      d = _labels[ind];  //Coord(IMREF(x_left, p), IMREF(y_left, p));
      
      _E += data_penalty(p, d);
      
      for (k=0; k<(int)NEIGHBOR_NUM; k++) {
	q = p + NEIGHBORS[k];
	
	if (q>=Coord(0,0) && q<_im_size) {
	  dq = _labels[REFP(q)];
	  _E += smoothness_penalty(p, q, d, dq);
	}
      }
    }

  return _E;
}

void error_function(char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

#define VAR_ACTIVE ((Energy::Var)0)

void SeamFinder::expand(int a) {
  Coord p, q;
  int d,dq;
  Energy::Var var, qvar;
  captype E00=0, E0a=0, Ea0=0;
  flowtype E_old;
  int k, ind;

  /* node_vars stores variables corresponding to nodes */

  Energy *e = new Energy(error_function);

  /* initializing */
  for (p.y=0, ind=0; p.y<_im_size.y; p.y++)
    for (p.x=0; p.x<_im_size.x; p.x++, ++ind)
      {
	d = _labels[ind];
	if (a == d/* || _imgs[a]->trans(p.x, p.y)*/) // NOTE
	  {
	    _node_vars[ind] = VAR_ACTIVE;
	    //IMREF(node_vars, p) = VAR_ACTIVE;
	    e -> add_constant(data_penalty(p, d));
	  }
	else
	  {
	     _node_vars[REFP(p)] = var = e -> add_variable();
	    e -> ADD_TERM1(var, data_penalty(p, d), data_penalty(p, a));
	  }
      }

  for (p.y=0,ind=0; p.y<_im_size.y; p.y++)
    for (p.x=0; p.x<_im_size.x; p.x++, ++ind)
      {
	d = _labels[ind];
	var = _node_vars[REFP(p)];

	/* smoothness term */
	for (k=0; k<(int)NEIGHBOR_NUM; k++)
	  {
	    q = p + NEIGHBORS[k];
	    if ( ! ( q>=Coord(0,0) && q<_im_size ) ) continue;
	    qvar = _node_vars[REFP(q)];
	    dq = _labels[REFP(q)];

	    if (var != VAR_ACTIVE && qvar != VAR_ACTIVE)
	      E00 = smoothness_penalty(p, q, d, dq);
	    if (var != VAR_ACTIVE)
	      E0a = smoothness_penalty(p, q, d, a);
	    if (qvar != VAR_ACTIVE)
	      Ea0 = smoothness_penalty(p, q, a, dq);

	    if (var != VAR_ACTIVE)
	      {
		if (qvar != VAR_ACTIVE) {
		  //if (E00 > E0a + Ea0) // G! truncation
		  //E00 = E0a + Ea0;
		  e -> ADD_TERM2(var, qvar, E00, E0a, Ea0, 0);
		}
		else                    
		  e -> ADD_TERM1(var, E0a, 0);
	      }
	    else
	      {
		if (qvar != VAR_ACTIVE) 
		  e -> ADD_TERM1(qvar, Ea0, 0);
		//else                    {}
	      }
			
	  }
      }

  E_old = _E;
  _E = e -> minimize();
  printf("old, new %d %d\n",E_old, _E);

  if (_E < E_old)
    {
      for (p.y=0,ind=0; p.y<_im_size.y; p.y++)
	for (p.x=0; p.x<_im_size.x; p.x++, ++ind)
	  {
	    var = _node_vars[REFP(p)];
	    if (var!=VAR_ACTIVE && e->get_var(var)==VALUE1)
	      {
		_labels[REFP(p)] = a;
	      }
	  }
    }
  //printf("yikes %.10f\n",computeEnergy());
  delete e;

}


