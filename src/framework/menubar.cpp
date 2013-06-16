#include "wxgis/framework/menubar.h"
#include "wxgis/framework/config.h"
#include "wxgis/framework/toolbarmenu.h"

wxGISMenuBar::wxGISMenuBar(long style, IApplication* pApp, wxXmlNode* pConf) : wxMenuBar(style)
{
	if(!pConf || pApp == NULL)
		return;
	wxXmlNode *child = pConf->GetChildren();
	while (child)
	{
		wxString sMenuName = child->GetPropVal(wxT("name"), wxT(""));
		AddMenu(pApp->GetCommandBar(sMenuName));
		child = child->GetNext();
	}
}

wxGISMenuBar::~wxGISMenuBar(void)
{
	m_menus.Clear();
}

bool wxGISMenuBar::IsMenuBarMenu(wxString sMenuName)
{
	for(size_t i = 0; i< m_MenubarArray.size(); i++)
	{
		if(m_MenubarArray[i]->GetName() == sMenuName)
			return true;
	}
	return false;
}

COMMANDBARARRAY* wxGISMenuBar::GetMenuBarArray(void)
{
	return &m_MenubarArray;
}

void wxGISMenuBar::MoveLeft(int pos)
{
	if(pos == 0)
		return;

	IGISCommandBar* val = m_MenubarArray[pos];
	m_MenubarArray[pos] = m_MenubarArray[pos - 1];
	m_MenubarArray[pos - 1] = val;
	Insert(pos - 1, Remove(pos), m_MenubarArray[pos-1]->GetCaption());
}

void wxGISMenuBar::MoveRight(int pos)
{
	if(pos == m_MenubarArray.size() - 1)
		return;

	IGISCommandBar* val = m_MenubarArray[pos];
	m_MenubarArray[pos] = m_MenubarArray[pos + 1];
	m_MenubarArray[pos + 1] = val;
	Insert(pos + 1, Remove(pos), m_MenubarArray[pos + 1]->GetCaption());
}

void wxGISMenuBar::RemoveMenu(IGISCommandBar* pBar)
{
	for (size_t i = 0; i< m_MenubarArray.size(); i++)
	{
		if(m_MenubarArray[i] == pBar)
		{
			Remove(i);
			m_MenubarArray.erase(m_MenubarArray.begin() + i);
		}
	}
}

bool wxGISMenuBar::AddMenu(IGISCommandBar* pBar)
{
	wxMenu* pMenu = dynamic_cast<wxMenu*>(pBar);
	if(pMenu)
	{
		Append(pMenu, pBar->GetCaption());
		m_MenubarArray.push_back(pBar);
		return true;
	}
	return false;
}

void wxGISMenuBar::Serialize(wxXmlNode* pConf)
{
	if(!pConf)
		return;
	wxGISConfig::DeleteNodeChildren(pConf);
	for(size_t i = m_MenubarArray.size(); i > 0; i--)
	{
		wxString sName = m_MenubarArray[i - 1]->GetName();
		wxXmlNode* pNewNode = new wxXmlNode(pConf, wxXML_ELEMENT_NODE, wxString(wxT("menu")));
		pNewNode->AddProperty(wxT("name"),sName);
	}
}