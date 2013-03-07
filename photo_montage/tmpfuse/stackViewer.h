#ifndef STACKVIEWER_H
#define STACKVIEWER_H

//#undef _DEBUG
#include <vector>
#include <map>
#include <utility>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "baseGLViewer.h"
#include "globFunc.h"
//#include "distributionPlot.h"
#include "bestFitAffine.h"


#define ID_ZCHANGE 20000

class StackViewer: public BaseGLViewer
{
public:

    StackViewer(int w, int h, const wxString& baseName, wxFrame *frame, const wxString& title, 
		const wxPoint& pos, const wxSize& size, const wxArrayString& names);

    /*StackViewer(int w, int h, char* stackDescrip, wxFrame *frame, const wxString& title, 
      const wxPoint& pos, const wxSize& size, const wxArrayString& names);*/
    
    void finishInit();

    ~StackViewer();

    // events
    void zChange(wxSpinEvent& evt);
    void handleMouseMove(wxCommandEvent& evt);
    //    void handleRMouseDown(wxCommandEvent& event);
    void handleMouseDown(wxCommandEvent& event);

    void externalSetImage(const int i);
    const ImageAbs* getCurrImage() const;
    int getCurrNum() const { return _currImage; }
    void resize();

    // relative
    void translateCurrImage(const int x, const int y);
    void scaleCurrImage(const float s);
    void rotateCurrImage(const float a);

    //void save(char* name);

    void addImageToStack(ImageAbs* im);
    void removeLastImage ();
    int width() const { return _w;}
    int height() const { return _h; }
    int count() const { return _count; }
    const std::vector<ImageAbs*>& images() const { return _images;}

    std::vector<ImageAbs*>& getImageVector() { return _images; }
    const std::vector<ImageAbs*>& getImageVector() const { return _images; }

    const CorrVec& getCurrCorrVec() const;
    CorrVec& getCurrCorrVec();

    void enableCreatePoint();
    void disableCreatePoint();

    //    DistributionPlot* _distPlot;

protected:
    
    void setCurrImage(const int i);
    void handlePointTransform();

    wxSpinCtrl* _spin;
    wxArrayString _filenames;
    std::vector<ImageAbs*> _images;
    std::vector <CorrVec> _corrs;
    int _count;
    int _currImage;
    int _w, _h; // composite space
    bool _createPoint;
    DECLARE_EVENT_TABLE()
};


#endif
