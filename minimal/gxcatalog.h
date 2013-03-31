#pragma once

#include "catalog.h"
#include "art\mainframe.xpm"
#include "gxselection.h"
//----------------
// wxGxCatalog
//--------------------
#define DISCCONNCAT wxT("wxGxDiscConnection")

class WXDLLIMPEXP_GIS_CLT wxGxCatalog :
    public IGxObjectUI,
    public IGxObjectEdit,
	public IGxObjectContainer,
	public IGxCatalog,
	public IConnectionPointContainer
{
public:
	wxGxCatalog(void);
	virtual ~wxGxCatalog(void);
	//IGxObject
	virtual bool Attach(IGxObject* pParent, IGxCatalog* pCatalog){m_pParent = NULL; m_pCatalog = NULL; return true;};
	virtual void Detach(void);
	virtual wxString GetName(void){ return wxString(_("Catalog"));};
	virtual wxString GetCategory(void){return wxString(wxT("Root"));};
	virtual IGxObject* GetParent(void){return NULL;};
	virtual void Refresh(void);
	//IGxObjectUI
	virtual wxIcon GetLargeImage(void){return wxIcon();};
	virtual wxIcon GetSmallimage(void){return wxIcon(mainframe_xpm);};
	virtual wxString ContextMenu(void){return wxString(wxT("wxGxCatalog.ContextMenu"));};
	virtual wxString NewMenu(void){return wxString(wxT("wxGxCatalog.NewMenu"));};
	//IGxObjectEdit
	virtual bool Delete(void){return false;};
	virtual bool CanDelete(void){return false;};
	virtual bool Rename(wxString NewName){return false;};
	virtual bool CanRename(void){return false;};
	virtual void EditProperties(wxWindow *parent);
	//IGxObjectContainer
	virtual bool AddChild(IGxObject* pChild);
	virtual bool DeleteChild(IGxObject* pChild);
	virtual bool AreChildrenViewable(void){return true;};
	virtual bool HasChildren(void){return true;};
	virtual GxObjectArray* GetChildren(void){return &m_Children;};
	//IGxCatalog
	virtual wxString ConstructFullName(IGxObject* pObject);
	virtual bool GetChildren(wxString sParentDir, wxArrayString* pFileNames, 
		GxObjectArray* pObjArray);
	virtual void SetShowHidden(bool bShowHidden);
	virtual void SetShowExt(bool bShowExt);
	virtual void ObjectAdded(IGxObject* pObject);
	virtual void ObjectChanged(IGxObject* pObject);
	virtual void ObjectDeleted(IGxObject* pObject);
	virtual void ObjectRefreshed(IGxObject* pObject);
	virtual IGxObject* ConnectFolder(wxString sPath);
	virtual void DisconnectFolder(wxString sPath);
	virtual void SetLocation(wxString sPath);
	//IConnectionPointContainer
	virtual long Advise(wxObject* pObject);
	//wxGxCatalog
	virtual void LoadChildren(wxXmlNode* pNode);
	virtual void EmptyChildren(void);
	virtual void LoadObjectFactories(wxXmlNode* pNode);
	virtual void EmptyObjectFactories(void);
    virtual void Init(void);
	virtual void SerializeDiscConnections(wxXmlNode* pNode, bool bStore = false);

	typedef struct _objfactory_desc
	{
		IGxObjectFactory* pFactory;
		bool bIsEnabled;
	} OBJFACTORYDESC;

	private:
		bool m_bIsChildrenLoaded;
		IGISConfig* m_pConf;
		std::vector<OBJFACTORYDESC> m_ObjectFactoriesArray;
		std::map<wxString, IGxObject*> m_DiscConnections;
		wxArrayString m_CatalogRootItemArray;

};