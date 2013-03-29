#pragma once

#include "gxview.h"

#include <wx\treectrl.h>

#include "art\document.xpm"

class wxGxTreeItemData : public wxTreeItemData
{
public:
	wxGxTreeItemData(IGxObject* pObject, int smallimage_index, bool bExpandedOnce)
	{
		m_pObject = pObject;
		m_smallimage_index = smallimage_index;
		m_bExpandedOnce = bExpandedOnce;
	}

	~wxGxTreeItemData(void)
	{
		m_pObject = NULL;
	}

	IGxObject* m_pObject;
	int m_smallimage_index;
	bool m_bExpandedOnce;
};

class WXDLLIMPEXP_GIS_CLU wxGxTreeView :
	public wxTreeCtrl,
	public wxGxView,
	public IGxSelectionEvents,
	public IGxCatalogEvents
{
public:
	wxGxTreeView(wxWindow* parent, wxWindowID id = TREECTRLID);
	virtual ~wxGxTreeView(void);
	void AddTreeItem(IGxObject* pGxObject, wxTreeItemId hParent, bool sort = true);
	void AddRoot(IGxObject* pGxObject);
//
	void OnItemExpanding(wxTreeEvent* event);
	void OnBeginLabelEdit(wxTreeEvent* event);
	void OnEndLabelEdit(wxTreeEvent* event);
	void OnSelChange(wxTreeEvent* event);
//IGxView
	virtual bool Activate(wxGxApplication* application,  IGxCatalog* Catalog, wxXmlNode* pConf);
	virtual void Deactivate(void);
//IGxSelectionEvents
	virtual void OnSelectionChanged(IGxSelection* Selection, long nInitiator );
//IGxCatalogEvents
	virtual void OnObjectAdded(IGxObject* object);
	virtual void OnObjectChanged(IGxObject* object);
	virtual void OnObjectDeleted(IGxObject* object);
	virtual void OnObjectRefreshed(IGxObject* object);
	virtual void OnRefreshAll(void);

	
	typedef std::map<IGxObject*, wxTreeItemId> WETREEMAP;
private:
	wxImageList m_TreeImageList;
	WETREEMAP m_TreeMap;
	IConnectionPointContainer* m_pConnectionPointCatalog, *m_pConnectionPointSelection;
	long m_ConnectionPointCatalogCookie, m_ConnectionPointSelectionCookie;
	IGxSelection* m_pSelection;

	DECLARE_EVENT_TABLE()

};