//#undef _DEBUG
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/glcanvas.h>
#include "fuse.h"
#include "compViewer.h"
#include "stackViewer.h"

enum InputDataType {FAMILY, BRONZE, FRANKENFACE, NA};
InputDataType DATA_SET = NA;
IMPLEMENT_APP(MyApp)

bool MyApp::OnInit(void)
{
  wxLog::SetTraceMask(wxTraceMessages);
  ::wxInitAllImageHandlers();

  // Create the main frame window
  CompViewer *compframe = new CompViewer("Composite", wxPoint(50, 50),
                               wxSize(400, 300));
  SetTopWindow(compframe);

  //StackViewer *stackframe = new StackViewer(NULL, "Stack", wxPoint(460, 50),
  //			 wxSize(400, 300));
  

  int attribList[4];
  attribList[0] = WX_GL_RGBA;
  attribList[1] = WX_GL_STENCIL_SIZE;
  attribList[2] = 8;
  attribList[3] = WX_GL_DOUBLEBUFFER;
  compframe->m_canvas = new MyGLCanvas(compframe,-1, wxDefaultPosition, wxDefaultSize, 0, "shit",  attribList);
  compframe->finishInit();
  //compframe->m_canvas->Show();
  //stackframe->m_canvas = new MyGLCanvas(stackframe);
  
  wxCommandEvent tempEvent;
  switch (DATA_SET) {
	  case FAMILY:
		compframe->_preLoadedComp = "small0168.JPG";
		compframe->_preLoadedStack.Add ("small0170.JPG", 1);
		compframe->_preLoadedStack.Add ("small0173.JPG", 1);
		compframe->_preLoadedStack.Add ("small0174.JPG", 1);
		compframe->_preLoadedStack.Add ("small0176.JPG", 1);

		compframe->NewComposite (tempEvent);
		compframe->LoadStack (tempEvent);
		compframe->SetInertiaConstant (0.);
		compframe->SetCutType (C_GRAD);
		break;
	  case BRONZE:
		compframe->_preLoadedComp = "IMG_3083.JPG";
		compframe->_preLoadedStack.Add ("IMG_3084.JPG", 1);
		compframe->_preLoadedStack.Add ("IMG_3085.JPG", 1);
		compframe->_preLoadedStack.Add ("IMG_3086.JPG", 1);
		compframe->_preLoadedStack.Add ("IMG_3087.JPG", 1);
		compframe->_preLoadedStack.Add ("IMG_3088.JPG", 1);
		compframe->_preLoadedStack.Add ("IMG_3089.JPG", 1);
		compframe->_preLoadedStack.Add ("IMG_3090.JPG", 1);
		compframe->_preLoadedStack.Add ("IMG_3091.JPG", 1);
		compframe->_preLoadedStack.Add ("IMG_3092.JPG", 1);
		compframe->_preLoadedStack.Add ("IMG_3093.JPG", 1);
		compframe->_preLoadedStack.Add ("IMG_3094.JPG", 1);
		compframe->_preLoadedStack.Add ("IMG_3095.JPG", 1);
		compframe->_preLoadedStack.Add ("IMG_3096.JPG", 1);
		compframe->_preLoadedStack.Add ("IMG_3097.JPG", 1);
		compframe->_preLoadedStack.Add ("IMG_3098.JPG", 1);

		compframe->NewComposite (tempEvent);
		compframe->LoadStack (tempEvent);
		compframe->SetInertiaConstant (0.01f);
		compframe->SetCutType (C_GRAD);
		break;
	  case FRANKENFACE:
		compframe->_preLoadedComp = "IMG_2739.png";
		compframe->_preLoadedStack.Add ("IMG_2661.png", 1);
		compframe->_preLoadedStack.Add ("IMG_2739.png", 1);

		compframe->NewComposite (tempEvent);
		compframe->LoadStack (tempEvent);
		compframe->SetInertiaConstant (0.);
		break;
	  default:
		compframe->SetInertiaConstant (0.);
		  break;
  }
 // Show the frame
  compframe->Show(TRUE);
  //stackframe->Show(TRUE);


  return TRUE;
}
