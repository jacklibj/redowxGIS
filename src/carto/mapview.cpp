#include "wxgis/carto/mapview.h"
//
#include "wxgis/display/screendisplayplus.h"

#define WAITTIME 650

//------
// wxDrawingThread
//--------

wxDrawingThread::wxDrawingThread(wxGISMapView* pView, std::vector<wxGISLayer*>& Layers) : wxThread(), m_pView(pView), m_Layers(Layers)
{

}