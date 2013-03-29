#pragma once
#include "gxapplication.h"

#include <wx\intl.h>

#include <wx\gdicmn.h>
#include <wx\aui\auibook.h>
#include <wx\font.h>
#include <wx\colour.h>
#include <wx\settings.h>
#include <wx\string.h>
#include <wx\sizer.h>
#include <wx\button.h>
#include <wx\dialog.h>
#include <wx\splitter.h>
#include <wx\checklst.h>
#include <wx\listctrl.h>
#include <wx\panel.h>
#include <wx\listbox.h>


class wxGISToolBarPanel : public wxPanel
{
	enum
	{
		ID_CHKLSTBX = wxID_HIGHEST + 30,
		ID_BUTTONSLST,
		ID_ONSETKEYCODE,
		ID_CREATECB,
		ID_DELETECB,
		ID_ADDBUTTON,
		ID_REMOVEBUTTON,
		ID_MOVECONTROLUP,
		ID_MOVECONTROLDOWN
	};
private:
	COMMANDBARARRAY m_CategoryArray;
protected:
	wxSplitterWindow* m_splitter1;
	wxCheckListBox* m_commandbarlist;
	wxListView* m_buttonslist;
	wxButton* m_createbutton;
	wxButton* m_deletebutton;
	wxButton* m_addbutton;
	wxButton* m_rembutton;
	wxButton* m_moveup;
	wxButton* m_movedown;
	wxGxApplication* m_pGxApp;
	wxImageList m_ImageList;
	bool m_bToolsFocus, m_bCmdFocus;
	wxMenu* m_pContextmenu;
	int m_nContextMenuPos, m_nMenubarPos, m_nToolbarPos;

public:
	wxGISToolBarPanel(wxGxApplication* pGxApp, wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxSize( 540,400 ), long style = wxTAB_TRAVERSAL );
	~wxGISToolBarPanel();
	void m_split10nIdle( wxIdleEvent& )
	{

	}

}