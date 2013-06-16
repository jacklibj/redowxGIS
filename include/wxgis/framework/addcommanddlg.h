#pragma once
//#include "wxgis/framework/framework.h"
#include "wxgis/framework/application.h"

#include "wx\init.h"

#include "wx\gdicmn.h"
#include "wx\aui\auibook.h"
#include "wx\font.h"
#include "wx\colour.h"
#include "wx\settings.h"
#include "wx\string.h"
#include "wx\sizer.h"
#include "wx\button.h"
#include "wx\dialog.h"
#include "wx\splitter.h"
#include "wx\checklst.h"
#include "wx\listctrl.h"
#include "wx\panel.h"
#include "wx\listbox.h"


class WXDLLIMPEXP_GIS_FRW wxGISAddCommandDlg : public wxDialog
{
	enum
	{
		ID_LSTBX = wxID_HIGHEST + 20,
		ID_LSTCTRL,
		ID_ONSETKEYCODE,
		MENUID = wxGISApplication::ID_PLUGINCMD + 1200
	};
public:
	typedef std::map<wxString, COMMANDARRAY*> CATEGORYMAP;
	std::vector<long> m_IDArray;

private:
	CATEGORYMAP m_CategoryMap;

protected:
	wxSplitterWindow* m_splitter2;
	wxListBox* m_listBox1;
	wxListView* m_listCtrl3;
	wxGISApplication* m_pGxApp;
	wxImageList m_ImageList;
	int m_CurSelection;
	wxStdDialogButtonSizer* m_sdbSizer;
	wxButton* m_sdbSizerOK;
	wxButton* m_sdbSizerCancel;

public:
	wxGISAddCommandDlg( wxGISApplication* pGxApp, wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Add command"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 540, 400), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    ~wxGISAddCommandDlg(void);
	void FillIDArray(void);
	void m_splitter2OnIdle( wxIdleEvent& )
	{
		m_splitter2->SetSashPosition( 150 );
		m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxGISAddCommandDlg::m_splitter2OnIdle), NULL, this );
	}
	//
	void OnListboxSelect( wxCommandEvent& event);
	void OnDoubleClickSash( wxSplitterEvent& event);
	void OnListctrlActivated( wxListEvent& event);
	void OnUpdateOKUI(wxUpdateUIEvent& event);
	void OnOk(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};