#pragma once

#include "wxgis/carto/carto.h"
#include "wxgis/carto/featuredataset.h"

class WXDLLIMPEXP_GIS_CRT wxGISFeatureLayer :
	public wxGISLayer
{
public:
	wxGISFeatureLayer(wxGISDataset* pwxGISDataset);
	virtual ~wxGISFeatureLayer(void);
	//
	virtual void Draw(wxGISEnumDrawPhase DrawPhase, ICachedDisplay* pDisplay, ITrackCancel* pTrackCancel);
	virtual OGRSpatialReference* GetSpatialReference(void);
	virtual OGREnvelope* GetEnvelope(void);
	virtual bool IsValid(void);
//wxGISFeatureLayer
	virtual IFeatureRenderer* GetRender(void){return m_pFeatureRenderer;}
	virtual void SetRenderer(IFeatureRenderer* pFeatureRenderer){m_pFeatureRenderer = pFeatureRenderer;};
protected:
	wxGISFeatureDataset* m_pwxGISFeatureDataset;
	IFeatureRenderer* m_pFeatureRenderer;
	OGREnvelope m_pPreviousDisplayEnv;
};