#include "wxgis/catalogui/viewfactory.h"
#include "wxgis/catalogui/gxnoview.h"
#include "wxgis/catalogui/gxcontentview.h"
#include "wxgis/catalogui/gxtableview.h"
#include "wxgis/catalogui/gxmapview.h"

IMPLEMENT_DYNAMIC_CLASS(wxGISViewsFactory, wxObject)

wxGISViewsFactory::wxGISViewsFactory(void)
{

}

wxGISViewsFactory::~wxGISViewsFactory(void)
{

}

wxWindow* wxGISViewsFactory::wxCreateObject(wxString sName, wxWindow* parent)
{
	if (sName == wxT("NoView"))
	{
		wxGxNoView* pwxGxNoView = new wxGxNoView(parent, wxID_ANY);
		return static_cast<wxWindow*>(pwxGxNoView);//
	}
	if (sName == wxT("ContentView"))
	{
		wxGxContentView* pwxGxContentView = new wxGxContentView(parent);
		return static_cast<wxWindow*>(pwxGxContentView);//
	}
	if (sName == wxT("GeoGraphicView"))
	{
		wxGxMapView* pwxGxMapView = new wxGxMapView(parent);
		return static_cast<wxWindow*>(pwxGxMapView);
	}
	if (sName == wxT("TableView"))
	{
		wxGxTableView* pwxGxTableView = new wxGxTableView(parent);
		return static_cast<pwxGxTableView>;
	}
	return NULL;
}