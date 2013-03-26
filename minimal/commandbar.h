#pragma once
#include "wxgis\framework\framework.h"
#include "wx\aui\aui.h"

//----------
// wxGISCommandBar
//----------

class WXDLLIMPEXP_GIS_FRW wxGISCommandBar :
	public IGISCommandBar
{
public:
	wxGISCommandBar(const wxString& sName = NONAME, const wxString& sCaption = _("No Caption"), wxGISEnumCommandBars type = enumGISCBNone);
	virtual ~wxGISCommandBar(void);
	virtual void SetName(const wxString& sName);
	virtual wxString GetName(void);
	virtual void SetCaption(const wxString& sCaption);
	virtual wxString GetCaption(void);
	virtual void SetType(wxGISEnumCommandBars type);
	virtual wxGISEnumCommandBars GetType(void);
	virtual void AddCommand(ICommand* pCmd);
	virtual void AddMenu(wxMenu* pMenu, wxString sName) = 0;
	virtual void RemoveCommand(size_t nIndex);
	virtual void MoveCommandLeft(size_t nIndex);
	virtual void MoveCommandRight(size_t nIndex);
	virtual size_t GetCommandCount(void);
	virtual ICommand* GetCommand(size_t nIndex);
	virtual void Serialize(IApplication* pApp, wxXmlNode* pNode, bool bStore = false);
protected:
	COMMANDARRAY m_CommmandArray;
	wxString m_sName;
	wxString m_sCaption;
	wxGISEnumCommandBars m_type;

};