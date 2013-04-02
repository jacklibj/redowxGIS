#include "commandbar.h"
#include "art\default_16.xpm"

//---------
// wxGISCommandBar
//------------------------

wxGISCommandBar::wxGISCommandBar(const wxString& sName = NONAME, 
	const wxString& sCaption = _("No Caption"), wxGISEnumCommandBars type = enumGISCBNone)
{
	m_sName = sName;
	m_sCaption = sCaption;
	m_type = type;
}

wxGISCommandBar::~wxGISCommandBar(void)
{
}

void wxGISCommandBar::SetName(const wxString& sName)
{
	m_sName = sName;
}

wxString wxGISCommandBar::GetName(void)
{
	return m_sName;
}

void wxGISCommandBar::SetCaption(const wxString& sCaption)
{
	m_sCaption = sCaption;
}

void wxGISCommandBar::SetType(wxGISEnumCommandBars type)
{
	m_type = type;
}

wxGISEnumCommandBars wxGISCommandBar::GetType(void)
{
	return m_type;
}

void wxGISCommandBar::AddCommand(ICommand* pCmd)
{
	m_CommandArray.push_back(pCmd);
}

void wxGISCommandBar::RemoveCommand(size_t nIndex)
{
	wxASSERT(nIndex >= 0 && nIndex < m_CommandArray.size());
	m_CommandArray.erase(m_CommandArray.begin() + nIndex);
}

void wxGISCommandBar::MoveCommandLeft(size_t nIndex)
{
	wxASSERT(nIndex >= 0 && nIndex < m_CommandArray.size());
	ICommand* pCmd = m_CommandArray[nIndex];
	m_CommandArray[nIndex] = m_CommandArray[nIndex-1];
	 m_CommandArray[nIndex-1] = pCmd;
}

void wxGISCommandBar::MoveCommandRight(size_t nIndex)
{
	wxASSERT(nIndex >= 0 && nIndex < m_CommandArray.size());
	ICommand* pCmd = m_CommandArray[nIndex];
	m_CommandArray[nIndex] = m_CommandArray[nIndex+1];
	m_CommandArray[nIndex+1] = pCmd;
}

size_t wxGISCommandBar::GetCommandCount(void)
{
	return m_CommandArray.size();
}

ICommand* wxGISCommandBar::GetCommand(size_t nIndex)
{
	wxASSERT(nIndex >= 0 && nIndex > m_CommandArray.size());
	return m_CommandArray[nIndex];
}

void wxGISCommandBar::Serialize(IApplication* pApp, wxXmlNode* pNode, bool bStore)
{
	if(bStore)
	{
		pNode->AddProperty(wxT("name"), m_sName);
		pNode->AddProperty(wxT("caption"), m_sCaption);
		for(size_t i = m_CommandArray.size(); i > 0; i--)
		{
			ICommand* pCmd = m_CommandArray[i-1];
			if(pCmd)
			{
				wxXmlNode* pNewNode = new wxXmlNode(pNode, wxXML_ELEMENT_NODE, wxString(wxT("Item")));
				wxGISEnumCommandKind Kind = pCmd->GetKind();
				switch(Kind)
				{
				case enumGISCommandSeperator:
					pNewNode->AddProperty(wxT("type"), wxT("sep"));
					break;
				case enumGISCommandCheck:
				case enumGISCommandRadio:
				case enumGISCommandNormal:
				case enumGISCommandControl:
					{
						pNewNode->AddProperty(wxT("type"), wxT("cmd"));
						wxObject* pObj = dynamic_cast<wxObject*>(pCmd);
						if(pObj)
						{
							wxClassInfo* pInfo = pObj->GetClassInfo();
							wxString sClassName = pInfo->GetClassName();
							pNewNode->AddProperty(wxT("cmd_name"), sClassName);
							pNewNode->AddProperty(wxT("subtype"), wxString::Fromat(wxT("%u"), pCmd->GetSubType()));
							pNewNode->AddProperty(wxT("name"), pCmd->getCaption());
						}
						break;
					}
				case enumGISCommandMenu:
					{
						pNewNode->AddProperty(wxT("type"), wxT("menu"));
						IGISCommandBar* pCB = dynamic_cast<IGISCommandBar*>(pCmd);
						if(pCB)
						{
							pNewNode->AddProperty(wxT("cmd_name"), pCB->GetName());
							pNewNode->AddProperty(wxT("name"), pCmd->getCaption());
						}
						break;
					}
				default:
					break;
				}
			}
		}
	}
	else
	{
		wxXmlNode *subchild = pNode->GetChildren();
		while(subchild)
		{
			wxString sType = subchild->GetPropVal(wxT("type"), wxT("sep"));
			if(sType == wxT("cmd"))
			{
				wxString sCmdName = subchild->GetPropVal(wxT("cmd_name"), wxT("None"));
				unsigned char nSubtype = wxAtoi(subchild->GetPropVal(wxT("subtype"), wxT("0")));
				//
				ICommand* pSubCmd = pApp->GetCommand(sCmdName, nSubtype);
				if(pSubCmd)
					AddCommand(pSubCmd);
			}
			else if(sType == wxT("menu"))
			{
				wxString sCmdName = subchild->GetPropVal(wxT("cmd_name"), ERR);
				IGISCommandBar* pGISCommandBar = pApp->GetCommandBar(sCmdName);
				if(pGISCommandBar)
				{
					ICommand* pSubCmd = dynamic_cast<ICommand*>(pGISCommandBar);
					if(pSubCmd)
						AddCommand(pSubCmd);
					else
						AddMenu(dynamic_cast<wxMenu*>(pGISCommandBar), pGISCommandBar->GetCaption());
				}
			}
			else
			{
				ICommand* pSubCmd = pApp->GetCommand(wxT("wxGISCommandCmd"), 3);
				if(pSubCmd)
					AddCommand(pSubCmd);
			}
			subchild = subchild->GetNext();
		}
	}
}

