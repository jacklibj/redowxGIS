/******************************************************************************
 * Project:  wxGIS (GIS Catalog)
 * Purpose:  wxGxContentView class.
 * Author:   Bishop (aka Barishnikov Dmitriy), polimax@mail.ru
 ******************************************************************************
*   Copyright (C) 2009  Bishop
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include "wxgis/catalogui/gxcontentview.h"
#include "../../art/cont_view_16.xpm"
#include "../../art/cont_view_48.xpm"

BEGIN_EVENT_TABLE(wxGxContentView, wxListCtrl)
	EVT_LIST_BEGIN_LABEL_EDIT(LISTCTRLID, wxGxContentView::OnBeginLabelEdit)
	EVT_LIST_END_LABEL_EDIT(LISTCTRLID, wxGxContentView::OnEndLabelEdit)
	EVT_LIST_ITEM_SELECTED(LISTCTRLID, wxGxContentView::OnSelected)
	EVT_LIST_ITEM_DESELECTED(LISTCTRLID, wxGxContentView::OnDeselected)
	EVT_LIST_ITEM_ACTIVATED(LISTCTRLID, wxGxContentView::OnActivated)

	EVT_LIST_COL_CLICK(LISTCTRLID, wxGxContentView::OnColClick)
	EVT_CONTEXT_MENU(wxGxContentView::OnContextMenu)
	EVT_LEFT_DOWN(wxGxContentView::OnLeftDown)
END_EVENT_TABLE()

int wxCALLBACK MyCompareFunction(long item1, long item2, long sortData)
{
	wxGxContentView::LPITEMDATA pItem1 = (wxGxContentView::LPITEMDATA)item1;
	wxGxContentView::LPITEMDATA pItem2 = (wxGxContentView::LPITEMDATA)item2;
	// inverse the order
	if(pItem1->bContainer && !pItem2->bContainer)
		return sortData == 0 ? 1: -1;
	if(!pItem1->bContainer && pItem2->bContainer)
		return sortData == 0 ? -1: 1;

	return pItem1->pObject->GetName().CmpNoCase(pItem2->pObject->GetName()) * (sortData == 0 ? -1 : 1);
}

wxGxContentView::wxGxContentView(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) :
wxListCtrl(parent, id, pos, size, wxLC_REPORT | wxBORDER_NONE |wxLC_EDIT_LABELS | wxLC_SORT_ASCENDING), m_bSortAsc(true), m_current_style(REPORT), m_pConnectionPointCatalog(NULL), m_ConnectionPointCatalogCookie(-1), m_pParentGxObject(NULL), m_currentSortCol(0), m_pSelection(NULL)
{
	m_style = wxBORDER_NONE | wxLC_EDIT_LABELS;
	InsertColumn(0, _("Name"), wxLIST_FORMAT_LEFT, 150);
	InsertColumn(1,_("Type"), wxLIST_FORMAT_LEFT, 250);

	m_ImageListSmall.Create(16, 16);
	m_ImageListLarge.Create(48, 48);
	//
	//
	m_ImageListLarge.Add(wxBitmap(cont_view_48_xpm));
	m_ImageListSmall.Add(wxBitmap(cont_view_16_xpm));

	SetImageList(&m_ImageListLarge, wxIMAGE_LIST_NORMAL);
	SetImageList(&m_ImageListSmall, wxIMAGE_LIST_SMALL);
	m_bCtrlDown = false;
}

wxGxContentView::~wxGxContentView(void)
{
	ResetContents();
}

bool wxGxContentView::Activate(wxGxApplication* application, IGxCatalog* Catalog, wxXmlNode* pConf)
{
	wxGxView::Activate(application,Catalog, pConf);
	Serialize(m_pXmlConf, false);

	m_pConnectionPointCatalog = dynamic_cast<IConnectionPointContainer*>( m_pCatalog );
	if(m_pConnectionPointCatalog != NULL)
		m_ConnectionPointCatalogCookie = m_pConnectionPointCatalog->Advise(this);

	m_pSelection = m_pCatalog->GetSelection();

	return true;
}

void wxGxContentView::Deactivate(void)
{
	//if(m_ConnectionPointSelectionCookie != -1)
	//	m_pConnectionPointSelection->Unadvise(m_ConnectionPointSelectionCookie);
	if(m_ConnectionPointCatalogCookie != -1)
		m_pConnectionPointCatalog->Unadvise(m_ConnectionPointCatalogCookie);

	Serialize(m_pXmlConf, true);
	wxGxView::Deactivate();
}

void wxGxContentView::Serialize(wxXmlNode* pRootNode, bool bStore)
{
	if(pRootNode == NULL)
		return;
}

void wxGxContentView::AddObject(IGxObject* pObject)
{
	if(pObject == NULL)
		return;
	IGxObjectUI* pObjUI = dynamic_cast<IGxObjectUI*>(pObject);
	wxIcon icon_small, icon_large;
	if (pObjUI != NULL)
	{
		icon_small = pObjUI->GetSmallImage();
		icon_large = pObjUI->GetLargeImage();
	}

	int pos(0);
	if(icon_small.IsOk())
		pos = m_ImageListSmall.Add(icon_small);
	else
		pos = m_ImageListSmall.Add(m_ImageListSmall.GetIcon(2));

	if(icon_large.IsOk())
		pos = m_ImageListLarge.Add(icon_large);
	else
		pos = m_ImageListLarge.Add(m_ImageListLarge.GetIcon(2));

	IGxObjectContainer* pGxObjectContainer = dynamic_cast<IGxObjectContainer*>(pObject);

	LPITEMDATA pData = new _itemdata;
	pData->pObject = pObject;
	pData->iImageIndex = pos;
	pData->bContainer = pGxObjectContainer == NULL ? false : true;

	wxString name = pObject->GetName();
	wxString type = pObject->GetCategory();

	long ListItemID = InsertItem(GetItemCount(), name, pos);
	SetItem(ListItemID, 1, type);
	SetItemPtrData(ListItemID, (wxUIntPtr) pData);

	wxListCtrl::Refresh();
}

void wxGxContentView::OnColClick(wxListEvent& event)
{
	int col = event.GetColumn();
	if(col != 0)
		return;

	m_bSortAsc = !m_bSortAsc;
	SortItems(MyCompareFunction, m_bSortAsc);

	SetColumnImage(col, m_bSortAsc ? 0 : 1);
}

void wxGxContentView::OnContextMenu(wxContextMenuEvent& event)
{
	wxPoint point = event.GetPosition();
	// if from keyboard
	if (point.x == -1 && point.y == -1)
	{
		wxSize size = GetSize();
		point.x = size.x / 2;
		point.y = size.y / 2;
	}
	else
	{
		point = ScreenToClient(point);
	}
	ShowContextMenu(point);
}

void wxGxContentView::OnSelected(wxListEvent& event)
{
	event.Skip();
	LPITEMDATA pItemData = (LPITEMDATA)event.GetData();
	if(pItemData == NULL)
		return;

	m_pSelection->Select(pItemData->pObject, m_bCtrlDown, NOTFIRESELID);
}

void wxGxContentView::OnDeselected(wxListEvent& event)
{
	event.Skip();
	LPITEMDATA pItemData = (LPITEMDATA)event.GetData();
	if(pItemData == NULL)
		return;

	m_pSelection->Unselect(pItemData->pObject, NOTFIRESELID);
}

void wxGxContentView::ShowContextMenu(const wxPoint& pos)
{

}

void wxGxContentView::SetColumnImage(int col, int image)
{
	wxListItem item;
	item.SetMask(wxLIST_MASK_IMAGE);
	item.SetImage(image);
	SetColumn(col, item);
}

void wxGxContentView::OnActivated(wxListEvent& event)
{
	event.Skip();
	//dbl click
	LPITEMDATA pItemData = (LPITEMDATA)event.GetData();
	if(pItemData == NULL)
		return;

	IGxObjectWizard* pGxObjectWizard = dynamic_cast<IGxObjectWizard*>(pItemData->pObject);
	if(pGxObjectWizard != NULL)
		if(!pGxObjectWizard->Invoke(this))
			return;

	IGxObjectContainer* pGxObjectContainer = dynamic_cast<IGxObjectContainer*>(pItemData->pObject);
	if (pGxObjectContainer != NULL && pGxObjectContainer->HasChildren() )
	{
		m_pSelection->Select(pItemData->pObject, false, GetId());
	}
}

void wxGxContentView::SetStyle(LISTSTYLE style)
{
	m_current_style = style;
	switch(style)
	{
	case REPORT:
		SetWindowStyleFlag(m_style | wxLC_REPORT);
		break;
	case SMALL:
		SetWindowStyleFlag(m_style | wxLC_SMALL_ICON);
		break;
	case LARGE:
		SetWindowStyleFlag(m_style | wxLC_ICON);
		break;
	case LIST:
		SetWindowStyleFlag(m_style | wxLC_LIST);
		break;

	}
}

void wxGxContentView::OnBeginLabelEdit(wxListEvent& event)
{
	LPITEMDATA pItemData = (LPITEMDATA)event.GetData();
	if (pItemData == NULL)
	{
		event.Veto();
		return;
	}
	IGxObjectEdit* pObjEdit = dynamic_cast<IGxObjectEdit*>(pItemData->pObject);
	if (pObjEdit == NULL)
	{
		event.Veto();
		return;
	}
	if (!pObjEdit->CanRename())
	{
		event.Veto();
		return;
	}
}

void wxGxContentView::OnEndLabelEdit(wxListEvent& event)
{
	if ( event.GetLabel().Len() == 0)
	{
		wxMessageBox(_("wxGxContentView: Too short label. Please add longer text!"));
		event.Veto();
	}
	else
	{
		LPITEMDATA pItemData = (LPITEMDATA)event.GetData();
		if (pItemData == NULL)
		{

		    event.Veto();
			return;
		}
		IGxObjectEdit* pObjEdit = dynamic_cast<IGxObjectEdit*>(pItemData->pObject);
		if (pObjEdit == NULL)
		{
			event.Veto();
			return;
		}
		pObjEdit->Rename(event.GetLabel());
	}
}

void wxGxContentView::OnObjectAdded(IGxObject* pObj)
{

}

void wxGxContentView::OnObjectDeleted(IGxObject* pObj)
{

}

void wxGxContentView::OnObjectChanged(IGxObject* pObj)
{

}

void wxGxContentView::OnObjectRefreshed(IGxObject* pObj)
{

}

void wxGxContentView::OnRefreshAll(void)
{

}

void wxGxContentView::OnSelectionChanged(IGxSelection* Selection, long nInitiator)
{
	if(nInitiator == GetId())
		return;
	GxObjectArray* pGxObjectArray = m_pSelection->GetSelectedObjects();
	if(pGxObjectArray == NULL || pGxObjectArray->size() == 0)
		return;
	IGxObject* pGxObj = pGxObjectArray->at(pGxObjectArray->size() - 1);
	if(m_pParentGxObject == pGxObj)
		return;

	//reset
	ResetContents();

	IGxObjectContainer* pObjContainer = dynamic_cast<IGxObjectContainer*>(pGxObj);
	if(pObjContainer == NULL || !pObjContainer->HasChildren())
		return;
	GxObjectArray* pArr = pObjContainer->GetChildren();
	for(size_t i = 0; i< pArr->size(); i++)
	{
		AddObject(pArr->at(i));
	}

	SortItems(MyCompareFunction, m_bSortAsc);
	SetColumnImage(m_currentSortCol, m_bSortAsc ? 0 : 1);

	m_pParentGxObject = pGxObj;
}

bool wxGxContentView::Applies(IGxSelection* Selection)
{
	if(Selection == NULL)
		return false;

	GxObjectArray* pGxObjectArray = Selection->GetSelectedObjects();
	for (size_t i = 0; i < pGxObjectArray->size(); i++)
	{
	     IGxObjectContainer* pGxObjectContainer = dynamic_cast<IGxObjectContainer*>( pGxObjectArray->at(i) );
		 if( pGxObjectContainer != NULL)
			 return true;
	}
	return false;
}

void wxGxContentView::ResetContents(void)
{
	for(long i = 0; i < GetItemCount(); i++ )
		delete (LPITEMDATA)GetItemData(i);
	DeleteAllItems();
}

void wxGxContentView::OnLeftDown(wxMouseEvent& event)
{
	event.Skip();
	m_bCtrlDown = event.m_controlDown;
}