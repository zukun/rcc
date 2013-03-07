#include <iostream>
#include <time.h>
#include "blend.h"
#include "../steppableLinearSolver.h"
using namespace std;

namespace Adrien {

  
  GlobalBlender::GlobalBlender(int w, int h, std::vector<ImageAbs*> * imgs, 
			       unsigned char* final, Areal edgeThres, FILE* fp) : 
    /*wxThread(wxTHREAD_JOINABLE),*/ _w(w), _h(h), _edgeThres(edgeThres), _fp(fp), _result(final) {
    _wh = _w*_h;
    _pixelPin = 0;
    _numImages = (int) imgs->size();
    _linkTypeLookup = new PixLinkTypes[numPixels()];

    //_x = new Areal[numVariables()];
    //_b = new Areal[numVariables()];
    //memset(_x, 0, sizeof(Areal)*numVariables()); // not really necessary

    //_x = NULL; _b = NULL; // historical
    _xR = new Areal[numVariables()];
    _xG = new Areal[numVariables()];
    _xB = new Areal[numVariables()];
    _bR = new Areal[numVariables()];
    _bG = new Areal[numVariables()];
    _bB = new Areal[numVariables()];

    _labels = new ushort[numPixels()];
    _newLabels = NULL;
    _imgMap = new int[_numImages];
    for (int i=0; i<_numImages; ++i)
      _imgMap[i]=-1;
    _imgs = *imgs;

    // May or may not be good idea to assume base image should play a role,
    // but makes it easier to establish initial condition
    memset(_labels, 0, numPixels()*sizeof(ushort));

    if (_fp==NULL) 
      _fp = fopen("blendingNotes.txt","w");

    buildOffsetImage(0);
    /*
    for (int c=0; c<3; ++c) {
      buildInitial(c, _labels);
      pasteResultIntoFinal(c, _labels);
    }
    */
    
    buildInitial(0, _xR, _labels);
    buildInitial(1, _xG, _labels);
    buildInitial(2, _xB, _labels);
    pasteResultIntoFinal(0, _xR, _labels);
    pasteResultIntoFinal(1, _xG, _labels);
    pasteResultIntoFinal(2, _xB, _labels);
    
  }
  
  GlobalBlender::~GlobalBlender() {
    delete[] _linkTypeLookup;
    delete[] _labels;
    delete[] _imgMap;
    //delete[] _x;
    //delete[] _b;

    delete[] _xR; delete[] _xG; delete[] _xB; 
    delete[] _bR; delete[] _bG; delete[] _bB; 
  }

