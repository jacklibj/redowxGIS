/******************************************************************************
 * Project:  wxGIS (GIS Catalog)
 * Purpose:  wxGISTabView class.
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
#include "wxgis/catalogui/gxview.h"
#include "wxgis/catalog/catalog.h"

#include <wx/aui/aui.h>
#include <wx/artprov.h>
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

//-----------
// wxGxTab
//------------
class wxGxTab :
	public wxPanel,
	public IGxSelectionEvents
{
public:

	wxGxTab(wxGxApplication* application, IGxCatalog* Catalog, wxXmlNode* pTabDesc, wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);
	virtual ~wxGxTab(void);
	virtual wxString GetName(void);
	virtual wxWindow* GetWindow(int iIndex);
	virtual wxWindow* GetCurrentWindow(void) {return m_pCurrentWnd;};
	virtual size_t GetCount(void) {return m_pWindows.size();};
	virtual bool Show(bool bShow);

	virtual void OnSelectionChanged(IGxSelection* Selection, long nInitiator);

	void OnChoice(wxCommandEvent& event);

private:
	std::vector<wxWindow*> m_pWindows;
	wxString m_sName;

	wxPanel* m_tabwnd;
	wxPanel* m_tabselector;
	wxStaticText* m_staticText;
	wxChoice* m_choice;
	wxBoxSizer* m_bSizerMain;

	bool m_bShowChoices;
	wxWindow* m_pCurrentWnd;

	IGxSelection* m_pSelection;

DECLARE_EVENT_TABLE()
};


//---------
// wxGxTabView
//-------------
class WXDLLIMPEXP_GIS_CLU wxGxTabView :
	public wxAuiNotebook,
	public wxGxView,
	public IGxSelectionEvents
{
public:
	wxGxTabView(wxWindow* parent, wxWindowID id = TABCTRLID, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	virtual ~wxGxTabView(void);
	virtual wxWindow* GetCurrentWnd(void);
	//IGxView
	virtual bool Activate(wxGxApplication* application, IGxCatalog* Catalog, wxXmlNode* pConf);
	virtual void Deactivate(void);
	//IGxSelectionEvents
	virtual void OnSelectionChanged(IGxSelection* Selection, long nInitiator);
	//Events
	void OnAUINotebookPageChanged(wxAuiNotebookEvent& event);
protected:
	std::vector<wxGxTab*> m_Tabs;
	IConnectionPointContainer *m_pConnectionPointSelection;
	long m_ConnectionPointSelectionCookie;
	IGxSelection* m_pSelection;

	DECLARE_EVENT_TABLE()
};