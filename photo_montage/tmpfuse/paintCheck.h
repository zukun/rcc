#ifndef PAINTCHECK_H
#define PAINTCHECK_H

//#include "ImageAbs.h"


class PaintCheck {

 public:
  PaintCheck(int w, int h) : _w(w), _h(h) {
    _labels = new ushort[_w*_h];
    _allStrokes = new StrokeData[_w*_h];
    _overlay = new unsigned char[4*_w*_h];
    _undoable = false;
  }

  void save(ushort* labels, const unsigned char* ov, const ImageAbs& blend, StrokeData* allStrokes) {
    memcpy(_labels, labels, _w*_h*sizeof(ushort));
    memcpy(	_allStrokes, allStrokes, _w*_h*sizeof(StrokeData));
    memcpy(_overlay, ov, 4*_w*_h*sizeof(unsigned char));
    _blend = blend;
    _undoable = true;
  }

 bool undoable() { return _undoable; }

 ~PaintCheck() {
   delete[] _labels;
   delete[] _allStrokes;
   delete[] _overlay;
 }


  int _w, _h;
  ImageAbs _blend;
  ushort* _labels;
  StrokeData* _allStrokes;
  unsigned char* _overlay;
  bool _undoable;

};






#endif
