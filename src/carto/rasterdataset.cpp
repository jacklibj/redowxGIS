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
	}
}