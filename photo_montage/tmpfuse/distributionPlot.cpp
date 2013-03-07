#include "distributionPlot.h"
//#include "graphcuts/holeFillCut.h"
#include "compViewer.h"

BEGIN_EVENT_TABLE(DistributionPlot,wxFrame)
//  EVT_PLOT_CLICKED         ( -1,  DistributionPlot::OnPlotClick)
 // EVT_PLOT_DOUBLECLICKED   ( -1,  DistributionPlot::OnPlotDClick)
//  EVT_CUSTOM(wxEVT_MYRMOUSE_DOWN, -1, DistributionPlot::handleRMouseDown)
END_EVENT_TABLE()

FILE* fp;

DistributionPlot::DistributionPlot(CompViewer* comp, StackViewer* stack)
       : wxFrame( comp, -1, "Pixel Color Distribution",
                  wxPoint(20,20), wxSize(470,500) )
{
	_compWidget = comp;
	_stackWidget = stack;
	stack->_distPlot = this;

    wxMenu *file_menu = new wxMenu();

    wxMenuBar *menu_bar = new wxMenuBar();
    menu_bar->Append(file_menu, "&File");

    SetMenuBar( menu_bar );

    CreateStatusBar(2);
    int widths[] = { -1, 100 };
    SetStatusWidths( 2, widths );

	_plotR = new wxPlotWindow( this, -1, wxPoint(0,0), wxSize(20,20), wxSUNKEN_BORDER | wxPLOT_BUTTON_ENLARGE);
//    _plotR->SetUnitsPerValue( 0.01 );
    _plotG = new wxPlotWindow( this, -1, wxPoint(0,0), wxSize(20,20), wxSUNKEN_BORDER | wxPLOT_BUTTON_ENLARGE);
 //   _plotG->SetUnitsPerValue( 0.01 );
    _plotB = new wxPlotWindow( this, -1, wxPoint(0,0), wxSize(20,20), wxSUNKEN_BORDER | wxPLOT_BUTTON_ENLARGE);
  //  _plotB->SetUnitsPerValue( 0.01 );

//    m_plot->SetScrollOnThumbRelease( TRUE );

    
    wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );
    
    topsizer->Add( _plotR, 1, wxEXPAND );
    topsizer->Add( _plotG, 1, wxEXPAND );
    topsizer->Add( _plotB, 1, wxEXPAND );

    SetAutoLayout( TRUE );
    SetSizer( topsizer );
	fp = fopen ("output.txt", "w");
}


DistributionPlot::~DistributionPlot () {
	fclose (fp);
}

void DistributionPlot::handleRMouseDown(wxCommandEvent& event)
{

    fprintf(fp,  "in handleRMouseDown\n" );
	int x = _stackWidget->m_canvas->mx ();
	int y = _stackWidget->m_canvas->my ();
	
	PixelHisto* ph = _compWidget->GetHistograms()->GetDataForPixel(x, y);
    fprintf(fp, "got histogams\n" );
	if (_plotR->GetCount () > 0) {
		ChannelHistoCurve* c = (ChannelHistoCurve*)(_plotR->GetAt(0));
		_plotR->Delete (c);
		delete c;
	}
	if (_plotG->GetCount () > 0) {
		ChannelHistoCurve* c = (ChannelHistoCurve*)(_plotG->GetAt(0));
		_plotG->Delete (c);
		delete c;
	}
	if (_plotB->GetCount () > 0) {
		ChannelHistoCurve* c = (ChannelHistoCurve*)(_plotB->GetAt(0));
		_plotB->Delete (c);
		delete c;
	}
	
    fprintf(fp, "removed all curves\n" );

	wxPen pen;
	pen.SetWidth (10);
	pen.SetColour (255, 0, 0);
	pen.SetStyle (wxSOLID);
 	ChannelHistoCurve* chc = new ChannelHistoCurve (&ph->_rc);
	chc->SetPenNormal (pen);
	_plotR->Add (chc);
	chc = new ChannelHistoCurve (&ph->_gc);
	pen.SetColour (0, 255, 0);
	chc->SetPenNormal (pen);
	_plotG->Add (chc);
	chc = new ChannelHistoCurve (&ph->_bc);
	pen.SetColour (0, 0, 255);
	chc->SetPenNormal (pen);
	_plotB->Add (chc);
	
	fprintf(fp, "added all curves\n" );

//	_plotR->Refresh ();
}
