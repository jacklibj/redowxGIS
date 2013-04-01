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

		}
	}
}