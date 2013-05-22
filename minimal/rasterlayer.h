#pragma once

#include "carto.h"
#include "rasterdataset.h"

class WXDLLIMPEXP_GIS_CRT wxGISRasterlayer :
	public wxGISLayer
{
public:
	wxGISRasterLayer(wxGISDataset* pwxGISDataset);
	virtual ~wxGISRasterLayer(void);

};