#pragma once

#include "wxgis/carto/carto.h"

//wxGISRasterRGBRender
class  wxGISRasterRGBRenderer :
	public IRasterRenderer
{
public:
	wxGISRasterRGBRenderer(void);
    ~wxGISRasterRGBRenderer(void);
	//IRasterRenderer
	virtual bool CanRender(wxGISDataset* pDataset);
	virtual void Draw(wxGISDataset* pRasterDataset, wxGISEnumDrawPhase DrawPhase, IDisplay* pDisplay, ITrackCancel* pTrackCancel);
	//
	virtual OGREnvelope TransformEnvelope(OGREnvelope* pEnvelope, OGRSpatialReference* pSrsSpatialReference,
		OGRSpatialReference* pDstSpatialReference);
	virtual wxImage Scale(unsigned char* pData, int nOrigX, int nOrigY, double rOrigX, double rOrigY,
		int nDestX, int nDestY, double rDeltaX, double rDeltaY, wxGISEnumDrawQuality Quality, ITrackCancel* pTrackCancel);
};
