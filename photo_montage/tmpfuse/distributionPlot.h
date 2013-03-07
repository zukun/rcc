#ifndef DISTRIBUTIONPLOT_H
#define DISTRIBUTIONPLOT_H
//#undef _DEBUG
#include <wx/wx.h>
#include <wx/plot/plot.h>

class CompViewer;
class StackViewer;
class DistributionPlot: public wxFrame
{
public:
	DistributionPlot(CompViewer* comp, StackViewer* stack);
    ~DistributionPlot ();

    void OnPlotClick( wxPlotEvent &event );
    void OnPlotDClick( wxPlotEvent &event );
    void handleRMouseDown(wxCommandEvent& event);

    wxPlotWindow   *_plotR;
    wxPlotWindow   *_plotG;
    wxPlotWindow   *_plotB;
    wxTextCtrl     *_log;

private:
	CompViewer*		_compWidget;
	StackViewer*	_stackWidget;
    DECLARE_EVENT_TABLE()
};


#endif
