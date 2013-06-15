#pragma once

#include "wxgis/carto/map.h"
#include "wxgis/framework/animation.h"

#include "wx/scrolbar.h"
#include "wx/tipwin.h"

#define  TIMER_ID 1013

class wxGISMapView;

//wxDrawingThread
class wxDrawingThread : public wxThread
{
public:
	//
	wxDrawingThread(wxGISMapView* pView, std::vector<wxGISLayer*>& Layers);
	virtual void *Entry();
	virtual void OnExit();
private:
	ITrackCancel* m_pTrackCancel;
	ICachedDisplay* m_pGISScreenDisplay;
	std::vector<wxGISLayer*>& m_Layers;
	wxGISMapView* m_pView;
};

class WXDLLIMPEXP_GIS_CRT wxGISMapView;

//ExtentStack
class WXDLLIMPEXP_GIS_CRT ExtentStack
{
public:
	ExtentStack(wxGISMapView* pView);
	virtual bool CanRedo();
	virtual bool CanUndo();
	virtual void Do(OGREnvelope NewEnv);
	virtual void Redo();
	virtual void Undo();
	virtual void Reset();
	virtual size_t GetSize();
protected:
	virtual void SetExtent(OGREnvelope Env);
protected:
	wxGISMapView* m_pView;
	std::vector<OGREnvelope> m_EnvelopeArray;
	int m_Pos;
};

//wxGISMapView
class WXDLLIMPEXP_GIS_CRT wxGISMapView :
	public wxScrolledWindow,
	public wxGISMap
{
public:
	friend class ExtentStack;
public:
	wxGISMapView(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = wxSTATIC_BORDER|wxTAB_TRAVERSAL);
	virtual ~wxGISMapView(void);
	virtual void OnDraw(wxDC& dc);
	virtual void PanStart(wxPoint MouseLocation);
	virtual void PanMoveTo(wxPoint MouseLocation);
	virtual void PanStop(wxPoint MouseLocation);
	//events
	void OnEraseBackGround(wxEraseEvent & event);
	void OnSize(wxSizeEvent & event);
	void OnKeyDown(wxKeyEvent & event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnTimer(wxTimerEvent & event);
	//wxGISMap
	virtual void AddLayer(wxGISLayer* pLayer);
	virtual void ClearLayers(void);
	//
	void OnThreadExit(void);
	virtual ICachedDisplay* GetCachedDisplay(void){return pGISScreenDisplay;};
	virtual ITrackCancel* GetTrackCancel(void){return m_pTrackCancel;};
	virtual void SetTrackCancel(ITrackCancel* pTrackCancel);
	virtual void SetFullExtent(void);
	virtual void SetExtent(OGREnvelope Env);
	virtual ExtentStack* GetExtentStack(void){return m_pExtentStack;};
protected:
	ICachedDisplay* pGISScreenDisplay;
	WXWORD m_MouseState;
	WXWORD m_MapToolState;;
	ITrackCancel* m_pTrackCancel;
	wxDrawingThread* m_pThread;
	wxGISAnimation* m_pAni;
	wxCriticalSection m_CriticalSection;
	wxTimer m_timer;
	wxRect m_virtualrc;
	OGREnvelope m_virtualbounds;
	wxPoint m_StartMouseLocation;
	bool m_bZoomIn;
	ExtentStack* m_pExtentStack;

	DECLARE_EVENT_TABLE()
};