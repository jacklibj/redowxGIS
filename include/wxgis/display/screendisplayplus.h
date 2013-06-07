#pragma once

#include "wxgis/display/screendisplay.h"

#if wxUSE_GRAPHICS_CONTEXT

//----
// wxGISScreenDisplay
//-----

class WXDLLIMPEXP_GIS_DSP wxGISScreenDisplayPlus :
	public wxGISScreenDisplay
{
public:
	//wxGISScreenPlusDisplay
	wxGISScreenDisplayPlus(void);
	virtual ~wxGISScreenDisplayPlus(void);
	//IDisplay
	virtual void DrawPolygon(int n, wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0 , int fill_style = wxODDEVEN_RULE);
	virtual void DrawPolyPolygon(int n, int count[], wxPoint points[], wxCoord xoffset , wxCoord yoffset );
	virtual void DrawPoint(wxCoord x, wxCoord y);
	virtual void DrawLines(int n, wxPoint points[], wxCoord xoffset , wxCoord yoffset );
	virtual void DrawCircle(wxCoord x, wxCoord y, wxCoord radius);
	virtual void DrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
	virtual void DrawBitmap(const wxBitmap& bitmap, wxCoord x, wxCoord y, bool transparent );
};

#endif