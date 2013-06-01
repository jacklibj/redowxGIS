#pragma once

#include "wxgis/catalogui/gxview.h"
#include "wxgis/carto/tableview.h"

class WXDLLIMPEXP_GIS_CLU wxGxTableView :
	public wxGISTableView,
	public wxGxView,
	public IGxSelectionEvents
{
public:
	wxGxTableView(wxWindow* parent, wxWindowID id = TABLECTRLID, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	virtual ~wxGxTableView(void);
//IGxView
	virtual bool Activate(wxGxApplication* application, IGxCatalog* catalog, wxXmlNode* pConf);
	virtual void Deactivate(void);
	virtual bool Applies(IGxSelection* Selection, long nInitiator);
	//
	virtual void OnSelectionChanged(IGxSelection* Selection, long nInitiator);
private:
	IGxSelection* m_pSelection;
	IGxObject* m_pParentGxObject;
};