  Areal GlobalBlender::getE(const int aI, const int bI, const ushort* labels, const bool vertCut) {
    Areal sumA=0, sumB=0, d;
    const int aIm = labelImToOffsetIm(labels[aI]), bIm = labelImToOffsetIm(labels[bI]);
    int ax = aI%_w, ay = aI / _w;
    for (int c=0; c<3; ++c) {  // iterate over colors
      
      if (vertCut) { //vertical cut
	if (ay==0 || ay==_h-1) 
	  d = Areal(getImgValue(aI,c,aIm)) - Areal(getImgValue(bI,c,aIm));
	else {
	  d = Areal(getImgValue(aI-_w,c,aIm)) + 2.*Areal(getImgValue(aI,c,aIm)) + Areal(getImgValue(aI+_w,c,aIm)) -
	    Areal(getImgValue(bI-_w,c,aIm)) - 2.*Areal(getImgValue(bI,c,aIm)) - Areal(getImgValue(bI+_w,c,aIm));
	  d /= 3.;
	}

	sumA += d*d;
	
	if (ay==0 || ay==_h-1) 
	  d = Areal(getImgValue(aI,c,bIm)) - Areal(getImgValue(bI,c,bIm));
	else {
	  d = Areal(getImgValue(aI-_w,c,bIm)) + 2.*Areal(getImgValue(aI,c,bIm)) + Areal(getImgValue(aI+_w,c,bIm)) -
	    Areal(getImgValue(bI-_w,c,bIm)) - 2.*Areal(getImgValue(bI,c,bIm)) - Areal(getImgValue(bI+_w,c,bIm));
	  d /= 3.;
	}

	sumB += d*d;

      } 
      else { // horizontal cut
	if (ax==0 || ax==_w-1)
	  d = Areal(getImgValue(aI,c,aIm)) - Areal(getImgValue(bI,c,aIm));
	else {
	  d = Areal(getImgValue(aI-1,c,aIm)) + 2.*Areal(getImgValue(aI,c,aIm)) + Areal(getImgValue(aI+1,c,aIm)) -
	    Areal(getImgValue(bI-1,c,aIm)) - 2.*Areal(getImgValue(bI,c,aIm)) - Areal(getImgValue(bI+1,c,aIm));
	  d /= 3.;
	}

	sumA += d*d;

	if (ax==0 || ax==_w-1)
	  d = Areal(getImgValue(aI,c,bIm)) - Areal(getImgValue(bI,c,bIm));
	else {
	  d = Areal(getImgValue(aI-1,c,bIm)) + 2.*Areal(getImgValue(aI,c,bIm)) + Areal(getImgValue(aI+1,c,bIm)) -
	    Areal(getImgValue(bI-1,c,bIm)) - 2.*Areal(getImgValue(bI,c,bIm)) - Areal(getImgValue(bI+1,c,bIm));
	  d /= 3.;
	}

	sumB += d*d;
	
      }
      
    } // end iterate over colors

    sumA = sqrt(sumA);
    sumB = sqrt(sumB);
    return sumA + sumB;
  }


  void GlobalBlender::computeEdgeMap(const ushort* labels)  { 
    int index=0,x,y;
    Areal E;
    for (y=0; y<_h; ++y)
      for (x=0; x<_w; ++x, ++index) {

	// right
	if (x==_w-1)   // out of bounds
	  _linkTypeLookup[index].r = L_IGNORE;
	else if (labels[index] == labels[index+1])  // same label, no cut
	  _linkTypeLookup[index].r = L_NORMAL;
	else {         // vertical cut, blend or don't care?
	  E = getE(index, index+1, labels, true);
	  if (E < _edgeThres)  // not an edge
	    _linkTypeLookup[index].r = L_BLEND;
	  else
	    _linkTypeLookup[index].r = L_IGNORE;
	}

	// bottom 
	if (y==_h-1)
	  _linkTypeLookup[index].b = L_IGNORE;
	else if (labels[index] == labels[index+_w])
	  _linkTypeLookup[index].b = L_NORMAL;
	else {
	  E = getE(index, index+_w, labels, false);
	  if (E < _edgeThres)  // not an edge
	    _linkTypeLookup[index].b = L_BLEND;
	  else
	    _linkTypeLookup[index].b = L_IGNORE;
	}

      } // end iteration over pixels

  }

  /*  void GlobalBlender::buildInitial(const int c, const ushort* newLabels) {
    for (int i=0; i<numVariables(); ++i) // iterate over variables
      _x[i] = (Areal) getImgValue(varToPixelIndex(i), c, newLabels);
      }

  void GlobalBlender::modifyInitial(const int c, const ushort* newLabels) {
    int pixIndex, i;
    for (i=0; i<numVariables(); ++i) {
      pixIndex = varToPixelIndex(i);
      if (newLabels[pixIndex] != _labels[pixIndex])
	_x[i] = (Areal) getImgValue(pixIndex, c, newLabels);
      else
	_x[i] = (Areal) getResultVal(pixIndex, c);
    }

    }*/

  void GlobalBlender::buildInitial(const int c, Areal* x, const ushort* newLabels) {
    //wxMutexLocker lockX(_xMutex);
    for (int i=0; i<numVariables(); ++i) // iterate over variables
      x[i] = (Areal) getImgValue(varToPixelIndex(i), c, newLabels);
  }

