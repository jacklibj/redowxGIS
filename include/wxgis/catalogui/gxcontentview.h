#pragma once
#include "wxgis/catalogui/gxtabview.h"
#include <wx/listctrl.h>


class WXDLLIMPEXP_GIS_CLU wxGxContentView :
	public wxListCtrl,
	public wxGxView,
	public IGxSelectionEvents,
	public IGxCatalogEvents
{
public: 
	typedef enum _liststyle{ REPORT, SMALL, LARGE, LIST } LISTSTYLE, *LPLISTSTYLE;

	wxGxContentView(wxWindow* parent, wxWindowID id = LISTCTRLID, const wxPoint& pos = wxDefaultPosition,
		                  const wxSize& size = wxDefaultSize);
    virtual ~wxGxContentView(void);
	void Serialize(wxXmlNode* pRootNode, bool bStore);
	void AddObject(IGxObject* pObject);
	void SetStyle(LISTSTYLE style);
	void ResetContents(void);
	//IGxView
	virtual bool Activate(wxGxApplication* application, IGxCatalog* Catalog, wxXmlNode* pConf);
	virtual bool Deactivate(void);
	virtual bool Applies(IGxSelection* Selection);
	//IGxSelectionEvents
	virtual void OnSelectionChanged(IGxSelection* Selection, long nInitiator);
//IGxCatalogEvents
	virtual void OnObjectAdded(IGxObject* object);
	virtual void OnObjectChanged(IGxObject* object);
	virtual void OnObjectDeleted(IGxObject* object);
	virtual void OnObjectRefreshed(IGxObject* object);
	virtual void OnRefreshAll(void);
//events
	void OnColClick(wxListEvents& event);
	void OnContentMenu(wxContextMenuEvent& event);
	void ShowContextMenu(const wxPoint& pos);
	void SetColumnImage(int col, int image);
	void OnActivated(wxListEvent& event);
	void OnBeginLabelEdit(wxListEvent& event);
	void OnEndLabelEdit(wxListEvent& event);
	void OnSelected(wxListEvent& event);
	void OnDeselected(wxListEvent& event);
	void OnLeftDown(wxMouseEvent& event);

	typedef struct _itemdata
	{
		IGxObject* pObject;
		int iImageIndex;
		bool bContainer;
	}ITEMDATA, *LPITEMDATA;

private:
	bool m_bSortAsc;
	short m_currentSortCol;
	LISTSTYLE m_current_style;
	wxImageList m_imageListSmall, m_imageListLarge;
	IConnectionPointContainer* m_pConnectionPointCatalog;
	long m_pConnectionPointCatalogCookie;
	IGxSelection* m_pSelection;
	long style;
	IGxObject* m_pParentGxObject;
	bool m_bCtrlDown;

DECLARE_EVENT_TABLE()
};

