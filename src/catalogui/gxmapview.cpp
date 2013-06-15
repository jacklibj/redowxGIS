#include "wxgis/catalogui/gxmapview.h"
#include "wxgis/carto/featurelayer.h"
#include "wxgis/carto/rasterlayer.h"
#include "wxgis/catalogui/gxapplication.h"

BEGIN_EVENT_TABLE(wxGxMapView, wxGISMapView)
	EVT_LEFT_DOWN(wxGxMapView::OnMouseDown)
	EVT_MIDDLE_DOWN(wxGxMapView::OnMouseDown)
	EVT_RIGHT_DOWN(wxGxMapView::OnMouseDown)
	EVT_LEFT_UP(wxGxMapView::OnMouseUp)
	EVT_MIDDLE_UP(wxGxMapView::OnMouseUp)
	EVT_RIGHT_UP(wxGxMapView::OnMouseUp)
	EVT_LEFT_DCLICK(wxGxMapView::OnMouseDoubleClick)
	EVT_MIDDLE_DCLICK(wxGxMapView::OnMouseDoubleClick)
	EVT_RIGHT_DCLICK(wxGxMapView::OnMouseDoubleClick)
	EVT_MOTION(wxGxMapView::OnMouseMove)
END_EVENT_TABLE()

wxGxMapView::wxGxMapView(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : wxGISMapView(parent, id, pos, size)
{
	m_sViewName = wxString(_("Geography View"));
}

wxGxMapView::~wxGxMapView(void)
{

}

bool wxGxMapView::Activate(wxGxApplication* application, IGxCatalog* Catalog, wxXmlNode* pConf)
{
	wxGxView::Activate(application, Catalog, pConf);
	//

	m_pSelection = m_pCatalog->GetSelection();
	m_pStatusBar = application->GetStatusBar();
	GetTrackCancel()->SetProgressor(m_pStatusBar->GetAnimation());
	//

	//
	return true;
}

void wxGxMapView::Deactivate(void)
{
	//Serialize(m_pXmlConf, true);
	wxGxView::Deactivate();
}

bool wxGxMapView::Applies(IGxSelection* Selection)
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
			case enumGISRasterDataset:
			case enumGISFeatureDataset:
				return true;
			case enumGISTableDataset:
				break;
			}
		}
	}
	return false;
}

void wxGxMapView::OnSelectionChanged(IGxSelection* Selection, long nInitiator)
{
	if(nInitiator == GetId())
		return;

	GxObjectArray* pGxObjectArray = m_pSelection->GetSelectedObjects();
	if(pGxObjectArray == NULL || pGxObjectArray->size() == 0)
		return;
	IGxObject* pGxObj = pGxObjectArray->at( pGxObjectArray->size() - 1);
	if(m_pParentGxObject == pGxObj)
		return;

	IGxDataset* pGxDataset = dynamic_cast<IGxDataset*>(pGxObj);
	if(pGxDataset == NULL)
		return;
	wxGISDataset* pwxGISDataset = pGxDataset->GetDataset();
	if(pwxGISDataset == NULL)
		return;

	wxGISEnumDatasetType type = pwxGISDataset->GetType();
	wxGISLayer* pwxGISLayer = NULL;
	switch(type)
	{
	case enumGISFeatureDataset:
		pwxGISLayer = new wxGISFeatureLayer(pwxGISDataset);
		break;
	case enumGISRasterDataset:
		pwxGISLayer = new wxGISRasterLayer(pwxGISDataset);
		break;
	default:
		break;
	}

	if(pwxGISLayer)
		pwxGISLayer->SetName(pGxObj->GetName());

	//the pOGRLayer
	wsDELETE(pwxGISDataset);

	//
	//
	//

	ClearLayers();

	if(pwxGISLayer && pwxGISLayer->IsValid())
		AddLayer(pwxGISLayer);

	m_pParentGxObject = pGxObj;

	wxGISMapView::Refresh();
}

void wxGxMapView::OnMouseMove(wxMouseEvent& event)
{
	IDisplayTransformation* pDisplayTransformation = pGISScreenDisplay->GetDisplayTransformation();
	if (pDisplayTransformation)
	{
		wxPoint* pDCPoint = new wxPoint(event.m_x, event.m_y);
		OGRRawPoint* pGeoPoints = pDisplayTransformation->TransformCoordDC2World(pDCPoint, 1);
		m_pStatusBar->SetMessage(wxString::Format(_("X: %.4f Y: %.4f"), pGeoPoints->x, pGeoPoints->y), 2);
		delete [] pDCPoint;
		delete [] pGeoPoints;
	}

	if(m_pApplication)
		m_pApplication->OnMouseMove(event);
	event.Skip();
}

void wxGxMapView::OnMouseDown(wxMouseEvent& event)
{
	if(m_pApplication)
		m_pApplication->OnMouseMove(event);
	event.Skip();
}

void wxGxMapView::OnMouseUp(wxMouseEvent& event)
{
	if(m_pApplication)
		m_pApplication->OnMouseUp(event);
	event.Skip();
}

void wxGxMapView::OnMouseDoubleClick(wxMouseEvent& event)
{
	if(m_pApplication)
		m_pApplication->OnMouseDoubleClick(event);
	event.Skip();
}