  void GlobalBlender::modifyInitial(const int c, Areal* x, const ushort* newLabels) {
    //printf("modify initial\n");
    //wxMutexLocker lockX(_xMutex);
    int pixIndex, i;
    for (i=0; i<numVariables(); ++i) {
      pixIndex = varToPixelIndex(i);
      if (newLabels[pixIndex] != _labels[pixIndex]) 
	x[i] = (Areal) getImgValue(pixIndex, c, newLabels);
      else
	x[i] = (Areal) getResultVal(pixIndex, c);
    }

  }

  // given new labeling, makes sure _offsetImgs has necessary offset image data
  void GlobalBlender::updateOffsetImgs(const ushort* newLabels) { 
    int index=0, lab;
    for (int j=0; j<_h; ++j)
      for (int i=0; i<_w; ++i, ++index) {
	lab = newLabels[index];
	if (_imgMap[lab] == -1) 
	  buildOffsetImage(lab);
      }
  }

  void GlobalBlender::buildOffsetImage(const int i) {
    unsigned char* newIm = new unsigned char[3*numPixels()];
    _offsetImgs.push_back(newIm);
    _imgMap[i] = int(_offsetImgs.size()) - 1;
    ImageAbs* ia = _imgs[i];
    
    for (int x = 0 ; x < _w ; x++) 
      for (int y = 0 ; y < _h ; y++) {
	Coord prime(x,y);
	ia->displace(prime);
	int xPrime = prime.x, yPrime = prime.y;
	//int xPrime = x - ia->displacement().x;
	//int yPrime = y - ia->displacement().y;
	if ((xPrime < 0) || (xPrime >= ia->_size.x) ||
	    (yPrime < 0) || (yPrime >= ia->_size.y)) {
	  newIm[imgIndexRGB(x,y,_w) + 0] = 0;
	  newIm[imgIndexRGB(x,y,_w) + 1] = 0;
	  newIm[imgIndexRGB(x,y,_w) + 2] = 0;
	}
	else {
	  const unsigned char* I = ia->data(xPrime, yPrime);
	  newIm[imgIndexRGB(x,y,_w) + 0] = I[0]; 
	  newIm[imgIndexRGB(x,y,_w) + 1] = I[1];
	  newIm[imgIndexRGB(x,y,_w) + 2] = I[2];
	}
      } // x,y

    /*ImageAbs* shit = new ImageAbs(_w, _h, newIm); 
      char filename[100];
      sprintf(filename,"shit%.2d.ppm",i);
      shit->writePPM(filename);
      delete shit;*/
  }

  void GlobalBlender::takeComputeData(bool reInit, const ushort* newLabels, Areal edgeThres) { 
    _myStop = false;
    _edgeThres = edgeThres;
    updateOffsetImgs(newLabels);
    computeEdgeMap(newLabels);
    if (reInit) {
      buildInitial(0, _xR, newLabels);
      buildInitial(1, _xG, newLabels);
      buildInitial(2, _xB, newLabels);
    }
    else {
      modifyInitial(0, _xR, newLabels);
      modifyInitial(1, _xG, newLabels);
      modifyInitial(2, _xB, newLabels);
    }
    _newLabels = newLabels;
  }

