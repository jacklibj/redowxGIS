#pragma once
#include "wxgis\framework\framework.h"
#include "commandbar.h"

class WXDLLIMPEXP_GIS_FRW wxGISMenuBar :
	public wxMenuBar
{
public:
	wxGISMenuBar(long style = 0, IApplication* pApp = NULL, wxXmlNode* pConf = NULL);
	virtual ~wxGISMenuBar(void);
	virtual bool IsMenuBarMenu(wxString sMenuName);
	virtual COMMANDBARARRAY* GetMenuBarArray(void);
	virtual void MoveLeft(int pos);
	virtual void MoveRight(int pos);
	virtual void RemoveMenu(IGISCommandBar* pBar);
	virtual bool AddMenu(IGISCommandBar* pBar);
	virtual void Serialize(wxXmlNode* pConf);

protected:
	COMMANDBARARRAY m_MenubarArray;
};