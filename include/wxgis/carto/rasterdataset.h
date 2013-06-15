#pragma once

#include "wxgis/carto/carto.h"

//--------------
// wxGISRasterDataset
//-----------

class WXDLLIMPEXP_GIS_CRT wxGISRasterDataset :
	public wxGISDataset
{
public:
	wxGISRasterDataset(wxString sPath);
	virtual ~wxGISRasterDataset(void);
	virtual wxGISEnumDatasetType GetType(void){return enumGISRasterDataset;};
	virtual bool Open(void);
	virtual OGRSpatialReference* GetSpatialReference(void);
	virtual OGREnvelope* GetEnvelope(void);
	virtual GDALDataset* GetRaster(void){return m_poDataset;};
//

protected:
	OGREnvelope* m_psExtent;
	GDALDataset *m_poDataset;
	OGRSpatialReference* m_pSpaRef;
	bool m_bIsOpened;
};