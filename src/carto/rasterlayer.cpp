#include "wxgis/carto/rasterlayer.h"
#include "wxgis/carto/rasterrenderer.h"

wxGISRasterLayer::wxGISRasterLayer(wxGISDataset* pwxGISDataset) : wxGISLayer(), m_pwxGISRasterDataset(NULL), m_pRasterRenderer(NULL)
{
	m_pwxGISRasterDataset = dynamic_cast<wxGISRasterDataset*>(pwxGISDataset);
	if(m_pwxGISRasterDataset)
	{
		m_pwxGISRasterDataset->Reference();
		//
		m_pRasterRenderer = new wxGISRasterRGBRenderer();
	}
}

wxGISRasterLayer::~wxGISRasterLayer(void)
{
	wxDELETE(m_pRasterRenderer);
	wsDELETE(m_pwxGISRasterDataset);
}

void wxGISRasterLayer::Draw(wxGISEnumDrawPhase DrawPhase, ICachedDisplay* pDisplay, ITrackCancel* pTrackCancel)
{
	IDisplayTransformation* pDisplayTransformation  = pDisplay->GetDisplayTransformation();
	if(!pDisplayTransformation)
		return;
	//1. get envelope
	OGREnvelope Env = pDisplayTransformation->GetVisibleBounds();
	OGREnvelope* LayerEnv = m_pwxGISRasterDataset->GetEnvelope();
	OGRSpatialReference* pEnvSpaRef = pDisplayTransformation->GetSpatialReference();
	OGRSpatialReference* pLayerSpaRef = m_pwxGISRasterDataset->GetSpatialReference();
	
	if(pLayerSpaRef && pEnvSpaRef)
	{
		if(!pLayerSpaRef->isSame(pEnvSpaRef))
		{
			OGRCoordinateTransformation *poCT = OGRCreateCoordinateTransformation( pEnvSpaRef, pLayerSpaRef );
			poCT->Transform(1, &Env.MaxX, &Env.MaxY);
			poCT->Transform(1, &Env.MinX, &Env.MinY);
		}
	}
    
	//2. set spatial filter
	pDisplay->StartDrawing(GetCacheID());
	if (m_pRasterRenderer && m_pRasterRenderer->CanRender(m_pwxGISRasterDataset))
	{
		m_pRasterRenderer->Draw(m_pwxGISRasterDataset, DrawPhase, pDisplay, pTrackCancel);
		m_pPreviousDisplayEvn = Env;
	}
	// 5. clear a spatial filter
	pDisplay->FinishDrawing();
}
OGRSpatialReference* wxGISRasteLayer::GetSpatialReference(void)
{
	if(m_pwGISRasterDataset)
		return m_pwxGISRasterDataset->GetSpatialReference();
    return NULL;
}

OGREnvelope* wxGISRasterLayer::GetEnvelope(void)
{
	if(m_pwxGISRasterDataset)
		return m_pwxGISRasterDataset->GetEnvelope();
	return NULL;
}

bool wxGISRasterLayer::IsValid(void)
{
	return m_pwGISRasterDataset == NULL ? false : true;
}