#include "wxgis/catalogui/gxtableview.h"
#include "wxgis/carto/featuredataset.h"

wxGxTableView::wxGxTableView(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : wxGISTableView(parent, id, pos, size)
{
	m_sViewName = wxString(_("Table View"));
	SetReadOnly(true);
}

wxGxTableView::~wxGxTableView(void)
{

}

bool wxGxTableView::Activate(wxGxApplication* application, IGxCatalog* Catalog, wxXmlNode* pConf)
{
	wxGxView::Activate(application, Catalog, pConf);

	m_pSelection = m_pCatalog->GetSelection();
	return true;
}

void wxGxTableView::Deactivate(void)
{
	wxGxView::Deactivate();
}

bool wxGxTableView::Applies(IGxSelection* Selection)
{
	if(Selection == NULL)
		return false;
	GxObjectArray* pGxObjectArray = Selection->GetSelectedObjects();
	for (size_t i = 0; i < pGxObjectArray->size(); i++)
	{
		IGxDataset* pGxDataset = dynamic_cast<IGxDataset*>( pGxObjectArray->at(i) );
		if (pGxDataset != NULL)
		{
			wxGISEnumDatasetType type = pGxDataset->GetType();
			switch(type)
			{
			case enumGISTableDataset:
			case enumGISFeatureDataset:
				return true;
			case enumGISRasterDataset:
				break;
			}
		}
	}
	return false;
}

void wxGxTableView::OnSelectionChanged(IGxSelection* Selection, long nInitiator)
{
	if(nInitiator == GetId())
		return;

	GxObjectArray* pGxObjectArray = m_pSelection->GetSelectedObjects();
	if(pGxObjectArray == NULL || pGxObjectArray->size() == 0)
		return;
	IGxObject* pGxObj = pGxObjectArray->at(pGxObjectArray->size() - 1);
	if(m_pParentGxObject == pGxObj)
		return;

	IGxDataset* pGxDataset = dynamic_cast<IGxDataset*>(pGxObj);
	if(pGxDataset == NULL)
		return;

	wxGISDataset* pwxGISDataset = pGxDataset->GetDataset();
	if(pwxGISDataset == NULL)
		return;

	//

	wxGISTable* pTable = new wxGISTable(pwxGISDataset);
	wxGISTableView::SetTable(pTable, true);

	//

	m_pParentGxObject = pGxObj;
	wxWindow::Refresh();
}