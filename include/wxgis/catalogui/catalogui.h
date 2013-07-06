/******************************************************************************
 * Project:  wxGIS (GIS Catalog)
 * Purpose:  wxCatalogUI main header.
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

#include "wxgis/base.h"

#define TREECTRLID  1005 //wxGxCatalog tree
#define LISTCTRLID  1006 //wxGxCatalog content view
#define TABLECTRLID 1007 //wxGxCatalog table view
#define TABCTRLID   1008 //wxGxCatalog tab with views - contents, preview & etc.
#define MAPCTRLID   1009 //wxGxCatalog map view

class IGxViewsFactory
{
public:
	virtual ~IGxViewsFactory(void){};
	virtual wxWindow* CreateView(wxString sName, wxWindow* parent) = 0;

};