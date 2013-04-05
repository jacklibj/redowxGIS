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
	if(m_pTreeView->Activate(this, m_pCatalog, m_pConfig->GetConfigNode(enumGISHKLM, wxString(wxT("frame/views/treeview")))))
	{
		m_mgr.AddPane(m_pTreeView, wxAuiPaneInfo().Name(wxT("tree_window")).Caption(_("Tree Pane")).BestSize(wxSize(280,128)).MinSize(wxSize(200,64)).Left().Layer(1).Position(1).CloseButton(true));
		RegisterChildWindow(m_pTreeView);
	}
	else
		wxDELETE(m_pTreeView);

	m_pTabView = new wxGxTabView(this);
	if(m_pTabView->Activate(this, m_pCatalog, m_pConfig->GetConfigNode(enumGISHKLM, wxString(wxT("frame/views/tabview")))))
	{

	}



};