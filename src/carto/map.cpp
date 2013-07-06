/******************************************************************************
 * Project:  wxGIS (GIS Catalog)
 * Purpose:  wxGISMap class.
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
#include "wxgis/carto/map.h"


wxGISMap::wxGISMap(void)
{
	m_sMapName = wxString(_("new map"));
}

wxGISMap::~wxGISMap(void)
{
	//
	ClearLayers();
}

//
void wxGISMap::AddLayer(wxGISLayer* pLayer)
{
	if(!pLayer)
		return;
	m_Layers.push_back(pLayer);
}

void wxGISMap::ClearLayers(void)
{
	for (size_t i = 0 ; i < m_Layers.size(); i++ )
		delete m_Layers[i];
	m_Layers.clear();
}

OGREnvelope wxGISMap::GetFullExtent(void)
{
	OGREnvelope res;
	for(size_t i = 0; i < m_Layers.size(); i++)
		//
		res.Merge(*m_Layers[i]->GetEnvelope());
	return res;
}
