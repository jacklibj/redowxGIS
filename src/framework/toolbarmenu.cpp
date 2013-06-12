#include "wxgis/framework/toolbarmenu.h"
#include "wxgis/framework/commandbar.h"

BEGIN_EVENT_TABLE(wxGISToolBarMenu, wxMenu)
	EVT_MENU_RANGE(wxGISApplication::ID_PLUGINCMD + 520, wxGISApplication::ID_PLUGINCMD + 1032, wxGISToolBarMenu::OnCommand)
END_EVENT_TABLE()

wxGISToolBarMenu::wxGISToolBarMenu(const wxString& sName, const wxString& sCaption, wxGISEnumCommandBars type, const wxString& title, long style): wxGISMenu(sName, sCaption, type, title, style)
{
}

wxGISToolBarMenu::~wxGISToolBarMenu(void)
{
}

void wxGISToolBarMenu::Update(void)
{
	//clear
	for(size_t i = 0; i< m_delitems.size(); i++)
		Destroy(m_delitems[i]);
	m_delitems.clear();

	COMMANDBARARRAY* pCommandBars = m_pApp->GetCommandBars();

	for (size_t i = 0; i < pCommandBars->size(); i++)
	{
		IGISCommandBar* pCmdBar = pCommandBars->at(i);
		if(pCmdBar->GetType() == enumGISCBToolbar)
		{
			wxWindow* pWnd = dynamic_cast<wxWindow*>(pCmdBar);
			bool bIsShown = pWnd->IsShown();
			//sort
			wxMenuItem* pItem = Prepend(wxGISApplication::ID_PLUGINCMD + i + 520, pCmdBar->GetCaption(), wxT(""), wxITEM_CHECK);
			pItem->Check(bIsShown);
			m_delitems.push_back(pItem);
		}
	}
	m_delitems.push_back(AppendSeparator());
	ICommand* pCmd = m_pApp->GetCommand(wxT("wxGISCommand"), 2);
	if(pCmd)
		m_delitems.push_back(Append(pCmd->GetID(), pCmd->GetCaption(), pCmd->GetTooltip(), (wxItemKind)pCmd->GetKind()));
}

void wxGISToolBarMenu::OnCommand(wxCommandEvent& event)
{
	int pos = event.GetId() - (wxGISApplication::ID_PLUGINCMD + 520);
	COMMANDBARARRAY* pCommandBars = m_pApp->GetCommandBars();
	wxWindow* pWnd = dynamic_cast<wxWindow*>(pCommandBars->at(pos));
	m_pApp->ShowApplicationWindow(pWnd, !pWnd->IsShown());
}

wxIcon wxGISToolBarMenu::GetBitmap(void)
{
	return wxIcon();
}

wxString wxGISToolBarMenu::GetCaption(void)
{
	return wxString(_("Toolbars"));
}

wxString wxGISToolBarMenu::GetCategory(void)
{
	return wxString(_("[Menues]"));
}


bool wxGISToolBarMenu::GetChecked(void)
{
	return false;
}

bool wxGISToolBarMenu::GetEnabled(void)
{
	return true;
}

wxString wxGISToolBarMenu::GetMessage(void)
{
	return wxString(_("Toolbars"));
}

wxGISEnumCommandKind wxGISToolBarMenu::GetKind(void)
{
	return enumGISCommandMenu;
}

void wxGISToolBarMenu::OnClick(void)
{

}

bool wxGISToolBarMenu::OnCreate(IApplication* pApp)
{
	m_pApp = dynamic_cast<wxGISApplication*>(pApp);
	return true;
}

wxString wxGISToolBarMenu::GetTooltip(void)
{
	return wxString(_("Toolbars"));
}

unsigned char wxGISToolBarMenu::GetCount(void)
{
	return 1;
}