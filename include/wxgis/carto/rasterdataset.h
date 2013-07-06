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
#pragma once

#include "wxgis/carto/carto.h"

//--------------
// wxGISRasterDataset
//-----------

class WXDLLIMPEXP_GIS_CRT wxGISRasterDataset :
	public wxGISDataset
{
public:
	wxGISRasterDataset(wxString sPath);
	virtual ~wxGISRasterDataset(void);
	virtual wxGISEnumDatasetType GetType(void){return enumGISRasterDataset;};
	virtual bool Open(void);
	virtual OGRSpatialReference* GetSpatialReference(void);
	virtual OGREnvelope* GetEnvelope(void);
	virtual GDALDataset* GetRaster(void){return m_poDataset;};
//

protected:
	OGREnvelope* m_psExtent;
	GDALDataset *m_poDataset;
	OGRSpatialReference* m_pSpaRef;
	bool m_bIsOpened;
};