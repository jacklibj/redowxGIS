#include "wxgis/carto/cartocmd.h"
#include "../../art/geography16.xpm"
#include "wxgis/display/rubberband.h"
#include "wxgis/display/simplefillsymbol.h"

// 0 Full Extent
// 1 Prev Extent
// 2 Next Extent
// 3 ?

//------
// wxGISCartoMainCmd
//-----

IMPLEMENT_DYNAMIC_CLASS(wxGISCartoMainCmd, wxObject)

wxGISCartoMainCmd::wxGISCartoMainCmd(void) : m_pMapView(NULL)
{
	m_ImageList.Create(16, 16);
	m_ImageList.Add(wxBitmap(geography16_xpm));
}

wxGISCartoMainCmd::~wxGISCartoMainCmd(void)
{

}

wxIcon wxGISCartoMainCmd::GetBitmap(void)
{
	switch(m_subtype)
	{
	case 0:
		return m_ImageList.GetIcon(9);
	case 1:
		return m_ImageList.GetIcon(6);
	case 2:
		return m_ImageList.GetIcon(7);
	default:
		return wxIcon();
	}
}

wxString wxGISCartoMainCmd::GetCaption(void)
{

}

