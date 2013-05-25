#pragma once
#include "wxgis/display/display.h"


class WXDLLIMPEXP_GIS_DSP wxSimpleMarkerSymbol :
	public ISymbol
{
public:
	wxSimpleMarkerSymbol(void);
	virtual ~wxSimpleMarkerSymbol(void);
	//
	virtual void Draw(OGRGeometry* pGeometry, IDisplay* pwxGISDisplay);
protected:
	wxBrush m_Brush;
	wxPen m_Pen;
	wxCoord m_Size;
};