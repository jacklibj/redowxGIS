#include "wxgis/carto/mapview.h"
//
#include "wxgis/display/screendisplayplus.h"

#define WAITTIME 650

//------
// wxDrawingThread
//--------

wxDrawingThread::wxDrawingThread(wxGISMapView* pView, std::vector<wxGISLayer*>& Layers) : wxThread(), m_pView(pView), m_Layers(Layers)
{
	m_pTrackCancel = m_pView->GetTrackCancel();
	m_pGISScreenDisplay = m_pView->GetCachedDisplay();

	if(m_pTrackCancel)
		m_pTrackCancel->Reset();
}

void *wxDrawingThread::Entry()
{
	wxClientDC CDC(m_pView);

	for (size_t i = 0; i < m_Layers.size(); i++)
	{
		if (m_pTrackCancel && !m_pTrackCancel->Continue())
	         break;
		if (m_pGISScreenDisplay->IsCacheDerty(m_Layers[i]->GetCacheID()) && m_Layers[i]->GetVisible())
		{
			size_t CacheIDCurrent, CacheIDPrevious;
			if (i == 0)
		         CacheIDPrevious = 0;
			else
				CacheIDPrevious = m_Layers[i - 1]->GetCacheID();

			CacheIDCurrent = m_Layers[i]->GetCacheID();

			if (CacheIDCurrent != CacheIDPrevious)
			{
				m_pGISScreenDisplay->MergeCaches(CacheIDPrevious, CacheIDCurrent);
				m_pGISScreenDisplay->SetCacheDerty(CacheIDCurrent, false);
			}

			if(m_pTrackCancel && !m_pTrackCancel->Continue())
				break;
			m_Layers[i]->Draw(wxGISDPGeography, m_pGISScreenDisplay, m_pTrackCancel);
		}
	}
	m_pGISScreenDisplay->SetDerty(false);
	m_pGISScreenDisplay->OnDraw(CDC);

	return NULL;
}

void wxDrawingThread::OnExit()
{
	m_pView->OnThreadExit();
}

//-----------------------------------------------
//ExtenStack
//-----------------------------------------------
ExtenStack::ExtenStack(wxGISMapView* pView)
{
	m_pView = pView;
	m_Pos = -1;
}

bool ExtenStack::CanRedo()
{
	if(m_EnvelopeArray.empty())
		return false;
	return m_Pos < m_EnvelopeArray.size() - 1;
}

bool ExtenStack::CanUndo()
{
	if (m_EnvelopeArray.empty())
        return false;
	return m_Pos > 0;
}

void ExtenStack::Do(OGREnvelope NewEnv)
{
	m_Pos++;
	if(m_Pos == m_EnvelopeArray.size())
		m_EnvelopeArray.push_back(NewEnv);
	else
	{
		m_EnvelopeArray[m_Pos] = NewEnv;
		m_EnvelopeArray.erase(m_EnvelopeArray.begin() + m_Pos + 1, m_EnvelopeArray.end());
	}

	SetExtent(NewEnv);
}

void ExtenStack::Redo()
{
	m_Pos++;
	if(m_Pos < m_EnvelopeArray.size())
	{
		OGREnvelope Env = m_EnvelopeArray[m_Pos];
		SetExtent(Env);
	}
}

void ExtenStack::Undo()
{
	m_Pos--;
	if(m_Pos > -1)
	{
		OGREnvelope Env = m_EnvelopeArray[m_Pos];
		SetExtent(Env);
	}
}

void ExtenStack::SetExtent(OGREnvelope Env)
{
	m_pView->GetTrackCancel()->Cancel();
	if(m_pView->m_pThread)
		m_pView->m_pThread->Delete();
	IDisplayTransformation* pDisplayTransformation = m_pView->pGISScreenDisplay->GetDisplayTransformation();
	pDisplayTransformation->SetBounds(Env);
	m_pView->pGISScreenDisplay->SetDerty(true);
	m_pView->Refresh(false);
}

