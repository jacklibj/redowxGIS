#program once

#include "wxgis\framework\framework.h"

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

