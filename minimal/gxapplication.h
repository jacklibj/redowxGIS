#pragma once
#include "catalog.h"
#include "application.h"
#include "gxtreeview.h"
#include "gxtabview.h"

#include "wx\aui\aui.h"
#include "wx/artprov.h"

//------
//wxGxApplication
//------------------

class WXDLLIMPEXP_GIS_CLU wxGxApplication :
	public wxGISApplication
{
	wxGxApplication(IGISConfig* pConfig, wxWindow* parent, wxWindowID id, const wxString& title,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);
	virtual ~wxGxApplication(void);


	virtual void SerailizeFramePos(bool bSave = false);
	virtual wxAuiManager*  GetAuiManger(void) {return & m_mgr;};
	virtual void ShowPane(wxWindow* pWnd, bool bShow = true);
	virtual bool ShowPane(const wxString& sName, bool bShow = true);
	virtual bool IsPaneShown(const wxString& sName);
	//wxGISApplication
	virtual void RemoveCommandBar(IGISCommandBar* pBar);
	virtual bool AddCommandBar(IGISCommandBar* pBar);
	//IApplication
	virtual void Customize(void);
	virtual void ShowStatusBar(bool bShow);
	virtual void ShowApplicationWindow(wxWindow* pWnd, bool bShow = true);
	virtual WINDOWARRAY* GetChildWindows(void);
	virtual void RegisterChildWindow(wxWindow* pWnd);
protected:
	wxAuiManager m_mgr;
	wxGxTreeView* m_pTreeview;
	wxGxTabView* m_pTabView;
	IGxCatalog* m_pCatalog;
	WINDOWARRAY m_WindowArray;

};