void ExtenStack::Reset()
{
	m_EnvelopeArray.clear();
	m_Pos = -1;
}

size_t ExtenStack::GetSize()
{
	return m_EnvelopeArray.size();
}

//-------
// wxGISMapView
//-------

BEGIN_EVENT_TABLE(wxGISMapView, wxScrolledWindow)
	EVT_ERASE_BACKGROUND(wxGISMapView::OnEraseBackground)
	EVT_SIZE(wxGISMapView::OnSize)
	EVT_MOUSEWHEEL(wxGISMapView::OnMouseWheel)
	EVT_KEY_DOWN(wxGISMapView::OnKeyDown)
	EVT_TIMER( TIMER_ID, wxGISMapView::OnTimer )
END_EVENT_TABLE()

wxGISMapView::wxGISMapView(wxWindow* parent, wxWindowID id, const wxPoint& pos , const wxSize& size , long style ) : wxScrolledWindow(parent, id, pos, size, style | wxHSCROLL | wxVSCROLL), wxGISMap(), m_pTrackCancel(NULL), m_pThread(NULL), m_pAni(NULL), m_timer(this, TIMER_ID)
{
	//set map init envelope

#if wxUSE_GRAPHICS_CONTEXT
	pGISScreenDisplay = new wxGISScreenDisplayPlus();
#else
	pGISScreenDisplay = new wxGISScreenDisplay();
#endif
	m_pExtenStack = new ExtenStack(this);
	
	m_pTrackCancel = new ITrackCancel();
	m_pTrackCancel->Reset();

	IDisplayTransformation* pDisplayTransformation = pGISScreenDisplay->GetDisplayTransformation();
	pDisplayTransformation->SetDeviceFrame(GetClientRect());
	wxClientDC CDC(this);
	pDisplayTransformation->SetPPI(CDC.GetPPI());

	m_MouseState = enumGISMouseNone;
	m_MapToolState = enumGISMapNone;

}

wxGISMapView::~wxGISMapView(void)
{
	wxDELETE(pGISScreenDisplay);
	wxDELETE(m_pExtenStack);
	wxDELETE(m_pTrackCancel);
	if(m_pThread)
		m_pThread->Delete();
}

void wxGISMapView::OnDraw(wxDC& dc)
{
	if (m_pExtenStack->GetSize() == 0)
	{
		IDisplayTransformation* pDisplayTransformation = pGISScreenDisplay->GetDisplayTransformation();
		m_pExtenStack->Do(pDisplayTransformation->GetBounds());
	}

	if(m_pTrackCancel && !m_pAni)
		m_pAni = static_cast<wxGISAnimation*>(m_pTrackCancel->GetProgressor());


	if(pGISScreenDisplay->IsDerty())
	{
		wxCriticalSectionLocker locker(m_CriticalSection);
		if (m_pAni)
		{
			m_pAni->Show(true);
			m_pAni->Play();
		}

		if(m_pThread)
			m_pThread->Delete();

		//start draw thread
		m_pThread = new wxDrawingThread(this, m_Layers);
		if ( !m_pThread || m_pThread->Create() != wxTHREAD_NO_ERROR )
		{
			wxLogError(wxString(_("wxGISMapView: Can't create wxDrawingThread!")));
			return;
		}
		if ( !m_pThread || m_pThread->Run() != wxTHREAD_NO_ERROR )
		{
			wxLogError(wxString(_("wxGISMapView: Can't run wxDrawingThread!")));
			return;
		}

		wxScrolledWindow::SetFocus();

		return;

	}

	if(m_MouseState != enumGISMouseNone)
		return;
	if(m_MapToolState != enumGISMapNone)
		return;
	pGISScreenDisplay->OnDraw(dc);
	//
}

