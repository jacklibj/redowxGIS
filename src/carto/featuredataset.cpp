#include "wxgis/carto/featuredataset.h"
#include <wx/filename.h>

wxGISFeatureDataset::wxGISFeatureDataset(wxString sPath) : wxGISDataset(sPath), m_poDS(NULL), m_bIsOpened(false), m_psExtent(NULL),
	m_poLayer(NULL), m_pQuadTree(NULL)
{

}

wxGISFeatureDataset::~wxGISFeatureDataset(void)
{
	wxDELETE(m_psExtent);

	Empty();

	if(m_pQuadTree)
		CPLQuadTreeDestroy(m_pQuadTree);

	if( m_poDS )
		OGRDataSource::DestroyDataSource( m_poDS );
}

OGRLayer* wxGISFeatureDataset::GetLayer(int iLayer)
{
	if (m_bIsOpened)
	{
		m_poLayer->ResetReading();
		return m_poLayer;
	}
	else
	{
		if(Open(iLayer))
			return GetLayer(iLayer);
		else
			return NULL;
	}
	return NULL;
}

bool wxGISFeatureDataset::Open(int iLayer)
{
	if(m_bIsOpened)
		return true;

	wxCriticalSectionLocker locker(m_CritSect);

	m_poDS = OGRSFDriverRegistrar::Open( wgWX2MB(m_sPath.c_str()), FALSE );
	if ( m_poDS == NULL)
	{
		const char* err = CPLGetLastErrorMsg();
		wxString sErr = wxString::Format(_("wxGISFeatureDataset: Open failed! Path '%s'. OGR error : %s"), m_sPath.c_str(), wgMB2WX(err));
	    wxLogError(sErr);
		wxMessageBox(sErr, _("Error"), wxOK | wxICON_ERROR);
		return false;
	}

	m_poLayer = m_poDS->GetLayer(iLayer);
	if (m_poLayer)
	{
		m_psExtent = new OGREnvelope();
		if (m_poLayer->GetExtent(m_psExtent, true) != OGRERR_NONE)
		{
			wxDELETE(m_psExtent);
			m_psExtent = NULL;
		}
		bool bOLCFastSpatialFilter = m_poLayer->TestCapability(OLCFastSpatialFilter);
		if (!bOLCFastSpatialFilter)
		{
			if (m_psExtent)
			{
				OGREnvelope Env = *m_psExtent;
				CPLRectObj Rect = {Env.MinX, Env.MinY, Env.MaxX, Env.MaxY};
				m_pQuadTree = CPLQuadTreeCreate(&Rect, GetFeatureBoundFunc)
			}
		//
		//
		//
		}

	//
	//

	m_OGRFeatureArray.reserve(m_poLayer->GetFeatureCount(true));

	size_t count(0);
	OGRFeature *poFeature;
	while ( (count < CACHE_SIZE) && ((poFeature = m_poLayer->GetNextFeature()) != NULL)
	{
		AddFeature(poFeature);
		count++;
	}

	//
	//
	//
	}

	m_bIsOpened = true;
	return true;
}

OGRSpatialReference* wxGISFeatureDataset::GetSpatialReference(void)
{
	if(!m_bIsOpened)
		if(!Open(0)£©
			return NULL;
	if( m_poLayer )
		return m_poLayer->GetSpatialRef();
	return NULL;
}

OGREnvelope* wxGISFeatureDataset::GetEnvelope(void)
{
	if(m_psExtent)
		return m_psExtent;
	return NULL;
}
void wxGISFeatureDataset::SetSpatialFilter(double dfMinX, double dfMinY, double dfMaxX, double dfMaxY)
{
	if(!m_bIsOpened)
		if( !Open(0))
			return;
	if ( m_poLayer )
	{
		m_poLayer->SetSpatialFilterRect(dfMinX, dfMinY, dfMaxX, dfMaxY);
	}
}

void wxGISFeatureDataset::Empty(void)
{
	for (size_t i = 0; i < m_OGRFeatureArray.size(); i++)
	     OGRFeature::DestroyFeature( m_OGRFeatureArray[i] );
	m_OGRFeatureArray.clear();

}

void wxGISFeatureDataset::AddFeature(OGRFeature* poFeature)
{
	if(m_pQuadTree)
		CPLQuadTreeInsert(m_pQuadTree, poFeature);
	m_OGRFeatureArray.push_back(poFeature);
}

OGRFeature* wxGISFeatureDataset::GetAt(int nIndex)
{
	wxASSERT(nIndex >= 0);
	//
	while(nIndex + 1 > m_OGRFeatureArray.size())
	{
		size_t count(0);
		OGRFeature *poFeature;
		while( (count < CACHE_SZIE) && (poFeature = m_poLayer->GetNextFeature()) != NULL)
		{
			AddFeature(poFeature);
			count++;
		}
	}
	return m_OGRFeatureArray[nIndex];
}

OGRFeature* wxGISFeatureDataset::operator [](int nIndex)
{
	return GetAt(nIndex);
}

wxString wxGISFeatureDataset::GetAsString(int row, int col)
{
	if(m_poLayer->GetFeature() <= row)
		return wxString();

}