  void GlobalBlender::compute(/*bool reInit, const ushort* newLabels, Areal edgeThres*/) { 
    fprintf(_fp, "Computing blend\n"); fflush(_fp);
    //_edgeThres = edgeThres;
    //updateOffsetImgs(newLabels);
    //computeEdgeMap(newLabels);


    time_t startTime, endTime;
    startTime = time(NULL);
    HB_Precond sweeper(_w, _h);

    ////////////  three at a time
    /*if (reInit) {
      buildInitial(0, _xR, newLabels);
      buildInitial(1, _xG, newLabels);
      buildInitial(2, _xB, newLabels);
    }
    else {
      modifyInitial(0, _xR, newLabels);
      modifyInitial(1, _xG, newLabels);
      modifyInitial(2, _xB, newLabels);
      }*/

    computeRhs(0, _bR, _newLabels);
    computeRhs(1, _bG, _newLabels);
    computeRhs(2, _bB, _newLabels);

   Areal epsilon = .1;  // /.0001
   int myi=0;
   bool _allDone = false;

   //wxMutexLocker *lockX = new wxMutexLocker(_xMutex);
   SteppableLinearSolver precR(numVariables(), this, &sweeper, _xR, _bR, epsilon);
   SteppableLinearSolver precG(numVariables(), this, &sweeper, _xG, _bG, epsilon);
   SteppableLinearSolver precB(numVariables(), this, &sweeper, _xB, _bB, epsilon);
   //delete lockX;

   while (!_allDone && !_myStop) {
     // don't need to check for done since it checks internally
     //lockX = new wxMutexLocker(_xMutex);
     precR.takeOneStep();
     precG.takeOneStep();
     precB.takeOneStep();
     //delete lockX;
     _allDone = precR.done() && precG.done() && precB.done();
     ++myi;
     wxTheApp->Yield();
     printf("took a blending step\n");
   }
   printf("Completed Poisson blend, epsilon %f, steps %d\n",epsilon, myi);

   pasteResultIntoFinal(0, _xR, _newLabels);
   pasteResultIntoFinal(1, _xG, _newLabels);
   pasteResultIntoFinal(2, _xB, _newLabels);

   /*/////////////  one at a time

    for (int c=0; c<3; ++c) { // iterate over color channels
   
      if (reInit)
	buildInitial(c, newLabels);
      else
	modifyInitial(c, newLabels);
        
      // do compute here
      computeRhs(c, newLabels);

      Areal epsilon = .1;  // /.0001
      //Areal epsilon = 0.000001; // DOES MAKE A DIFFERENCE!
      int steps = 10000;
      //Areal error = PrecConjGrad(numVariables(), this, &sweeper, _x,_b,epsilon,&steps, _fp);

      SteppableLinearSolver prec(numVariables(), this, &sweeper, _x,_b,epsilon);
      while (!prec.done())
	prec.takeOneStep();

      fprintf(_fp,"Completed Poisson blend in %d steps with %f error\n",prec.numSteps(), prec.error()); 
      fflush(_fp);

      //buildInitial(c, newLabels);
      //steps = 10000;
      //error = ConjGrad(numVariables(), this,_x,_b,epsilon,&steps, _fp);
      //fprintf(_fp,"Completed Poisson blend in %d steps with %f error\n",steps,error); fflush(_fp);

      //saveX();
      pasteResultIntoFinal(c, newLabels);
    }
   *///////////////  
  
    endTime = time(NULL);
    fprintf(_fp, "Took time %f\n",difftime(endTime, startTime)); fflush(_fp);
    memcpy(_labels, _newLabels, numPixels()*sizeof(short));
    _newLabels = NULL;
  }

  /*void GlobalBlender::saveX() {
    FILE* fp = fopen("X.txt","w");
    fprintf(fp,"%d\n", getImgValue(0, 0, _labels));
    for (int i=0; i<numVariables(); ++i)
      fprintf(fp,"%f\n",_x[i]);
    fclose(fp);
    }*/


  
  //void GlobalBlender::pasteResultIntoFinal(int c, const ushort* labels) {
  //  pasteResultIntoFinal(c,_x,labels);
  // }

  void GlobalBlender::pasteResultIntoFinal(int c, Areal *x, const ushort* labels) {
    //wxMutexLocker lockX(_xMutex);
    //wxMutexLocker lock(_resultMutex);
    int var,i3=c;
    float val;
    for (int i = 0 ; i < numPixels() ; ++i, i3+=3) { // iterate over pixels
      var = pixelToVar(i);
      if (var==-1) { // not a variable, just copy it
	_result[i*3+c] = getImgValue(i, c, labels);
      }
      else {         // take from solution, but clamp it first
	val = x[var];
	if (val < 0)
	  val = 0;
	if (val > 255)
	  val = 255;

	_result[i3] = (unsigned char) val;
      }
      //fprintf(_fp, "i: %d outc: %d, result %d from _x %.5f\n", i, outc,_result[i*3+outc], _x[var]);
    }
    // fflush(_fp);
  }