void wxGISMapView::OnSize(wxSizeEvent & event)
{
	m_pTrackCancel->Cancel();
	if(m_pThread)
		m_pThread->Delete();

	wxClientDC CDC(this);
	IDisplayTransformation* pDisplayTransformation = pGISScreenDisplay->GetDisplayTransformation();
	pDisplayTransformation->SetPPI(CDC.GetPPI());

	wxMouseState state = wxGetMouseState();
	if(state.LeftDown())
	{
		wxRect rc = GetClientRect();
		pGISScreenDisplay->OnStretchDraw(CDC, rc.width, rc.height);
		m_MouseState |= enumGISMouseLeftDown;
		m_timer.Start(300);
	}
	else
	{
		m_MouseState &= ~enumGISMouseLeftDown;
		//set map init envelope
		pDisplayTransformation->SetDeviceFrame(GetClientRect());
		pGISScreenDisplay->SetDerty(true);
	}
	Refresh(false);

	event.Skip();
}

void wxGISMapView::OnEraseBackground(wxEraseEvent & event)
{

}
void wxGISMapView::AddLayer(wxGISLayer* pLayer)
{
	IDisplayTransformation* pDisplayTransformation = pGISScreenDisplay->GetDisplayTransformation();
	if (pDisplayTransformation->GetSpatialReference() == NULL)
	{
		OGRSpatialReference* pSpaRef = pLayer->GetSpatialReference();
		if(pSpaRef)
			pDisplayTransformation->SetSpatialReference(pSpaRef);
		else
		{
			OGREnvelope* pEnv = pLayer->GetEnvelope();
			if (pEnv)
			{
				if (pEnv->MaxX <= 180 && pEnv->MaxY <= 90 && pEnv->MinX >= -180 && pEnv->MinY >= -90)
				{
					OGRSpatialReference* pSpaRef = new OGRSpatialReference();
					pSpaRef->SetWellKnownGeogCS("WGS84");
					pDisplayTransformation->SetSpatialReference(pSpaRef);
				}
			}
		}
	}

	OGREnvelope* pEnv = pLayer->GetEnvelope();
	if(pEnv == NULL)
		return;

	OGRSpatialReference* pSpaRef = pLayer->GetSpatialReference();

	if (pSpaRef && pDisplayTransformation->GetSpatialReference())
	{
		if (!pDisplayTransformation->GetSpatialReference()->IsSame(pSpaRef))
		{
			OGRCoordinateTransformation *poCT = OGRCreateCoordinateTransformation( pSpaRef, pDisplayTransformation->GetSpatialReference() );
			poCT->Transform(1, &pEnv->MaxX, &pEnv->MaxY);
			poCT->Transform(1, &pEnv->MinX, &pEnv->MinY);
		}
	}

	if(!pDisplayTransformation->IsBoundsSet())
		pDisplayTransformation->SetBounds(*pEnv);
	else
	{
		OGREnvelope Bounds = pDisplayTransformation->GetBounds();
		Bounds.Merge(*pEnv);
		pDisplayTransformation->SetBounds(Bounds);
	}

	//Caching
	if(pLayer->GetCached())
	{
		pLayer->SetCacheID(pGISScreenDisplay->AddCache());
	}
	else
	{
		if(m_Layers.size() > 0 && m_Layers[m_Layers.size() - 1]->GetCached())
			pLayer->SetCacheID(pGISScreenDisplay->AddCache());
		else
			pLayer->SetCacheID(pGISScreenDisplay->GetLastCacheID());
	}
	wxGISMap::AddLayer(pLayer);
}

void wxGISMapView::ClearLayers(void)
{
	m_pTrackCancel->Cancel();
	if(m_pThread)
		m_pThread->Delete();

	//delete all cashes
	pGISScreenDisplay->ClearCaches();
	//reset spatial reference
	IDisplayTransformation* pDisplayTransformation = pGISScreenDisplay->GetDisplayTransformation();
	pDisplayTransformation->Reset();
	//reset views stack
	m_pExtenStack->Reset();

	wxGISMap::ClearLayers();
}

