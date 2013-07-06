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
#pragma once

#include "wxgis/framework/framework.h"

#define ATIMER_ID 1012


class WXDLLIMPEXP_GIS_FRW wxGISAnimation :
	public wxControl,
	public IProgressor
{
public:
	wxGISAnimation(wxWindow * parent, wxWindowID id = wxID_ANY,
		const wxBitmap & bitmap = wxNullBitmap, const int bitmap_size = 16,
		const wxPoint & pos = wxDefaultPosition,
		const wxSize & size = wxDefaultSize,
		long style = wxNO_BORDER,
		const wxString name = wxT("Ani"));
	virtual ~wxGISAnimation(void);
	void OnPaint(wxPaintEvent & event);
	void OnEraseBackground(wxEraseEvent & event);
	void OnTimer( wxTimerEvent & event);



	virtual void Play(void) { m_timer.Start(50); };
	virtual void Stop(void) { m_timer.Stop(); };

	//IProgressor
	virtual bool Show(bool bShow) { return wxControl::Show(bShow); };
protected:
	wxImageList m_ImageList;
	int m_nImgPos;
	wxTimer m_timer;

	DECLARE_EVENT_TABLE()
};

