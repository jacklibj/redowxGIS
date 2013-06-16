#pragma once

#include "wxgis/display/display.h"
class WXDLLIMPEXP_GIS_DSP wxGISRubberBand :
	public wxEvtHandler
{
public:
	wxGISRubberBand(void);
	virtual ~wxGISRubberBand(void);
	virtual OGRGeometry* TrackNew(wxCoord x, wxCoord y, wxWindow *pWnd, ICachedDisplay* pCachedDisplay, ISymbol* pSymbol);
	virtual void OnUnlock(void);
    //events
	virtual void OnKeyDown(wxKeyEvent & event);
	virtual void OnMouseMove(wxMouseEvent& event);
	virtual void OnMouseDown(wxMouseEvent& event);
	virtual void OnMouseUp(wxMouseEvent& event);
	virtual void OnMouseDoubleClick(wxMouseEvent& event);
	virtual void OnLeave(wxMouseEvent& event);
	virtual void OnEnter(wxMouseEvent& event);
protected:
	OGRGeometry* m_pRetGeom;
	bool m_bLock;
	wxCoord m_StartX;
	wxCoord m_StartY;
	wxCoord m_StartXScr;
	wxCoord m_StartYScr;
	wxWindow* m_pWnd;
	ICachedDisplay* m_pCachedDisplay;
	ISymbol* m_pSymbol;

	DECLARE_EVENT_TABLE()
};

//--------
// class wxGISRubberEnvelope
//---------

class WXDLLIMPEXP_GIS_DSP wxGISRubberEnvelope :
	public wxGISRubberBand
{
public:
	wxGISRubberEnvelope(void);
	virtual ~wxGISRubberEnvelope(void);
	virtual void OnMouseMove(wxMouseEvent& event);
	virtual void OnMouseUp(wxMouseEvent& event);
};