void wxGISMapView::OnKeyDown(wxKeyEvent & event)
{
	switch(event.GetKeyCode())
	{
	case WXK_ESCAPE:
		m_pTrackCancel->Cancel();
		break;
	default:
		break;
	}
	event.Skip();
}

void wxGISMapView::SetTrackCancel(ITrackCancel* pTrackCancel)
{
	if(pTrackCancel == NULL)
		return;

	if (m_pTrackCancel)
	{
		m_pTrackCancel->Cancel();
		//wait while the thread s exited;
		wxSleep(1);
		delete m_pTrackCancel;
	}
	m_pTrackCancel = pTrackCancel;
}

void wxGISMapView::OnThreadExit(void)
{
	m_pThread = NULL;
	if(m_pAni)
	{
		m_pAni->Stop();
		m_pAni->Hide();
	}
}

void wxGISMapView::OnMouseWheel(wxMouseEvent& event)
{
	event.Skip();

	m_pTrackCancel->Cancel();
	if (m_pThread)
	    m_pThread->Delete();

	wxClientDC CDC(this);

	IDisplayTransformation* pDisplayTransformation = pGISScreenDisplay->GetDisplayTransformation();
	if (pDisplayTransformation)
	{
		if (!(m_MouseState & enumGISMouseWheel))
		{
			m_virtualrc = GetClientRect();
			m_virtualbounds = pDisplayTransformation->GetBounds();

			m_MouseState |= enumGISMouseWheel;
		}
		int direction = event.GetWheelRotation();
		int delta = event.GetWheelDelta();
		int factor = direction / delta;

		double world_zoom = ZOOM_FACTOR * (double)factor;

		//calc zoom dc
		int dx(0), dy(0);
		if (factor < 0)
		{
			dx = ((double)m_virtualrc.width * (world_zoom / (world_zoom - 1))) / -2;
			dy = ((double)m_virtualrc.height * (world_zoom / (world_zoom - 1))) / -2;
		}
		else
		{
			//
			//
			dx = ((double)m_virtualrc.width * (world_zoom)) / 2;
			dy = ((double)m_virtualrc.height * (world_zoom)) / 2;
		}

		m_virtualrc.Inflate(dx, dy);
		if(m_virtualrc.width <= 10 || m_virtualrc.height <= 10)
			return;

		m_virtualrc.CenterIn(GetClientRect());
		wxCoord x = m_virtualrc.x;
		wxCoord y = m_virtualrc.y;

		//calc zoom world
		double dwx(0), dwy(0);

		if (factor < 0)
		{
			dwx = (m_virtualbounds.MaxX - m_virtualbounds.MinX) * (world_zoom) / 2;
			dwy = (m_virtualbounds.MaxY - m_virtualbounds.MinY) * (world_zoom) / 2;
		}
		else
		{
			dwx = (m_virtualbounds.MaxX - m_virtualbounds.MinX) * (world_zoom) / (2 * (1 + world_zoom));
			dwy = (m_virtualbounds.MaxY - m_virtualbounds.MinY) * (world_zoom) / (2 * (1 + world_zoom));
		}

		m_virtualbounds.MaxX -= dwx;
		m_virtualbounds.MinX += dwx;
		m_virtualbounds.MaxY -= dwy;
		m_virtualbounds.MinY += dwy;

		wxClientDC CDC(this);

		if(!m_virtualrc.Contains(GetClientRect()))//
			pGISScreenDisplay->OnStretchDraw(CDC, m_virtualrc.width, m_virtualrc.height, x, y, true, enumGISQualityBicubic);
		else
		{
			wxRect client_rc = GetClientRect();
			wxRect rc = client_rc;
			rc.width = rc.width * rc.width / m_virtualrc.width;
			rc.height = rc.height * rc.height / m_virtualrc.height;
			rc.x = client_rc.x + (client_rc.width - rc.width) / 2;
			rc.y = client_rc.y + (client_rc.height - rc.height) / 2;
			pGISScreenDisplay->OnStretchDraw2(CDC, rc, false, enumGISQualityFourQuadBilinear);
		}

		pDisplayTransformation->SetBounds(m_virtualbounds);
		//draw scale text
		double sc = pDisplayTransformation->GetScaleRatio();
		wxString format_s = NumberScale(sc);
		format_s.Prepend(wxT("1 : "));
        //
		//
		wxSize size = GetClientSize();
		int width, height;

		CDC.SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
		CDC.GetTextExtent(format_s, &width, &height);

		wxBrush br(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK));
		CDC.SetBrush(br);
		CDC.SetPen(*wxBLACK_PEN);
		int x1 = (size.x - width) / 2, y1 = (size.y - height) - 50;
		CDC.DrawRectangle( x1 - 5, y1 - 2, width + 10, height + 4);

		CDC.DrawText(format_s, x1, y1);

		//
		//

		m_timer.Start(WAITTIME);
	}
}

