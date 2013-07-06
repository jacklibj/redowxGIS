/******************************************************************************
 * Project:  wxGIS (GIS Catalog)
 * Purpose:  RasterDataset class.
 * Author:   Bishop (aka Barishnikov Dmitriy), polimax@mail.ru
 ******************************************************************************
*   Copyright (C) 2009  Bishop
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
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
			wxLogDebug( wxT("       %s"), sFileName.c_str() );
		}
	}
	CSLDestroy( papszFileList );

	CPLSetConfigOption( "USE_RRD", "YES" );
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
	double adfGeoTransform[6];
	if(m_poDataset->GetGeoTransform( adfGeoTransform ) != CE_Fatal )
	{
		double inX[4];
		double inY[4];

		inX[0] = 0;
		inY[0] = 0;
		inX[1] = nXSize;
		inY[1] = 0;
		inX[2] = nXSize;
		inY[2] = nYSize;
		inX[3] = 0;
		inY[3] = nYSize;

		m_psExtent->MaxX = 0;
		m_psExtent->MaxY = 0;
		m_psExtent->MinX = 1000000000;
		m_psExtent->MinY = 1000000000;
		for(int i = 0; i < 4; i++)
		{
			double rX, rY;
			GDALApplyGeoTransform( adfGeoTransform, inX[i], inY[i], &rX, &rY);
			if(m_psExtent->MaxX < rX)
				m_psExtent->MaxX = rX;
			if(m_psExtent->MinX > rX)
				m_psExtent->MinX = rX;
			if(m_psExtent->MaxY < rY)
				m_psExtent->MaxY = rY;
			if(m_psExtent->MinY > rY)
				m_psExtent->MinY = rY;
		}
	}
	else
	{
		wxDELETE(m_psExtent);
		m_psExtent = NULL;
	}
	
	//

	m_bIsOpened = true;
	return true;
}

OGRSpatialReference* wxGISRasterDataset::GetSpatialReference(void)
{
	if(!m_bIsOpened)
		if(!Open())
			return NULL;
	if(m_pSpaRef)
		return m_pSpaRef;
	if ( m_poDataset )
	{
		m_pSpaRef = new OGRSpatialReference(m_poDataset->GetProjectionRef());
		return m_pSpaRef;
	}
	return NULL;
}

OGREnvelope* wxGISRasterDataset::GetEnvelope(void)
{
	if(m_psExtent)
		return m_psExtent;
	return NULL;
}

//
