#include "animation.h"

BEGIN_EVENT_TABLE(wxGISAnimation, wxControl)
	EVT_PAINT(wxGISAnimation::OnPaint)
	EVT_ERASE_BACKGROUND(wxGISAnimation::OnEraseBackground)
	EVT_TIMER( ATIMER_ID, wxGISAnimation::OnTimer )
END_EVENT_TABLE()

wxGISAnimation::wxGISAnimation(wxWindow * parent, wxWindowID id, const wxBitmap & bitmap, const int bitmap_size, const wxPoint & pos, const wxSize & size, long style, const wxString name)
{
	m_nImgPos = 0;
	m_ImageList.Create(bitmap_size, bitmap_size);
	m_ImageList.Add(bitmap);

	//Play()

	//
    //
	//
	//
}

wxGISAnimation::~wxGISAnimation()
{

}
void wxGISAnimation::OnPaint(wxPaintEvent & event)
{
	wxPaintDC dc(this);
	int x(0), y(0);
	m_ImageList.GetSize(0, x, y);
	wxRect rect;
	wxRect rc = GetClientSize();
	x = (rc.width - x) / 2;
	y = (rc.height - y) / 2;
	m_ImageList.Draw(m_nImgPos, dc, x, y, wxIMAGELIST_DRAW_NORMAL, true);
	m_nImgPos++;
	if(m_ImageList.GetImageCount() <= m_nImgPos)
		m_nImgPos = 0;
}

void wxGISAnimation::OnEraseBackground(wxEraseEvent & event)
{

}

void wxGISAnimation::OnTimer( wxTimerEvent& event)
{
	Refresh();
}