/******************************************************************************
 * Project:  wxGIS (GIS Catalog)
 * Purpose:  wxGISAnimation class.
 * Author:   Bishop (aka Barishnikov Dmitriy), polimax@mail.ru
 ******************************************************************************
*   Copyright (C) 2009  Bishop
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include "wxgis/framework/animation.h"

BEGIN_EVENT_TABLE(wxGISAnimation, wxControl)
	EVT_PAINT(wxGISAnimation::OnPaint)
	EVT_ERASE_BACKGROUND(wxGISAnimation::OnEraseBackground)
	EVT_TIMER( ATIMER_ID, wxGISAnimation::OnTimer )
END_EVENT_TABLE()

wxGISAnimation::wxGISAnimation(wxWindow * parent, wxWindowID id, const wxBitmap & bitmap, const int bitmap_size, const wxPoint & pos, const wxSize & size, long style, const wxString name) : wxControl(parent, id, pos, size, style, wxDefaultValidator, name), m_timer(this, ATIMER_ID)
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
	wxRect rc = GetClientRect();
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