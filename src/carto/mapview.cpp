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
