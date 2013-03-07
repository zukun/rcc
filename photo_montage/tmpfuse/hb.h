#ifndef HB_H
#define HB_H

#define Areal float

class HB_Precond {


 public:

  HB_Precond(const int w, const int h);


  void precondVec(Areal* r) const {
    sweepUp(r); sweepDown(r);
  }


 private:

  void sweepUp(Areal* r) const;
  void sweepDown(Areal* r) const;
  Areal* extractMatrix(int which) const;
  void printMatrix(Areal* m, FILE* fp) const;

  int _w, _h, _xlevels, _ylevels, _maxLevels;
  
};


#endif
