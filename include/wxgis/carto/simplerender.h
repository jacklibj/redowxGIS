#pragma once

#include <cstdlib>
#include "wxgis/carto/carto.h"

class wxGISSimpleRenderer :
	public IFeatureRenderer
{
public:
	wxGISSimpleRenderer(void);
	~wxGISSimpleRenderer(void);
//IFeatureRenderer
    virtual bool CanRender(wxGISDataset* pDataset);
	virtual void Draw(wxGISFeatureSet* pSet, wxGISEnumDrawPhase DrawPhase, IDisplay* pDisplay, ITrackCancel* pTrackCancel);
	virtual void DrawGeometry(OGRGeometry *pGeometry, IDisplay* pDisplay);
protected:
	ISymbol* m_pFillSymbol;
	ISymbol* m_pLineSymbol;
	ISymbol* m_pMakerSymbol;
};