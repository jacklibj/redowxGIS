#pragma once

#include "carto.h"
#include "rasterdataset.h"

class WXDLLIMPEXP_GIS_CRT wxGISRasterLayer :
	public wxGISLayer
{
public:
	wxGISRasterLayer(wxGISDataset* pwxGISDataset);
	virtual ~wxGISRasterLayer(void);
//IwxGISLayer
	virtual void Draw(wxGISEnumDrawPhase DrawPhase, ICachedDisplay* pDisplay, ITrackCancel* pTrackCancel);
	virtual OGRSpatialReference* GetSpatialReference(void);
	virtual OGREnvelope* GetEvelope(void);
	virtual bool IsValid(void);
//wxGISRasterLayer
	virtual IRasterRenderer* GetRenderer(void){return m_pRasterRenderer;};
	virtual void SetRenderer(IRasterRenderer* pRasterRenderer){m_pRasterRenderer = pRasterRenderer;};
protected:
	wxGISRasterDataset* m_pwxGISRasterDataset;
	IRasterRenderer* m_pRasterRenderer;
	OGREnvelope m_pPreviousDisplayEvn;
};