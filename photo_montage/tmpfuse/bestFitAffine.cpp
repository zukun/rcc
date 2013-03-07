#include "bestFitAffine.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

BestFitAffine::BestFitAffine(const CorrVec& cv, float offx, float offy) {
  assert(cv.size() >= 3);
  
  int f=-1;
  int n = cv.size();
  double* a = new double[6*2*n];
  double* b = new double[2*n];
  
  memset(a,0,12*n*sizeof(double));
  double* rowptr = a;
  double* bptr = b;
  for (int i=0; i<n; ++i) {
    rowptr[0] = cv[i].second.x-offx;   rowptr[1] = cv[i].second.y-offy;
    rowptr[4] = 1.;
    (*bptr) = cv[i].first.x-offx;  
    ++bptr; rowptr += 6;

    rowptr[2] = cv[i].second.x-offx;   rowptr[3] = cv[i].second.y-offy;
    rowptr[5] = 1.;
    (*bptr) = cv[i].first.y-offy;  
    ++bptr; rowptr += 6;
  }
  
  double ssq = qrlsq(a,b,2*n, 6, &f);
  delete[] a;
  _success = (f==0);
  printf("residual %f, success %d\n",ssq,_success);

  _M = new float[6];
  if (!_success)
    memset(_M,0,6*sizeof(float));
  else {
    _M[0] = b[0];
    _M[1] = b[1];
    _M[3] = b[2];
    _M[4] = b[3];
    _M[2] = b[4];
    _M[5] = b[5];
  }
  delete[] b;
}

BestFitAffine::~BestFitAffine() {
  delete[] _M;
}

const float* BestFitAffine::getTransform() const {
  return _M;
}



double BestFitAffine::qrlsq(double *a,double *b,int m,int n,int *f) { 
  double *p,*q,*w,*v;
  double s,h,r;
  int i,j,k,mm,ms;
  if(m<n) return -1;
  w=(double *)calloc(m,sizeof(double));
  for(i=0,mm=m,p=a; i<n ;++i,--mm,p+=n+1){
    if(mm>1){
      for(j=0,q=p,s=0.; j<mm ;++j,q+=n){
	w[j]= *q; s+= *q* *q;
       }
      if(s>0.){
	h=sqrt(s); if(*p<0.) h= -h;
	s+= *p*h; s=1./s; w[0]+=h;
	for(k=1,ms=n-i; k<ms ;++k){
	  for(j=0,q=p+k,r=0.; j<mm ;q+=n) r+=w[j++]* *q;
	  r=r*s;
	  for(j=0,q=p+k; j<mm ;q+=n) *q-=r*w[j++];
	 }
        *p= -h;
        for(j=0,q=b+i,r=0.; j<mm ;) r+=w[j++]* *q++;
        for(j=0,q=b+i,r*=s; j<mm ;) *q++ -=r*w[j++];
       }
     }
   }
  *f=solvru(a,b,n);
  for(j=n,q=b+j,s=0.; j<m ;++j,++q) s+= *q* *q;
  free(w);
  return s;
} 

int BestFitAffine::solvru(double *a,double *b,int n) { 
  int j,k; 
  double s,t,*p,*q;
  for(j=0,s=0.,p=a; j<n ;++j,p+=n+1) if((t=fabs(*p))>s) s=t;
  s*=1.e-16;
  for(j=n-1,p=a+n*n-1; j>=0 ;--j,p-=n+1){
    for(k=j+1,q=p+1; k<n ;) b[j]-=b[k++]* *q++;
    if(fabs(*p)<s) return -1;
    b[j]/= *p;
  }
  return 0;
}