void wxGISMapView::OnTimer( wxTimerEvent& event )
{
	wxMouseState state = wxGetMouseState();
	IDisplayTransformation* pDisplayTransformation = pGISScreenDisplay->GetDisplayTransformation();

	if (!state.LeftDown() && (m_MouseState & enumGISMouseLeftDown))
	{
		m_MouseState &= ~enumGISMouseLeftDown;
		//set map init envelope
		pDisplayTransformation->SetDeviceFrame(GetClientRect());
	}
	else if(m_MouseState & enumGISMouseWheel)
	{
		m_MouseState &= ~enumGISMouseWheel;

		//
		m_timer.Stop();
		m_pExtenStack->Do(m_virtualbounds);
		return;
	}
	else
		return;

	pGISScreenDisplay->SetDerty(true);
	m_timer.Stop();
	Refresh(false);
}

void wxGISMapView::SetFullExtent(void)
{
	SetExtent(GetFullExtent());
}

void wxGISMapView::SetExtent(OGREnvelope Env)
{
	m_pExtenStack->Do(Env);

	//
}

void wxGISMapView::PanStart(wxPoint MouseLocation)
{
	if (m_timer.IsRunning())
	{
		wxTimerEvent ev;
		OnTimer( ev );
#ifdef WAITTIME
		wxMilliSleep(WAITTIME);
#endif
		m_pTrackCancel->Cancel();
		if(m_pThread)
			m_pThread->Delete();
	}
	m_StartMouseLocation = MouseLocation;
	m_MapToolState |= enumGISMapPanning;
	CaptureMouse(); //events 
}

void wxGISMapView::PanMoveTo(wxPoint MouseLocation)
{
	
	if (m_MapToolState & enumGISMapPanning)
	{
		wxCoord x = m_StartMouseLocation.x - MouseLocation.x;
		wxCoord y = m_StartMouseLocation.y - MouseLocation.y;
		wxClientDC CDC(this);
		pGISScreenDisplay->OnPanDraw(CDC, x, y);
	}
}

void wxGISMapView::PanStop(wxPoint MouseLocation)
{

	if (m_MapToolState & enumGISMapPanning)
	{
		m_MapToolState &= ~enumGISMapPanning;
		ReleaseMouse();
		wxCoord x = m_StartMouseLocation.x - MouseLocation.x;
		wxCoord y = m_StartMouseLocation.y - MouseLocation.y;
		//calc new envelope
        IDisplayTransformation* pDisplayTransformation = pGISScreenDisplay->GetDisplayTransformation();
		wxRect rect = pDisplayTransformation->GetDeviceFrame();
		rect.Offset(x, y);
		//
		//
		OGREnvelope Env = pDisplayTransformation->TransformRect(rect);
		m_pExtenStack->Do(Env);
//		pDisplayTransformation->SetBounds(Env);
//		pGISScreenDisplay->SetDerty(true);
//		Refresh(false);
	}
}