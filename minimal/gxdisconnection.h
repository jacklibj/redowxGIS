#pragma once
#include "wx\imaglist.h"
#include "gxfolder.h"

class WXDLLIMPEXP_GIS_CLT wxGxDiscConnection :
	public wxGxFolder
{
public:
	wxGxDiscConnection(wxString Path, wxString Name, bool bShowHidden);
	virtual ~wxGxDiscConnection(void);
	//IGxObject
	virtual wxString GetCategory(void){return wxString(_("Folder Connection"));};
	//IGxObjectUI
	virtual wxIcon GetLargeImage(void);
	virtual wxIcon GetSmallImage(void);
	virtual wxString ContextMenu(void){return wxString(wxT("wxGxDIscConnection.Contextmenu"));};
	virtual wxString NewMenu(void){return wxString(wxT("wxGxDisconnection.NewMenu"));};
	//IGxObjectEdit
	virtual bool Delete(void);
	virtual bool Rename(wxString NewName);
	virtual void EditProperties(wxWindow * parent);
	//IGxObjectContainer
	//wxGxDisconnection
	//wxDirtraverser
protected:
	wxImageList m_ImageListSmall, m_ImageListLarge;
};