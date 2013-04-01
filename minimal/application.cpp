#include "application.h"
#include "toolbarmenu.h"
#include <wx\tokenzr.h>
#include "art\default_16.xpm"

BEGIN_EVENT_TABLE(wxGISApplication, wxFrame)
	EVT_ERASE_BACKGROUND(wxGISApplication::OnEraseBackground)
	EVT_SIZE(wxGISApplication::OnSize)
	EVT_RIGHT_DOWN(wxGISApplication::OnRightDown)
	EVT_MENU_RANGE(wxGISApplication::ID_PLUGINCMD, ID_PLUGINCMD + 512, wxGISApplication::OnCommand)
	EVT_UPDATE_UI_RANGE(wxGISApplication::ID_PLUGINCMD, ID_PLUGINCMD + 512, wxGISApplication::OnCommandUI)
END_EVENT_TABLE()

wxGISApplication::wxGISApplication(IGISConfig* pConfig, wxWindow* pParent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER )
{
	m_pConfig = pConfig;

	CreateStatusBar();
	wxFrame::GetStatusBar()->SetStatusText(_("Ready"));

	//load commands
	wxXmlNode* pCommandsNode = m_pConfig->GetConfigNode(enumGISHKLM, wxString(wxT("commands")));
	if(pCommandsNode)
		LoadCommands(pCommandsNode);
	//load commandbars
	SerializeCommandBars();
	//load accelerators
	m_pGISAcceleratorTable = new wxGISAcceleratorTable(this, pConfig);

	//Create MenuBar
	wxXmlNode* pMenuBarNode = m_pConfig->GetConfigNode(enumGISHKCU, wxString(wxT("frame/menubar")));
	if(!pMenuBarNode)
	{
		wxXmlNode* pMenuBarNodeLM =m_pConfig->GetConfigNode(enumGISHKLM, wxString(wxT("frame/menubar")));
		pMenuBarNode = m_pConfig->CreateConfigNode(enumGISHKCU, wxString(wxT("frame/menubar")), true);
		pMenuBarNode->operator = (*pMenuBarNodeLM);
	}
	m_pMenuBar = new wxGISMenuBar(0, static_cast<IApplication*>(this), pMenuBarNode);
	SetMenuBar(static_cast<wxMenuBar*>(m_pMenuBar));

    //mark menus from menu bar as enumGISTAMMenubar
	for(size_t i = 0; i< m_CommandBarArray.size(); i++)
		if(m_pMenuBar->IsMenuBarMenu(m_CommandBarArray[i]->GetName()))

}