#include "baseGLViewer.h"


BEGIN_EVENT_TABLE(BaseGLViewer, wxFrame)
  EVT_MENU(wxID_EXIT, BaseGLViewer::OnExit)
END_EVENT_TABLE()

// My frame constructor
BaseGLViewer::BaseGLViewer(wxFrame *frame, const wxString& title, const wxPoint& pos,
                 const wxSize& size, long style)
         : wxFrame(frame, -1, title, pos, size, style)
{
  m_canvas = NULL;
}



// Intercept menu commands
void BaseGLViewer::OnExit(wxCommandEvent& event)
{
    Destroy();
}
 