  void GlobalBlender::updateFinal() { // puts current result in final
    if (!_newLabels) return; // know final value is pasted, plus _newLabels is now null
    pasteResultIntoFinal(0, _xR, _newLabels);
    pasteResultIntoFinal(1, _xG, _newLabels);
    pasteResultIntoFinal(2, _xB, _newLabels);
    
  }

  void GlobalBlender::copyOutFinal(unsigned char *out) { // puts result(final) int out 
    //wxMutexLocker lock(_resultMutex);
    memcpy(out, _result, 3*_w*_h*sizeof(unsigned char));
  }  

  void GlobalBlender::matVecMult(const Areal x[], Areal r[]) const { 

    int l,i;
    int j = nonVarPixel();
    int nv = numVariables();
    memset(r, 0, nv*sizeof(Areal));
    for (i=0; i<nv; ++i) {  // iterate over variables
      
      int pixIndex = varToPixelIndex(i);

      // iterate over four neighbors
      for (l=0; l<4; ++l) {
	int otherPixIndex = linkResolve(pixIndex,l);
	LinkType lt = lookupLink(pixIndex, otherPixIndex, l);
	int otherVar = pixelToVar(otherPixIndex);
	
	if (lt==L_NORMAL || lt == L_BLEND) {
	  if (otherPixIndex == j)  // non-variable pixel, spits to rhs
	    r[i] -= x[i];
	  else                     // normal
	    r[i] += x[otherVar] - x[i];
	} 
	// ignore L_IGNORE
      } // iterate over 4 neighbors
    } // iterate over variables
  }

  //void GlobalBlender::computeRhs(const int c, const ushort* labels) {
  //computeRhs(c, _b, labels);
  //}

  void GlobalBlender::computeRhs(const int c, Areal* b, const ushort* labels) {
    //    0
    // 1  i  2
    //    3

    int j = nonVarPixel();
    memset(b, 0, numVariables()*sizeof(Areal));
    for (int i=0; i<numVariables(); ++i) {  // iterate over variables

      int pixIndex = varToPixelIndex(i);
      Areal myColor = (Areal)getImgValue(pixIndex, c, labels);

      // iterate over four neighbors
      for (int l=0; l<4; ++l) {

	int otherPixIndex = linkResolve(pixIndex,l);
	LinkType lt = lookupLink(pixIndex, otherPixIndex, l);
	
	if (lt==L_NORMAL) {
	  b[i] += Areal(getImgValue(otherPixIndex, c, labels)) - myColor;
	}

	else if (lt==L_BLEND) {
	  int myIm = labelImToOffsetIm(labels[pixIndex]),
	    otherIm = labelImToOffsetIm(labels[otherPixIndex]);
	  Areal a1 = Areal(getImgValue(otherPixIndex,c,myIm)) - myColor;
	  Areal a2 = Areal(getImgValue(otherPixIndex,c,otherIm)) - Areal(getImgValue(pixIndex,c,otherIm));
	  b[i] += (a1+a2) * .5;
	  //b[i] += ( ( Areal(getImgValue(otherPixIndex,c,myIm)) - myColor) +
	  //     ( Areal(getImgValue(otherPixIndex,c,otherIm)) - Areal(getImgValue(pixIndex,c,otherIm)))
	  //     * .5);	  
	}
	// ignore L_IGNORE case
	
	// deal with non-var pixel
	if (otherPixIndex == j)
	  b[i] -= (Areal) getImgValue(j,c,labels);

      } // iterate over neighbors
      
    }// iterate over variables
  }



  
  
} // end namespace
