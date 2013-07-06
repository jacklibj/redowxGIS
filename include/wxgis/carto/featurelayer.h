/******************************************************************************
 * Project:  wxGIS (GIS Catalog)
 * Purpose:  FeatureLayer header.
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
#include "wxgis/carto/featuredataset.h"

class WXDLLIMPEXP_GIS_CRT wxGISFeatureLayer :
	public wxGISLayer
{
public:
	wxGISFeatureLayer(wxGISDataset* pwxGISDataset);
	virtual ~wxGISFeatureLayer(void);
	//
	virtual void Draw(wxGISEnumDrawPhase DrawPhase, ICachedDisplay* pDisplay, ITrackCancel* pTrackCancel);
	virtual OGRSpatialReference* GetSpatialReference(void);
	virtual OGREnvelope* GetEnvelope(void);
	virtual bool IsValid(void);
//wxGISFeatureLayer
	virtual IFeatureRenderer* GetRenderer(void){return m_pFeatureRenderer;};
	virtual void SetRenderer(IFeatureRenderer* pFeatureRenderer){m_pFeatureRenderer = pFeatureRenderer;};
protected:
	wxGISFeatureDataset* m_pwxGISFeatureDataset;
	IFeatureRenderer* m_pFeatureRenderer;
	OGREnvelope m_pPreviousDisplayEnv;
};