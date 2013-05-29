#include "wxgis/carto/rasterdataset.h"
#include "gdal_rat.h"

wxGISRasterDataset::wxGISRasterDataset(wxString sPath) : wxGISDataset(sPath), m_bIsOpened(false), m_pSpaRef(NULL), m_psExtent(NULL)
{
}

wxGISRasterDataset::~wxGISRasterDataset(void)
{
	OSRDestroySpatialReference( m_pSpaRef );
	wxDELETE(m_psExtent);

	if(m_bIsOpened)
		GDALClose(m_poDataset);
}

bool wxGISRasterDataset::Open(void)
{
	if(m_bIsOpened)
		return true;

	wxCriticalSectionLocker locker(m_CritSect);


	m_poDataset = (GDALDataset *) GDALOpen( wgWX2MB(m_sPath.c_str()), GA_ReadOnly );
	if( m_poDataset == NULL )
		return false;

	int nXSize = m_poDataset->GetRasterXSize();
	int nYSize = m_poDataset->GetRasterYSize();

	bool bHasOverviews = false;


	char** papszFileList = m_poDataset->GetFileList();
	if ( CSLCount(papszFileList) == 0 )
	{
		wxLogDebug(wxT( "Files: none associated" ));
	}
	else
	{
		wxLogDebug(wxT("Files: %s"), wgMB2WX(papszFileList[0]) );
		for (int i = 1; papszFileList[i] != NULL; i++)
		{
			wxString sFileName = wgMB2WX(papszFileList[i]);
			if(sFileName.Find(wxT(".rrd")) != wxNOT_FOUND || sFileName.Find(wxT(".ovr")) != wxNOT_FOUND)
				bHasOverviews = true;
			wxLogDebug( wxT("        %s"),sFileName.c_str() );
		}
	}
	CSLDestroy( papszFileList );

	CPLSetConfigOption( "USE_RRD", "YES"");
	CPLSetConfigOption( "HFA_USE_RRD", "YES" );
	CPLSetConfigOption( "COMPRESS_OVERVIEW", "LZW" );

	bool bAskCreateOvr = false; 
	if(!bHasOverviews && bAskCreateOvr)
	{
		int anOverviewList[5] = { 4, 8, 16, 32, 64 };
		CPLErr err = m_poDataset->BuildOverviews( "CUBIC", 5, anOverviewList, 0, NULL, GDALDummyProgress, NULL );
		//
	}

	//

	m_psExtent = new OGREnvelope();
}