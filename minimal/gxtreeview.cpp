#include "gxtreeview.h"

BEGIN_EVENT_TABLE(wxGxTreeView, wxTreeCtrl)
	EVT_TREE_BEGIN_LABEL_EDIT(TREECTRLID, wxGxTreeView::OnBeginLabelEdit)
	EVT_TREE_END_LABEL_EDIT(TREECTRLID, wxGxTreeView::OnEndLabelEdit)
	EVT_TREE_ITEM_EXPANDING(TREECTRLID, wxGxTreeView::OnItemExpanding)
	EVT_TREE_SEL_CHANGED(TREECTRLID, wxGxTreeView::OnSelChanged)
END_EVENT_TABLE(wxGxTreeView, wxTreeCtrl)

wxGxTreeView::wxGxTreeView(wxWindow* parent, wxWindowID id) 
: wxTreeCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxBORDER_NONE | wxTR_EDIT_LABELS
      /*|wxTR_MULTIPLE| wxTR_HIDE_ROOT | wxTR_LINES_AT_ROOT*/ ,
	  wxDefaultValidator, wxT("wxGxTreeView")), 
	  m_pConnectionPointCatalog(NULL), m_pConnectionPointSelection(NULL), m_ConnectionPointCatalogCookie(-1),m_ConnectionPointSelectionCookie(-1)
{
	m_TreeImageList.Create(16,16);
	SetImageList(&m_TreeImageList);
}

wxGxTreeView::~wxGxTreeView(void)
{
}

void wxGxTreeView::AddRoot(IGxObject* pGxObject)
{
	if(NULL == pGxObject)
		return;
	IGxObjectUI* pObjUI = dynamic_cast<IGxObjectUI*>(pGxObject);
	wxIcon icon;
	if(pObjUI != NULL)
		icon = pObjUI->GetSmallImage();
	int pos(-1);
	if(icon.IsOk())
		pos = m_TreeImageList.Add(icon);

	wxGxTreeItemData* pData = new wxGxTreeItemData(pGxObject, pos, false);

	wxTreeItemId wxTreeItemIdRoot = wxTreeCtrl::AddRoot(pGxObject->GetName(), pos, -1, pData);
	m_TreeMap[pGxObject] =wxTreeItemIdRoot;


    SetItemHasChildren(wxTreeItemIdRoot);
	Expand(wxTreeItemIdRoot);

    SortChildren(wxTreeItemIdRoot);
	wxTreeCtrl::Refresh();
}

void wxGxTreeView::AddTreeItem(IGxObject* pGxObject, wxTreeItemId hParent, bool sort)
{
	if(NULL == pGxObject)
		return;
	IGxObjectUI* pObjUI = dynamic_cast<IGxObjectUI*>(pGxObject);
	wxIcon icon;
	if(pObjUI != NULL)
		icon = pObjUI->GetSmallImage();
	int pos(-1);
	if(icon.IsOk())
		pos = m_TreeImageList.Add(icon);

	wxGxTreeItemData* pData = new wxGxTreeItemData(pGxObject, pos, false);

	wxTreeItemId NewTreeItem = AppendItem(hParent, pGxObject->GetName(), pos, -1, pData);
	m_TreeMap[pGxObject] =NewTreeItem;

	IGxObjectContainer* pContainer = dynamic_cast<IGxObjectContainer*>(pGxObject);
	if(pContainer != NULL)
		if(pContainer->AreChildrenViewable())
			SetItemHasChildren(NewTreeItem);

	if(sort)
		SortChildren(hParent);
	wxTreeCtrl::Refresh();
}

void wxGxTreeView::OnItemExpanding(wxTreeEvent& event)
{
	wxTreeItemId item = event.GetItem();

	if(!item.IsOk())
		return;

	wxGxTreeItemData* pData = (wxGxTreeItemData*)GetItemData(item);
	if(pData != NULL)
	{
		IGxObjectContainer* pGxObjectContainer = dynamic_cast<IGxObjectContainer*>(pData->m_pObject);
		if(pGxObjectContainer != NULL)
		{
			if(pGxObjectContainer->HasChildren() && pGxObjectContainer->AreChildrenViewable())
			{
				if(pData->m_bExpandedOnce == false)
				{
					GxObjectArray* pArr = pGxObjectContainer->GetChildren();
					if(pArr != NULL)
					{
						if(pArr->size() != 0)
						{

							for(size_t i = 0; i< pArr->size(); i++)
								AddTreeItem(pArr->at(i), item, false);
							pData->m_bExpandedOnce = true;
							return;
						}
					}
				}
				else
					return;
			}
			else
			{
				SetItemHasChildren(item, false);
				return;
			}
		}
	}
	SetItemHasChildren(item, false);
}

void wxGxTreeView::OnBeginLabelEdit(wxTreeEvent& event)
{
	wxTreeItemId item = event.GetItem();
	if(!item.IsOk())
		return;
	wxGxTreeItemData* pData = (wxGxTreeItemData*)GetItemData(item);
	if(pData == NULL)
	{
		event.Veto();
		return;
	}
	IGxObjectEdit* pObjEdit = dynamic_cast<IGxObjectEdit*>(pData->m_pObject);
	if(pObjEdit == NULL)
	{
		event.Veto();
		return;
	}
	if(!pObjEdit->CanRename())
	{
		event.Veto();
		return;
	}
}

void wxGxTreeView::OnEndLabelEdit(wxTreeEvent& event)
{
	if( event.GetLabel().IsEmpty() )
	{
		wxMessageDialog(this, _("Too short label. Please add longer text!"), _("Warning"), wxOK | wxICON_EXCLAMATION);
		event.Veto();
	}
	else
	{
		wxTreeItemId item = event.GetItem();
		if(!item.IsOk())
		{
			event.Veto();
			return;
		}
		wxGxTreeItemData* pData = (wxGxTreeItemData*)GetItemData(event.GetItem());
		if(pData == NULL)
		{
			event.Veto();
			return;
		}
		IGxObjectEdit* pObjEdit = dynamic_cast<IGxObjectEdit*>(pData->m_pObject);
		if(pObjEdit == NULL)
		{
			event.Veto();
			return;
		}
		pObjEdit->Rename(event.GetLabel());
	}
}

bool wxGxTreeView::Activate(wxGxApplication* application, IGxCatalog* Catalog, wxXmlNode* pConf)
{
	if(!wxGxView::Activate(application, Catalog, pConf))
		return false;

	AddRoot(dynamic_cast<IGxObject*>(m_pCatalog));

	m_pConnectionPointCatalog = dynamic_cast<IConnectionPointContainer*>( m_pCatalog );
	if(m_pConnectionPointCatalog != NULL)
		m_ConnectionPointCatalogCookie =  m_pConnectionPointCatalog->Advise(this);

	m_pSelection = m_pCatalog->GetSelection();
	m_pConnectionPointSelection = dynamic_cast<IConnectionPointContainer*>( m_pSelection );
	if(m_pConnectionPointSelection != NULL)
		m_ConnectionPointSelectionCookie = m_pConnectionPointSelection->Advise(this);

	return true;
}

void wxGxTreeView::Deactivate(void)
{
	if(m_ConnectionPointCatalogCookie != -1)
		m_pConnectionPointSelection->Unadvise(m_ConnectionPointCatalogCookie);
	if(m_ConnectionPointCatalogCookie != -1)
		m_pConnectionPointCatalog->Unadvise(m_ConnectionPointCatalogCookie);

	wxGxView::Deactivate();
}