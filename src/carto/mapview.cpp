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

void ExtentStack::Redo()
{
	m_Pos++;
	if ()
	{
	}
}