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