//-----------
// wxGISMenu
//------------
wxGISMenu::wxGISMenu(const wxString& sName, const wxString& sCaption, wxGISEnumCommandBars type, const wxString& title, long style) : 
wxMenu(title, style), wxGISCommandBar(sName, sCaption, type)
{
}

wxGISMenu::~wxGISMenu(void)
{
	for(size_t i = 0; i< m_SubmenuArray.size(); i++)
	{
		Delete(m_SubmenuArray[i].pItem);
		wsDELETE(m_SubmenuArray[i].pBar);
	}
}

void wxGISMenu::AddCommand(ICommand* pCmd)
{
	switch(pCmd->GetKind())
	{
	case enumGISCommandSeparator:
		AppendSeparator();
		break;
	case enumGISCommandMenu:
		{
			IGISCommandBar* pGISCommandBar = dynamic_cast<IGISCommandBar*>(pCmd);
			if(pGISCommandBar)
			{
				pGISCommandBar->Reference();
				SUBMENUDATA data = {AppendSubMenu(dynamic_cast<wxMenu*>(pCmd), pCmd->getCaption(), pCmd->GetMessage()), pGISCommandBar};
				m_SubmenuArray.push_back(data);
			}
		}
		break;
	case enumGISCommandCheck:
	case enumGISCommandRadio:
	case enumGISCommandNormal:
		{
			wxMenuItem *item = new wxMenuItem(this, pCmd->GetID(), pCmd->GetMessage(), (wxItemKind)pCmd->GetKind());
			wxBitmap Bmp = pCmd->GetBitmap();
			if(Bmp.IsOk())
				item->SetBitmaps(Bmp);
			Append(item);

		}
		break;
	case enumGISCommandControl:
		return;
	}
	wxGISCommandBar::AddCommand(pCmd);
}

void wxGISMenu::RemoveCommand(size_t nIndex)
{
	if(m_CommandArray[nIndex]->GetKind() == enumGISCommandSeparator)
		Delete(FindItemByPosition(nIndex));
	else
		Destroy(m_CommandArray[nIndex]->GetID());
	wxGISCommandBar::RemoveCommand(nIndex);
}

void wxGISMenu::MoveCommandLeft(size_t nIndex)
{
	wxMenuItem *pMenuItem =Remove(FindItemByPosition(nIndex));
    Insert(nIndex - 1, pMenuItem);
	wxGISCommandBar::MoveCommandLeft(nIndex);
}

void wxGISMenu::MoveCommandRight(size_t nIndex)
{
	wxMenuItem *pMenuItem =Remove(FindItemByPosition(nIndex));
    Insert(nIndex + 1, pMenuItem);
	wxGISCommandBar::MoveCommandRight(nIndex);
}

void wxGISMenu::AddMenu(wxMenu* pMenu, wxString sName)
{
	IGISCommandBar* pGISCommandBar = dynamic_cast<IGISCommandBar*>(pMenu);
	if(pGISCommandBar)
		pGISCommandBar->Reference();
	SUBMENUDATA data = {AppendSubMenu(pMenu, sName), pGISCommandBar};
	m_SubmenuArray.push_back(data);
}

//------------
// wxGISToolbar
//------------------

BEGIN_EVENT_TABLE(wxGISToolBar, wxAuiToolBar)
	EVT_MOTION(wxGISToolBar::OnMotion)
END_EVENT_TABLE()

wxGISToolBar::wxGISToolBar(wxWindow* parent, wxWindowID id, const wxPoint& position, const wxSize& size, long style, const wxString& sName, const wxString& sCaption,
wxGISEnumCommandBars type ) : 
wxAuiToolBar(parent, id, position, size, style), wxGISCommandBar(sName, sCaption, type), m_pStatusBar(NULL)
{
	IApplication* pApp = dynamic_cast<IApplication*>(parent);
	if(pApp)
	{
		m_pStatusBar = pApp->GetStatusBar();
	}
}

wxGISToolBar::~wxGISToolBar(void)
{
}

void wxGISToolBar::OnMotion(wxMouseEvent& evt)
{
}