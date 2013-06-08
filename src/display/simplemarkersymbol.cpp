#include "wxgis/display/simplemarkersymbol.h"

wxSimpleMarkerSymbol::wxSimpleMarkerSymbol(void)
{
	m_Brush.SetColour(168, 0, 132);
	m_Brush.SetStyle(wxSOLID);

	m_Pen.SetColour(128, 128, 128);
	m_Pen.SetCap(
		wxCAP_ROUND);
	m_Pen.SetStyle(wxSOLID);
	m_Pen.SetWidth(1)

		m_size
		 = 4;
}