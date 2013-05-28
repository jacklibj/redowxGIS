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

//----
// ExtentStack
//------
ExtentStack::ExtentStack(wxGISMapView* pView)
{
	m_pView = pView;
	m_Pos = -1;
}

bool ExtentStack::CanRedo()
{
	if(m_EnvelopArray.empty())
		return false;
	return m_Pos < m_EnvelopArray.size() - 1;
}

bool ExtentStack::CanUndo()
{
	if (m_EnvelopArray.empty())
        return false;
	return m_Pos > 0;
}

void ExtentStack::Do(OGREnvelope NewEnv)
{
	m_Pos++;
	if (m_Pos == m_EnvelopArray.size())
       m_EnvelopArray.push_back(NewEnv);
	else
	{
		m_EnvelopArray[m_Pos] = NewEnv;
		m_EnvelopArray.erase(m_EnvelopArray.begin() + m_Pos + 1, m_EnvelopArray.end());
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

void ExtentStack::Undo
{
	m_Pos--;
	if(m_Pos > -1)
	{
		OGREnvelope Env = m_EnvelopArray[m_Pos];
		SetExtent(Env);
	}
};

void ExtentStack::SetExtent(OGREnvelope Env)
{
	m_pView->GetTrackCancel()->Cancel();
	if(m_pView->m_pThread)
		m_pView->m_pThread->Delete();
	IDisplayTransformation* pDisplayTransformation = m_pView->pGISScreenDisplay->GetDisplayTransformation();
	pDisplayTransformation->SetBounds(Env);
	m_pView->pGISScreenDisplay->SetDerty(true);
	m_pView->Refresh(false);
}

void ExtentStack::Reset()
{
	m_EnvelopArray.clear();
	m_Pos = -1;
}

size_t ExtentStack::GetSize()
{
	return m_EnvelopArray.size();
}

//-------
// wxGISMapView
//-------

BEGIN_EVENT_TABLE(wxGISMapView, wxScrolledWindow)
	EVT_ERASE_BACKGROUND(wxGISMapView::OnEraseBackGround)
	EVT_SIZE(wxGISMapView::OnSize)
	EVT_MOUSEWHEEL(wxGISMapView::OnMouseWheel)
	EVT_KEY_DOWN(wxGISMapView::OnKeyDown)
	EVT_TIMER( TIMER_ID, wxGISMapView::OnTimer )
END_EVENT_TABLE()

wxGISMapView::wxGISMapView(wxWindow* parent, wxWindowID id, const wxPoint& pos , const wxSize& size , long style ) : wxScrolledWindow(parent, id, pos, size, 
style | wxHSCROLL | wxVSCROLL), wxGISMap(),
m_PTrackCancel(NULL), m_pThread(NULL), m_pAni(NULL), m_timer(this, TIMER_ID)
{
	//set map init envelope

#if wxUSE_GRAPHICS_CONTEXT
	pGISScreenDisplay = new wxGISScreenDisplayPlus();
#else
	pGISScreenDisplay = new wxGISScreenDisplay();
#endif
	m_pExtentStack = new ExtentStack(this);

	m_PTrackCancel = new ITrackCancel();
	m_PTrackCancel->Reset();

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
	wxDELETE(m_pExtentStack);
	wxDELETE(m_PTrackCancel);
	if(m_pThread)
		m_pThread->Delete();
}

void wxGISMapView::OnDraw(wxDC& dc)
{
	if (m_pExtentStack->GetSize() == 0)
	{
		IDisplayTransformation* pDisplayTransformation = pGISScreenDisplay->GetDisplayTransformation();
		m_pExtentStack->Do(pDisplayTransformation->GetBounds());
	}

	if(m_PTrackCancel && !m_pAni)
		m_pAni = static_cast<wxGISAnimation*>(m_PTrackCancel->GetProgressor());


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
	m_PTrackCancel->Cancel();
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

void wxGISMapView::AddLayer(wxGISLayer* pLayer)
{
	IDisplayTransformation* pDisplayTransformation = pGISScreenDisplay->GetDisplayTransformation();
	if (pDisplayTransformation->GetSpatialReference() == NULL)
	{
		OGRSpatialReference* pSpaRef = pLayer->GetSpatialReference();
		if(pSpaRef)
			pDisplayTransformation->GetSpatialReference(pSpaRef);
		else
		{
			OGREnvelope* pEnv = pLayer->GetEnvelop();
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

	OGREnvelope* pEnv = pLayer->GetEnvelop();
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
	m_pExtentStack->Reset();

	wxGISMap::ClearLayers();
}

void wxGISmapView::OnKeyDown(wxKeyEvent & event)
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

void wxGISMapView::SetTrackCancel(ITrackCancel* m_PTrackCancel)
{
	if(pTrackCancel == NULL)
		return;

	if (m_PTrackCancel)
	{
		m_PTrackCancel->Cancel();
		//wait while the thread s exited;
		wxSleep(1);
		delete m_PTrackCancel;
	}
	m_PTrackCancel = m_PTrackCancel;
}

void wxGISMapView::OnThreadExit(void)
{
	m_pThread == NULL;
	if (m_pAni)
	{
		m_pAni->Stop();
		m_pAni->Hide();
	}
}

void wxGISMapView::OnMouseWheel(wxMouseEvent& event)
{
	event.Skip();

	m_PTrackCancel->Cancel();
	if (m_pThread)
	    m_pThread->Delete();

	wxClientDC CDC(this);

	IDisplayTransformation* pDisplayTransformation = pGISScreenDisplayTransformation();
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
			dy = ((double)m_virtualrc.height * (world_zoom))
		}
	}
}