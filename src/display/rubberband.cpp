#include "wxgis/display/rubberband.h"
#include "wx/evtloop.h"

BEGIN_EVENT_TABLE(wxGISRubberBand, wxEvtHandler)
	EVT_KEY_DOWN(wxGISRubberBand::OnKeyDown)
	EVT_LEFT_DOWN(wxGISRubberBand::OnMouseDown)
	EVT_MIDDLE_DOWN(wxGISRubberBand::OnMouseDown)
	EVT_RIGHT_DOWN(wxGISRubberBand::OnMouseDown)
	EVT_LEFT_UP(wxGISRubberBand::OnMouseUp)
	EVT_MIDDLE_UP(wxGISRubberBand::OnMouseUp)
	EVT_RIGHT_UP(wxGISRubberBand::OnMouseUp)
	EVT_LEFT_DCLICK(wxGISRubberBand::OnMouseDoubleClick)
	EVT_MIDDLE_DCLICK(wxGISRubberBand::OnMouseDoubleClick)
	EVT_RIGHT_DCLICK(wxGISRubberBand::OnMouseDoubleClick)
	EVT_MOTION(wxGISRubberBand::OnMouseMove)
	EVT_LEAVE_WINDOW(wxGISRubberBand::OnLeave)
	EVT_ENTER_WINDOW(wxGISRubberBand::OnEnter)
END_EVENT_TABLE()

wxGISRubberBand::wxGISRubberBand(void) : m_pRetGeom(NULL), m_bLock(true)
{

}

wxGISRubberBand::~wxGISRubberBand(void)
{

}

OGRGeometry* wxGISRubberBand::TrackNew(wxCoord x, wxCoord y, wxWindow *pWnd, ICachedDisplay* pCachedDisplay, ISymbol* pSymbol)
{
	m_StartX = x;
	m_StartY = y;
	m_pWnd = pWnd;
	m_pCachedDisplay = pCachedDisplay;
	m_pSymbol = pSymbol;

	wxRect ScrRect = m_pWnd->GetScreenRect();
	wxRect Rect = m_pWnd->GetRect();
	m_StartXScr = ScrRect.GetLeft() + m_StartX - Rect.GetLeft();
	m_StartYScr = ScrRect.GetTop() + m_StartY - Rect.GetTop();

	pWnd->CaptureMouse();
	pWnd->PushEventHandler(this);

	wxEventLoopBase* const loop = wxEventLoop::GetActive();
	while (loop->IsRunning())
	{
		loop->Dispatch();
	    if(!m_bLock) break;
	}

	pWnd->PopEventHandler();
	pWnd->ReleaseMouse();
	return m_pRetGeom;
}

void wxGISRubberBand::OnUnlock(void)
{
	m_bLock = false;
}

void wxGISRubberBand::OnKeyDown(wxKeyEvent & event)
{
	switch(event.GetKeyCode())
	{
	case WXK_ESCAPE:
		{
			wxClientDC CDC(m_pWnd);
			m_pCachedDisplay->OnDraw(CDC);
		}
		OnUnlock();
		break;
	default:
		break;
	}
	event.Skip();
}

void wxGISRubberBand::OnMouseMove(wxMouseEvent& event)
{

}

void wxGISRubberBand::OnMouseDown(wxMouseEvent& event)
{

}

void wxGISRubberBand::OnMouseUp(wxMouseEvent& event)
{

}

void wxGISRubberBand::OnMouseDoubleClick(wxMouseEvent& event)
{

}

void wxGISRubberBand::OnLeave(wxMouseEvent& event)
{

}

void wxGISRubberBand::OnEnter(wxMouseEvent& event)
{

}

//--------------
//class wxGISRubberEnvelope
//-----------------------------

wxGISRubberEnvelope::wxGISRubberEnvelope() : wxGISRubberBand()
{

}

wxGISRubberEnvelope::~wxGISRubberEnvelope()
{

}

void wxGISRubberEnvelope::OnMouseMove(wxMouseEvent& event)
{
	wxClientDC CDC(m_pWnd);
	m_pCachedDisplay->OnDraw(CDC);

	int EvX = event.GetX(), EvY = event.GetY();
	int width, height, X, Y;
	width = abs(EvX - m_StartX);
	height = abs(EvY - m_StartY);
	X = MIN(m_StartX, EvX);
	Y = MIN(m_StartY, EvY);

	//
	CDC.SetPen(m_pSymbol->GetPen());
	CDC.SetBrush(wxBrush(m_pSymbol->GetBrush().GetColour(), wxTRANSPARENT));
	CDC.SetLogicalFunction(wxOR_REVERSE);
	CDC.DrawRectangle(X, Y, width, height);
}

void wxGISRubberEnvelope::OnMouseUp(wxMouseEvent& event)
{
	IDisplayTransformation* pDT = m_pCachedDisplay->GetDisplayTransformation();
	wxPoint Points[2];
	Points[0] = wxPoint(m_StartX, m_StartY);
	Points[1] = wxPoint(event.GetX(), event.GetY());
	OGRRawPoint* pOGRPoints = pDT->TransformCoordDC2World(Points, 2);
	OGRLineString* pLine = new OGRLineString();
	pLine->setPoints(2, pOGRPoints);
	m_pRetGeom = static_cast<OGRGeometry*>(pLine);
    delete [] pOGRPoints;
	OnUnlock();
}