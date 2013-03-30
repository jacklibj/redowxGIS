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
	void m_splitter10nIdle( wxIdleEvent& )
	{
		m_splitter1->SetSashPosition( 150 );
		m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxGISToolBarPanel::m_split10nIdle), NULL, this );
	}
	//events
	void OnListboxSelect(wxCommandEvent& event);
	void OnDoubleClickSash(wxSplitterEvent& event);
	void OnCheckboxToggle(wxCommandEvent& event);
	void OnListctrlActivated(wxListEvent& event);
	void OnListctrlRClick(wxListEvent& event);
	void SetKeyCode(wxCommandEvent& event);
	void OnUpdateUI(wxUpdateUIEvent& event);
	void OnCreateCommandBar(wxCommandEvent& event);
	void OnDeleteCommandBar(wxCommandEvent& event);
	void OnAddButton(wxCommandEvent& event);
	void OnRemoveButton(wxCommandEvent& event);
	void OnMoveUp(wxCommandEvent& event);
	void OnMoveDown(wxCommandEvent& event);

	void OnSetKeyCode(int pos);
	long GetSelectedCommandItem(void);
	void LoadCommands(void);

	DECLARE_EVENT_TABLE()

};

//////////////
/////// Class wxGISCommandPanel
/////////////////////////
class wxGISCommandPanel : public wxPanel
{
	enum
	{
		ID_LSTBX = wxID_HIGHEST + 25,
		ID_LSTCTRL,
		ID_ONSETKEYCODE
	};
public:
	typedef std::map<wxString, COMMANDARRAY*> CATEGORYMAP;

private:
	CATEGORYMAP m_CategoryMap;

protected:
	wxSplitterWindow* m_splitter2;
	wxListBox* m_listBox1;
	wxListCtrl* m_listCtrl3;
	wxGxApplication* m_pGxApp;
	wxImageList m_ImageList;
	wxMenu* m_pContextMenu;
	int m_CurSelection;

public:
	wxGISCommandPanel( wxGxApplication* pGxApp, wxWindow* parent,wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 540, 400), long style = wxTAB_TRAVERSAL );
	~wxGISCommandPanel();

	void m_splitter2OnIdle( wxIdleEvent& )
	{
	m_splitter2->SetSashPosition( 150 );
	m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxGISToolBarPanel::m_splitter2OnIdle ), NULL, this );
	}
	//events
	void OnListboxSelect(wxCommandEvent& event);
	void OnDoubleClickSash(wxSplitterEvent& event);
	void OnListctrlActivated(wxListEvent& event);
	void OnListctrlRClick(wxListEvent& event);
	void OnSetKeyCode(wxCommandEvent& event);

	void OnSetKeyCode(int pos);

    DECLARE_EVENT_TABLE()	

};

class wxGISCustomizeDlg : public wxDialog
{
private:

protected:
	wxAuiNotebook* m_auinotebook;
	wxStdDialogButtonSizer* m_sdbSizer;
	wxButton* m_sdbSizerOK;

	wxGxApplication* m_pGxApp;

public:
	wxGISCustomizeDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("CUstomize"),
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 540, 400), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
	~wxGISCustomizeDlg();

};