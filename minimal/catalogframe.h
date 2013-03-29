#pragma once

#include "wxgis\framework\framework.h"
#include "gxapplication.h"

#include "art\mainframe.xpm"

#include "wx\aui\aui.h"
#include "wx\generic\logg.h"
#include "wx\artprov.h"
#include "wx\event.h"
#include "wx\menu.h"


//----------------------
//wxGISCatalogFrame
//-----------------------

class wxGISCatalogApp;

class wxGISCatalogFrame :
	public wxGxApplication
{
public:
	wxGISCatalogFrame(IGISConfig* pConfig, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

	~wxGISCatalogFrame(void);
//IApplication
    virtual void OnAppAbout(void);
private:
	wxXmlNode* m_pConfXmlNode;

	DECLARE_EVENT_TABLE()
};
