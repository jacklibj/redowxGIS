#pragma once
#include "wxgis/catalogui/gxview.h"
#include "wxgis/carto/mapview.h"

class WXDLLIMPEXP_GIS_CLU wxGxMapView :
	public wxGISMapView,
	public wxGxView,
	public IGxSelectionEvents
{
public:
// wxGxMapView(void){};
	wxGxMapView(wxWindow* parent, wxWindowID id = MAPCTRLID, const wxPoint& pos = wxDefaultPosition,
		                  const wxSize& size = wxDefaultSize);

	virtual ~wxGxMapView(void);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseDown(wxMouseEvent& event);
	void OnMouseUp(wxMouseEvent& event);
	void OnMouseDoubleClick(wxMouseEvent& event);
//IGxView
	virtual bool Activate(wxGxApplication* application, IGxCatalog* Catalog, wxXmlNode* pConf);
	virtual void Deactivate(void);
	virtual bool Applies(IGxSelection* Selection);
//iGxSelectionEvents
	virtual void OnSelectionChanged(IGxSelection* Selection, long nInitiator);
private:
	IGxSelection* m_pSelection;
	IGxObject* m_pParentGxObject;
	IStatusBar* m_pStatusBar;

	DECLARE_EVENT_TABLE()
};