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

	wxGxCatalog* pCatalog = new wxGxCatalog();
	pCatalog->Init();
	m_pCatalog = static_cast<IGxCatalog*>(pCatalog);

	wxXmlNode* pViewsNode = m_pConfig->GetConfigNode(enumGISHKLM, wxString(wxT("frame/views")));

	if(!pViewsNode)
	{
		wxLogError(_("wxGxApplication: Error find <views> XML Node"));
		return;
	}

	m_pTreeView = new wxGxTreeView(this, TREECTRLID);



};