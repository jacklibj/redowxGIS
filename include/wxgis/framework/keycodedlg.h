#pragma once

#include "wxgis/framework/framework.h"
#include "../art/charmap16.xpm"

#include "wx\intl.h"

#include "wx\string.h"
#include "wx\textctrl.h"
#include "wx\gdicmn.h"
#include "wx\font.h"
#include "wx\colour.h"
#include "wx\settings.h"
#include "wx\button.h"
#include "wx\sizer.h"
#include "wx\dialog.h"
#include "wx\valtext.h"

/////

///
///Class wxKeyCodeCtrl
/////

class wxKeyCodeCtrl : public wxTextCtrl
{
public:
	wxKeyCodeCtrl(wxWindow* parent, wxWindowID id, const wxString& value = wxT(""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxTextCtrlNameStr);
    virtual ~wxKeyCodeCtrl(void);
	//event
	void OnChar(wxKeyEvent& event);

	int m_Flags;
	int m_Key;

	DECLARE_EVENT_TABLE()
};

/////
///Class wxKeyCodeDlg
////
class WXDLLIMPEXP_GIS_FRW wxKeyCodeDlg :public wxDialog
{
private:

protected:
	wxKeyCodeCtrl* m_textCtrl1;
	wxButton* m_button;

public:
	wxKeyCodeDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Enter key code"), const wxPoint& pos = wxPoint(-1, -1), const wxSize& size = wxSize( 260, 70), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
	~wxKeyCodeDlg();
	static wxBitmap GetBitmap(){return wxBitmap(charmap16_xpm);};
	//events
	void OnUpdateUI(wxUpdateUIEvent& event);
	virtual bool TransferDataFromWindow();

	int m_Flags;
	int m_Key;
	wxString m_sKeyCode;

	DECLARE_EVENT_TABLE()
};