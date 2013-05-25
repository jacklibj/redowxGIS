#pragma once
#include "wxgis/display/display.h"


class WXDLLIMPEXP_GIS_DSP wxSimpleLineSymbol :
	public ISymbol
{
public:
	wxSimpleLineSymbol(void);
	wxSimpleLineSymbol(wxPen Pen);
	virtual ~wxSimpleLineSymbol(void);
	virtual void DrawLine(OGRLineString* pLine, IDisplay* pwxGISDisplay);
	virtual void DrawPolyLine(OGRMultiLineString* pLine, IDisplay* pwxGISDisplay);
	//
	virtual void Draw(OGRGeometry* pGeometry, IDisplay* pwxGISDisplay);
};