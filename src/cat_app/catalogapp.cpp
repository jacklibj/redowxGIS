﻿#include "wxgis/catalog/catalog.h"
#include "wxgis/cat_app/catalogapp.h"
#include "wxgis/cat_app/catalogframe.h"

#include <locale.h>
#include "ogrsf_frmts.h"
#include "ogr_api.h"

IMPLEMENT_APP(wxGISCatalogApp);

wxGISCatalogApp::wxGISCatalogApp(void) : m_pszOldLocale(NULL)
{
}

wxGISCatalogApp::~wxGISCatalogApp(void)
{
	//wxLogMessage(_("wxGISCatalogApp is exiting..."));

	OGRCleanupAll();
	GDALDestroyDriverManager();

	if(m_pszOldLocale != NULL)
		setlocale(LC_NUMERIC, m_pszOldLocale);
	wxDELETE(m_pszOldLocale);

	wxDELETE(m_pConfig);
}

bool wxGISCatalogApp::OnInit()
{
	m_pConfig = new wxGISAppConfig(APP_NAME, CONFIG_DIR);
	//setup loging
	wxString sLogDir = m_pConfig->GetLogDir();
	if(sLogDir.IsEmpty())
	{
		wxLogError(_("wxGISCatalogApp: Filed to get log dir"));
        return false;
	}

	if(!wxDirExists(sLogDir))
		wxFileName::Mkdir(sLogDir, 0777, wxPATH_MKDIR_FULL);

	wxDateTime dt(wxDateTime::Now());
	wxString logfilename = sLogDir + wxFileName::GetPathSeparator() + wxString::Format(wxT("log_%.4d%.2d%.2d.log"), dt.GetYear(), dt.GetMonth() + 1, dt.GetDay());

	if(!m_LogFile.Open(logfilename.GetData(), wxT("a+")))
		wxLogError(_("wxGISCatalogApp: Filed to open log file %s"), logfilename.c_str());

	wxLog::SetActiveTarget(new wxLogStderr(m_LogFile.fp()));

	wxLogMessage(wxT(" "));
	wxLogMessage(wxT("####################################################################")); 
	wxLogMessage(wxT("##                    %s                    ##"),wxNow().c_str()); 
	wxLogMessage(wxT("####################################################################")); 
	//wxLogMessage(_("HOST '%s': OS desc - %s, free memory - %u Mb"), wxGetFullHostName().c_str(),wxGetOsDescription().c_str(), wxGetFreeMemory()/1048576);
	wxLogMessage(_("HOST '%s': OS desc - %s, free memory -  Mb"), wxGetFullHostName().c_str(),wxGetOsDescription().c_str() );


	wxLogMessage(_("wxGISCatalogApp: Initializing..."));
	wxLogMessage(_("wxGISCatalogApp: Log file: %s"), logfilename.c_str());
	wxLogMessage(_("wxGISCatalogApp: Initialize locale"));

	wxString sLocale = m_pConfig->GetLocale();
	//init locale
	if(!sLocale.IsEmpty() )
	{
		int iLocale(0);
		const wxLanguageInfo* loc_info = wxLocale::FindLanguageInfo(sLocale);
		if(loc_info != NULL)
		{
			iLocale = loc_info->Language;
			wxLogMessage(_("wxGISCatalogApp: Language set to %s"), loc_info->Description.c_str());
		}

        // don't use wxLOCALE_LOAD_DEFAULT flag so that Init() doesn't return
        // false just because it failed to load wxstd catalog
       // if ( !m_locale.Init(iLocale, wxLOCALE_CONV_ENCODING) )
		if ( !m_locale.Init(iLocale) )
        {
            wxLogError(wxT("wxGISCatalogApp: This language is not supported by the system."));
            return false;
        }
    }


	//m_locale.Init(wxLANGUAGE_DEFAUTL);

	//normally this wouldn't be necessary as the catalog files would be found
	//in the default locations, but the program is not installed the
	//catalogs are in the build directory where we wouldn't find them by
	//default

	wxString sLocaleDir = m_pConfig->GetLocaleDir();
	wxString sLocalePath = sLocaleDir + wxFileName::GetPathSeparator() + sLocale;
	if(wxDirExists(sLocalePath))
	{
		wxLocale::AddCatalogLookupPathPrefix(sLocalePath);

		//Initialize the catalogs we'll be using
		//load muticat from locale
		wxArrayString trans_arr;
		wxDir::GetAllFiles(sLocalePath, &trans_arr, wxT("*_cat.mo"));

		for(size_t i = 0; i< trans_arr.size(); i++ )
		{
			wxFileName name(trans_arr[i]);
			m_locale.AddCatalog(name.GetName());

			//m_locale.AddCatalog(wxT("wxMonClient"));
		}

		//this catalog is installed in standard location on linux systems and 
		//shows that you may make use of the standard message catalogs as well
		//
		//if it's not installed on your system, it is just silently ignored 
#ifdef __LINUX__
		{
			wxLogNull noLog;
			m_locale.AddCatalog(_T("fileutils"));
		}
#endif
	}

	//support of dot in doubles and floats
	m_pszOldLocale = strdup(setlocale(LC_NUMERIC, NULL));
	if( setlocale(LC_NUMERIC, "C") == NULL )
		m_pszOldLocale = NULL;

	//setup sys dir
	wxString sSysDir = m_pConfig->GetSysDir(); //´í³öÔÚÕâ
	if(!wxDirExists(sSysDir))
	{
		wxLogError(wxString::Format(_T("wxGISCatalogApp: System dir is absent! Lookup path '%s'"), sSysDir.c_str()));
		return false;
	}

	bool bDebugMode = m_pConfig->GetDebugMode();

	CPLSetConfigOption("CPL_DEBUG", bDebugMode == true ? "ON" : "OFF");
	CPLSetConfigOption("CPL_TIMESTAMP", "ON");
	CPLSetConfigOption("CPL_LOG_ERRORS", bDebugMode == true ? "ON" : "OFF");
	
	CPLSetConfigOption("GDAL_DATA", wgWX2MB( (sSysDir + wxFileName::GetPathSeparator() + wxString(wxT("epsg_csv")) + wxFileName::GetPathSeparator()).c_str() ) );
	wxString sCPLLogPath = sLogDir + wxFileName::GetPathSeparator() + wxString(wxT("gdal_log_cat.txt"));
	
	CPLSetConfigOption("CPL_LOG", wgWX2MB(sCPLLogPath.c_str()) );

	OGRRegisterAll();
	GDALAllRegister();

	wxLogMessage(_("wxGISCatalogApp: Start main frame"));


	//store values
	m_pConfig->SetLogDir(sLogDir);
	m_pConfig->SetLocale(sLocale);
	m_pConfig->SetLocaleDir(sLocaleDir);
	m_pConfig->SetSysDir(sSysDir);
	m_pConfig->SetDebugMode(bDebugMode);

	wxFrame* frame = new wxGISCatalogFrame(m_pConfig, NULL, wxID_ANY, _("wxGIS Catalog"), wxDefaultPosition, wxSize(800, 480) );
   // SetTopWindow(frame);
    //frame->Show();

	// call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if ( !wxApp::OnInit() )
		return false;

	//MyFrame *frame = new MyFrame("Minimal wxWidgets App");

	// and show it (the frames, unlike simple controls, are not shown when
	// created initially)
	frame->Show(true);

	// success: wxApp::OnRun() will be called which will enter the main message
	// loop and the application will run. If we returned false here, the
	// application would exit immediately.

	return true;
}


