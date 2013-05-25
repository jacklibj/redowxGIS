#pragma once
#include "wxgis/display/display.h"


class WXDLLIMPEXP_DATA_GIS_DSP wxSimpleFillSymbol :
	public ISymbol
{
public:
	wxSimpleFillSymbol(void);
	wxSimpleFillSymbol(wxPen, wxBrush Brush);
	virtual ~wxSimpleFillSymbol(void);
	virtual void DrawPolygon(OGRPolygon* pPoly, IDisplay* pwxGISDisplay);
	virtual void DrawPolyPolygon(OGRMultiPolygon* pPoly, IDisplay* pwxGISDisplay);
	//ISymbol
	virtual void Draw(OGRGeometry* pGeometry, IDisplay* pwxGISDisplay);
};