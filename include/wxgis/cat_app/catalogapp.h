/******************************************************************************
 * Project:  wxGIS (GIS Catalog)
 * Purpose:  Main application class.
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
#include "wxgis/framework/config.h"


#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/ffile.h>
#include <wx/datetime.h>
#include <wx/intl.h>





class wxGISCatalogApp : 
	public wxApp
{
public:
    // override base class virtuals
    // ----------------------------
	 wxGISCatalogApp(void);
	 ~wxGISCatalogApp(void);

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();

private:
	wxLocale m_locale; // locale we'll be using
	char* m_pszOldLocale;
	wxFFile m_LogFile;
	wxGISAppConfig* m_pConfig;

};

DECLARE_APP(wxGISCatalogApp);
