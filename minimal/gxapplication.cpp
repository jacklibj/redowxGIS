#include "gxapplication.h"
#include "customizedlg.h"
#include "gxcatalog.h"
#include "toolbarmenu.h"

//---------------------
// wxGxApplication
//----------------------

wxGxApplication::wxGxApplication(IGISConfig* pConfig, wxWindow* parent, wxWindowID id,
	const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxGISApplication(pConfig, parent, id, title, pos, size, style),
	m_pCatalog(NULL), m_pTreeView(NULL), m_pTabView(NULL)
{
	wxLogMessage(_("wxGxApplication: Creating main application frame..."));

	m_mgr.SetManagedWindow(this);

	wxGxCatalog* pcatalog = new wxGxCatalog();

};