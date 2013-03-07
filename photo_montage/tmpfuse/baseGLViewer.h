#ifndef BASEGLVIEWER_H
#define BASEGLVIEWER_H
//#undef _DEBUG
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "myGLCanvas.h"
#include "ImageAbs.h"

class BaseGLViewer: public wxFrame
{
public:
    BaseGLViewer(wxFrame *frame, const wxString& title, const wxPoint& pos,
            const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);

    void SetViewImage(const ImageAbs* ia) { m_canvas->SetViewImage(ia); }
    
    void OnExit(wxCommandEvent& event);

    
    
public:
    MyGLCanvas*    m_canvas;

DECLARE_EVENT_TABLE()
};



#endif
