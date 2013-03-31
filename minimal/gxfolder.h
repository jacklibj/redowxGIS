#pragma once
#include "catalog.h"
#include "wx\dir.h"

class WXDLLIMPEXP_GIS_CLT wxGxFolder :
	public IGxObjectUI,
	public IGxObjectEdit,
	public IGxObjectContainer,
	public wxDirTraverser
{
public:
	wxGxFolder(wxString path, wxString Name, bool bShowHidden);
	virtual void Detach(void);
	virtual wxString GetName(void){return m_sName;};
	virtual wxString GetPath(void){return m_sPath;};
	virtual wxString GetCategory(void){return wxString(_("Folder"));};
	virtual void Refresh(void);
	//IGxObjectUI
	virtual wxIcon GetLargeImage(void);
	virtual wxIcon GetSmallImage(void);
	virtual wxString ContextMenu(void){ return wxString(wxT("wxGxFolder.ContextMenu"));};
	virtual wxString NewMenu(void){return wxString(wxT("wxGxFolder.NewMenu"));};
	//IGxObjectEdit
	virtual bool Delete(void);
	virtual bool CanDelete(void){return true;};
	virtual bool Rename(wxString NewName);
	virtual bool CanRename(void){return true;};
	virtual void EditProperties(wxWindow * parent);
	//IGxObjectContainer
	virtual bool DeleteChild(IGxObject* pChild);
	virtual bool AreChildrenViewable(void){return true;};
	virtual bool HasChildren(void){LoadChildren(); return m_Children.size() > 0 ? true : false; };
	//wxGxFolder
	virtual void LoadChildren(void);
	virtual void EmptyChildren(void);
	//wxDirTraverser
	virtual wxDirTraverseResult OnFile(const wxString& filename);
	virtual wxDirTraverseResult OnDir(const wxString& dirname);
protected:
	wxString m_sName, m_sPath;
	wxArrayString m_FileNames;
	bool m_bShowHidden;
	bool m_bIsChildrenLoaded;
};