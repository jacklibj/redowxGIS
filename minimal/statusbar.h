#pragma once


#include "animation.h"

#define TIMER_TD 1011

class WXDLLIMPEXP_GIS_FRW wxGISStatusBar :
	public wxStatusBar,
	public IStatusBar
{
public:
	wxGISStatusBar(wxWindow *parent, wxWindowID id, long style = wxST_SIZEGRIP, const wxString& name = wxT("statusBar"), WXWORD panesstyle 
		= enumGISStatusMain | enumGISStatusAnimation | enumGISStatusPosition | enumGISStatusClock);
	virtual ~wxGISStatusBar(void);
	void OnSize(wxSizeEvent & event);
	virtual wxString GetMessage(int i = 0);
	virtual IProgressor* GetAnimation(void)
	{
		if(m_Panes & enumGISStatusAnimation)
			return static_cast<IProgressor*>(m_pAni);
		return NULL;
	};
	virtual IProgressor* GetProgressor(void)
	{
		if(m_Panes & enumGISStatusProgress)
			return NULL;
		return NULL;
	};
	//events
	void OnRightDown(wxMouseEvent& event);
	void OnTimer(wxTimerEvent & event);

protected:
	wxTimer m_timer;
	wxGISAnimation* m_pAni;
	int m_MsgPos, m_AniPos, m_ProgressPos, m_ClockPos;
	//wxGauge   *m_pProgreesorBar;
	IApplication* m_pApp;

	DECLARE_EVENT_TABLE()

};