/*
#include "wxgis/catalog/catalog.h"
#include "wxgis/cat_app/catalogapp.h"
#include "wxgis/cat_app/catalogframe.h"
*/

#include "catalogapp.h"
#include <locale.h>
#include "ogrsf_frmts.h"
#include "ogr_api.h"
#include "gdal_priv.h"

IMPLEMENT_APP(wxGISCatalogApp)

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);

    // event handlers (these functions should _not_ be virtual)
    void OnHello(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

private:
    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
	Minimal_Hello = 1,
    Minimal_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(Minimal_Hello, MyFrame::OnHello)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)


// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool wxGISCatalogApp::OnInit()
{
	m_pConfig = new wxGISAppConfig(APP_NAME, CONFIG_DIR);
	//setup loging
	wxString sLogDir = m_pConfig->GetLogDir();
	if(sLogDir.IsEmpty())
	{
		wxLogError(_("wxGISCatalogApp: Failed to log dir"));
	}

	if(!wxDirExists(sLogDir))
		wxFileName::Mkdir(sLogDir, 0777, wxPATH_MKDIR_FULL);

	wxDateTime dt(wxDateTime::Now());
	wxString logfilename = sLogDir + wxFileName::GetPathSeparator() + wxString::Format(wxT("log_%.4d%.2d%.2d.log"), dt.GetYear(), dt.GetMonth() + 1, dt.GetDay());

	if(!m_LogFile.Open(logfilename.GetData(), wxT("a+")))
		wxLogError(_("wxGISCatalogApp: Failed to open log file %s"), logfilename.c_str());

	wxLog::SetActiveTarget(new wxLogStderr(m_LogFile.fp()));

	wxLogMessage(wxT(" "));
	wxLogMessage(wxT("####################################################################")); 
	wxLogMessage(wxT("##                    %s                    ##"),wxNow().c_str()); 
	wxLogMessage(wxT("####################################################################")); 
    wxLogMessage(wxT("HOST '%s': OS desc - %s, free memory -%u Mb"), wxGetFullHostName().c_str(),wxGetOsDescription().c_str(), wxGetFreeMemory()/1048576);


	wxLogMessage(_("wxGISCatalogApp: Initializeing..."));
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

		//don't use wxLOCALE_LOAD_DEFAULT flag so that Init() deesn't return
		//false just becasue it failed tp load wxstd catalog
		if( !m_locale.Init(iLocale, wxLOCALE_CONV_ENCODING) )
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
	wxString sSysDir = m_pConfig->GetSysDir();
	if(!wxDirExists(sSysDir))
	{
		wxLogError(wxString::Format(_T("wxGISaCatalogApp: System dir is absent! Lookup path '%s'"), sSysDir.c_str()));
		return false;
	}

	bool bDebugMode = m_pConfig->GetDebugMode();

	CPLSetConfigOption("CPL_DEBUG", bDebugMode == true ? "ON" : "OFF");
	CPLSetConfigOption("CPL_TIMESTAMP", "ON");
	CPLSetConfigOption("CPL_LOG_ERRORS", bDebugMode == true ? "ON" : "OFF");
	//liyang
	//CPLSetConfigOption("GDAL_DATA", wgWX2MB( (sSysDir + wxFileName::GetPathSeparator() + wxString(wxT("epsg_csv")) + wxFileName::GetPathSeparator()).c_str() ) );
	wxString sCPLLogPath = sLogDir + wxFileName::GetPathSeparator() + wxString(wxT("gdal_log_cat.txt"));
	//liyang
	//CPLSetConfigOption("CPL_LOG", wgWX2MB(sCPLLogPath.c_str()) );

	OGRRegisterAll();
	GDALAllRegister();

	wxLogMessage(_("wxGISCatalogApp: Start main frame"));


	//store values
	m_pConfig->SetLogDir(sLogDir);
	m_pConfig->SetLocale(sLocale);
	m_pConfig->SetLocaleDir(sLocaleDir);
	m_pConfig->SetSysDir(sSysDir);
	m_pConfig->SetDebugMode(bDebugMode);


	// need add wxGISCatalogFrame class
	//wxFrame* frame = new wxGISCatalogFrame(m_pConfig, NULL, wxID_ANY, _("wxGIS Catalog"), wxDefaultPosition, wxSize(800, 480) );
	//SetTopWindow(frame);
	//frame->Show();

    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if ( !wxApp::OnInit() )
        return false;

    MyFrame *frame = new MyFrame("Minimal wxWidgets App");

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.

    return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)
{
    // set the frame icon
    SetIcon(wxICON(sample));

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");

	//fileMenu->Append(Minimal_Hello,"&Hello...\tCtrl-H",
	//	"Help String shown in status bar for this menu item");

	fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText("Welcome to wxWidgets!");
#endif // wxUSE_STATUSBAR
}


// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
                 (
                    "Welcome to %s!\n"
                    "\n"
                    "This is the minimal wxWidgets sample\n"
                    "running under %s.",
                    wxVERSION_STRING,
                    wxGetOsDescription()
                 ),
                 "About wxWidgets minimal sample",
                 wxOK | wxICON_INFORMATION,
                 this);
}

void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}