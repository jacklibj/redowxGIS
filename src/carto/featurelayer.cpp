#include "wxgis/carto/featurelayer.h"
#include "wxgis/carto/simplerender.h"

wxGISFeatureLayer::wxGISFeatureLayer(wxGISDataset* pwxGISDataset) : wxGISLayer(), m_pwxGISFeatureDataset(NULL), m_pFeatureRenderer(NULL)
{
	m_pwxGISFeatureDataset = dynamic_cast<wxGISFeatureDataset*>(pwxGISDataset);
	if(m_pwxGISFeatureDataset)
	{
		m_pwxGISFeatureDataset->Reference();
		m_pFeatureRenderer = new wxGISSimpleRenderer();
		//pre load feature
	}
}

wxGISFeatureLayer::~wxGISFeatureLayer(void)
{
	wxDELETE(m_pFeatureRenderer);
	wsDELETE(m_pwxGISFeatureDataset);
}

void wxGISFeatureLayer::Draw(wxGISEnumDrawPhase DrawPhase, ICachedDisplay* pDisplay, ITrackCancel* pTrackCancel)
{
	IDisplayTransformation* pDisplayTransformation = pDisplay->GetDisplayTransformation();
	if (!pDisplayTransformation)
	     return;
	//1. get  envelope
	OGREnvelope Env = pDisplayTransformation->GetVisibleBounds();
	OGREnvelope* LayerEnv = m_pwxGISFeatureDataset->GetEnvelope();
	bool bSetFilter(false);
	if(m_pPreviousDisplayEnv.MaxX - Env.MaxX > DELTA || m_pPreviousDisplayEnv.MaxY - Env.MaxY > DELTA || m_pPreviousDisplayEnv.MinX - Env.MinX > DELTA 
		|| m_pPreviousDisplayEnv.MinY - Env.MinY > DELTA)
	bSetFilter = LayerEnv->Contains(Env); //!Env.Contains(*LayerEnv);
	OGRSpatialReference* pEnvSpaRef = pDisplayTransformation->GetSpatialReference();
	OGRSpatialReference* pLayerSpaRef = m_pwxGISFeatureDataset->GetSpatialReference();

	if (pLayerSpaRef && pEnvSpaRef)
	{
		if (!pLayerSpaRef->IsSame(pEnvSpaRef))
		{
			OGRCoordinateTransformation *poCT = OGRCreateCoordinateTransformation( pEnvSpaRef, pLayerSpaRef );
			poCT->Transform(1, &Env.MaxX, &Env.MaxY);
			poCT->Transform(1, &Env.MinX, &Env.MinY);
			OCTDestroyCoordinateTransformation(poCT);
		}
	}

	//2. set spatial filter
	pDisplay->StartDrawing(GetCacheID());
	if (m_pFeatureRenderer && m_pFeatureRenderer->CanRender(m_pwxGISFeatureDataset))
	{
		wxGISFeatureSet* pGISFeatureSet(NULL);
		//3. get features set
		if (bSetFilter)
		{
			wxGISSpatialFilter pFilter;
			pFilter.SetEnvelope(Env);
			pGISFeatureSet = m_pwxGISFeatureDataset->GetFeatureSet(&pFilter, pTrackCancel);
			//
			wxLogDebug(wxT("Attention! returned %u features"), pGISFeatureSet->GetSize());
		}
		else
			pGISFeatureSet = m_pwxGISFeatureDataset->GetFeatureSet(NULL, pTrackCancel);
		//4. send it to renderer
		m_pFeatureRenderer->Draw(pGISFeatureSet, DrawPhase, pDisplay, pTrackCancel);
		wxDELETE(pGISFeatureSet);
		m_pPreviousDisplayEnv = Env;
	}
	//5. clear a spatial filter
	pDisplay->FinishDrawing();
}

OGRSpatialReference* wxGISFeatureLayer::GetSpatialReference(void)
{
	if(m_pwxGISFeatureDataset)
		return m_pwxGISFeatureDataset->GetSpatialReference();
	return NULL;
}

OGREnvelope* wxGISFeatureLayer::GetEnvelop(void)
{
	if(m_pwxGISFeatureDataset)
		return m_pwxGISFeatureDataset->GetEnvelope();
	return NULL;
}

bool wxGISFeatureLayer::IsValid(void)
{
	return m_pwxGISFeatureDataset == NULL ? false : true;
}

	