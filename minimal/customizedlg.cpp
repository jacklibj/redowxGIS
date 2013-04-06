#include "customizedlg.h"
#include "keycodedlg.h"
#include "createdbdlg.h"
#include "addcommanddlg.h"
#include "wx\accel.h"
#include "wx\utils.h"
/////////

/////////////////
// Class wxGISToolBarpanel
/////
BEGIN_EVENT_TABLE(wxGISToolBarPanel, wxPanel)
	EVT_LISTBOX(wxGISToolBarPanel::ID_CHKLSTBX, wxGISToolBarPanel::OnListboxSelect)
	EVT_SPLITTER_DCLICK(wxID_ANY, wxGISToolBarPanel::OnDoubleClickSash)
	EVT_CHECKLISTBOX(wxGISToolBarPanel::ID_CHKLSTBX, wxGISToolBarPanel::OnCheckboxToggle)
	EVT_LIST_ITEM_RIGHT_CLICK(wxGISToolBarPanel::ID_BUTTONSLST, wxGISToolBarPanel::OnListctrlRClick)
	EVT_LIST_ITEM_ACTIVATED(wxGISToolBarPanel::ID_BUTTONSLST, wxGISToolBarPanel::OnListctrlActivated)
	EVT_MENU(wxGISToolBarPanel::ID_ONSETKEYCODE, wxGISToolBarPanel::OnSetKeyCode)
	EVT_BUTTON(wxGISToolBarPanel::ID_CREATECB, wxGISToolBarPanel::OnCreateCommandBar)
	EVT_BUTTON(wxGISToolBarPanel::ID_DELETECB, wxGISToolBarPanel::OnDeleteCommandBar)
	EVT_BUTTON(wxGISToolBarPanel::ID_ADDBUTTON, wxGISToolBarPanel::OnAddButton)
	EVT_BUTTON(wxGISToolBarPanel::ID_REMOVEBUTTON, wxGISToolBarPanel::OnRemoveButton)
	EVT_BUTTON(wxGISToolBarPanel::ID_MOVECONTROLUP, wxGISToolBarPanel::OnMoveUp)
	EVT_BUTTON(wxGISToolBarPanel::ID_MOVECONTROLDOWN, wxGISToolBarPanel::OnMoveDown)
    EVT_UPDATE_UI_RANGE(wxGISToolBarPanel::ID_CREATECB, wxGISToolBarPanel::ID_MOVECONTROLDOWN, wxGISToolBarPanel::OnUpdateUI)
END_EVENT_TABLE()

wxGISToolBarPanel::wxGISToolBarPanel(wxGxApplication* pGxApp, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style), m_bToolsFocus(false), m_bCmdFocus(false)
{
	m_pGxApp = pGxApp;
	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxHORIZONTAL );

	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxNO_BORDER);
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( wxGISToolBarPanel::m_splitter1OnIdle), NULL, this );
	bSizer->Add( m_splitter1, 1, wxEXPAND, 5 );

	wxArrayString m_commandbarlistChoices;
	m_commandbarlist = new wxCheckListBox( m_splitter1, wxGISToolBarPanel::ID_CHKLSTBX, wxDefaultPosition, wxDefaultSize, m_commandbarlistChoices, wxNO_BORDER);
	//

	m_buttonslist = new wxListView( m_splitter1, wxGISToolBarPanel::ID_BUTTONSLST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL|wxNO_BORDER);
	//
	m_buttonslist->InsertColumn(0, _("Command Name"), wxLIST_FORMAT_LEFT, 90);
	m_buttonslist->InsertColumn(1, _("Description"), wxLIST_FORMAT_LEFT, 120);
	m_buttonslist->InsertColumn(2, _("KeyCode"), wxLIST_FORMAT_LEFT, 60);
	m_ImageList.Create(16,16);
	m_buttonslist->SetImageList(&m_ImageList, wxIMAGE_LIST_SMALL);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	
	m_createbutton = new wxButton( this, wxGISToolBarPanel::ID_CREATECB, _("Create toolbar/menu"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add( m_createbutton, 0, wxALL| wxEXPAND, 5);

	m_deletebutton = new wxButton( this, wxGISToolBarPanel::ID_DELETECB, _("Delete toolbar/menu"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add( m_deletebutton, 0, wxALL| wxEXPAND, 5);

	m_addbutton = new wxButton( this, wxGISToolBarPanel::ID_ADDBUTTON, _("Add button"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add( m_addbutton, 0, wxALL| wxEXPAND, 5);

	m_rembutton = new wxButton( this, wxGISToolBarPanel::ID_REMOVEBUTTON, _("Remove button"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add( m_rembutton, 0, wxALL| wxEXPAND, 5);

	m_moveup = new wxButton( this, wxGISToolBarPanel::ID_MOVECONTROLUP, _("Move up"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add( m_moveup, 0, wxALL| wxEXPAND, 5);

	m_movedown = new wxButton( this, wxGISToolBarPanel::ID_MOVECONTROLDOWN, _("Move down"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add( m_movedown, 0, wxALL| wxEXPAND, 5);

	bSizer3->Add( bSizer3, 0, wxEXPAND, 5);

	m_splitter1->SetSashGravity(0.5);
	m_splitter1->SplitVertically(m_commandbarlist, m_buttonslist, 100);

	m_pContextMenu = new wxMenu();
	wxMenuItem *item = new wxMenuItem(m_pContextMenu, ID_ONSETKEYCODE, _("Set keycode"));
	item->SetBitmap(wxKeyCodeDlg::GetBitmap());
	m_pContextMenu->Append(item);

	this->SetSizer( bSizer );
	this->Layout();

	//load toolbars & menues
	unsigned int counter(0);
	m_nMenubarPos = counter;

	wxString sName = _("--- Menubar ---");
	m_commandbarlist->Insert(sName, counter);
	m_CategoryArray.push_back(NULL);
	counter++;

	wxGISMenuBar* pwxGISMenuBar = m_pGxApp->GetMenuBar();
	COMMANDBARARRAY* pMenuBarArray = pwxGISMenuBar->GetMenuBarArray();
	for (size_t i = 0; i < pMenuBarArray->size(); i++)
	{
		IGISCommandBar* pBar = pMenuBarArray->at(i);
		wxString sCaption = pBar->GetCaption();
		sCaption += wxString(_(" Menu"));

		m_CategoryArray.push_back(pBar);

		m_commandbarlist->Insert(sCaption, counter);
		m_commandbarlist->Check(counter);
		counter++;
	}
	m_nContextMenuPos = counter;
	m_CategoryArray.push_back(NULL);
	m_commandbarlist->Insert(_("--- Context menues ---"), counter);
	counter++;

	COMMANDBARARRAY* pMenuArray = m_pGxApp->GetCommandBars();
	for (size_t i = 0; i < pMenuArray->size(); i++)
	{
		IGISCommandBar* pBar = pMenuArray->at(i);
		if (pBar->GetType() == enumGISCBContextmenu)
		{
			wxString sCaption = pBar->GetCaption();
			sCaption += wxString(_(" context menu"));

			m_CategoryArray.push_back(pBar);

			m_commandbarlist->Insert(sCaption, counter);
			counter++;
		}
	}

	m_nToolbarPos = counter;
	m_commandbarlist->Insert(_("--- ToolBars ---"), counter);
	m_CategoryArray.push_back(NULL);
	counter++;

	COMMANDBARARRAY* pToolBarArray = m_pGxApp->GetCommandBars();
	for (size_t i = 0; i < pToolBarArray->size(); i++)
	{
		IGISCommandBar* pBar = pToolBarArray->at(i);
		if (pBar->GetType() == enumGISCBToolbar)
		{
			wxString sCaption = pBar->GetCaption();
			sCaption += wxString(_(" toolbar"));

			m_CategoryArray.push_back(pBar);

			m_commandbarlist->Insert(sCaption, counter);
			m_commandbarlist->Check(counter, m_pGxApp->IsPaneShown(pBar->GetName()));
			counter++;
		}
	}
}

wxGISToolBarPanel::~wxGISToolBarPanel()
{
	wxDELETE(m_pContextMenu);
}

void wxGISToolBarPanel::OnDoubleClickSash(wxSplitterEvent& event)
{
	event.Veto();
}

void wxGISToolBarPanel::OnCheckboxToggle(wxCommandEvent& event)
{
	int pos = event.GetInt();
	if(pos == wxNOT_FOUND)
		return;
	if (m_CategoryArray[pos] == NULL)
	{
		//
		bool bIsChecked = m_commandbarlist->IsChecked(pos);
		m_commandbarlist->Check(pos, !bIsChecked);
	}
	else if (m_CategoryArray[pos]->GetType() != enumGISCBToolbar)
	{
		//
		bool bIsChecked = m_commandbarlist->IsChecked(pos);
		m_commandbarlist->Check(pos, !bIsChecked);
	}
	else
	{
		IGISCommandBar* pToolBar = m_CategoryArray[pos];
		if (pToolBar)
		{
			bool bIsChecked = m_commandbarlist->IsChecked(pos);
			m_pGxApp->ShowPane(pToolBar->GetName(), bIsChecked);
		}
	}
}

void wxGISToolBarPanel::OnListboxSelect(wxCommandEvent& event)
{
	LoadCommands();
}

void wxGISToolBarPanel::LoadCommands(void)
{
	m_buttonslist->DeleteAllItems();
	int selpos = m_commandbarlist->GetSelection();
	if (selpos == wxNOT_FOUND)
		return;

	IGISCommandBar* pwxGISCommandBar = m_CategoryArray[selpos];
	if(pwxGISCommandBar == NULL)
		return;

	m_ImageList.RemoveAll();
	for(size_t i =0; i< pwxGISCommandBar->GetCommandCount(); i++)
	{
		ICommand* pCommand = pwxGISCommandBar->GetCommand(i);
		wxString sName = wxStripMenuCodes(pCommand->GetCaption());
		wxString sMessage = pCommand->GetMessage();

		wxString sKeyCode = m_pGxApp->GetGISAcceleratorTable()->GetText(pCommand->GetID());
		int nIndex = m_ImageList.Add(pCommand->GetBitmap());
		long pos = m_buttonslist->InsertItem(i, sName, nIndex);
		m_buttonslist->SetItem(pos, 1, sMessage);
		m_buttonslist->SetItem(pos, 2, sKeyCode);
		m_buttonslist->SetItemData(pos, pCommand->GetID());
	}
	m_buttonslist->Update();
}

void wxGISToolBarPanel::OnListctrlActivated(wxListEvent& event)
{
	OnSetKeyCode(event.GetIndex());
}

void wxGISToolBarPanel::OnListctrlRClick(wxListEvent& event)
{
	PopupMenu(m_pContextMenu);
}

void wxGISToolBarPanel::OnSetKeyCode(int pos)
{
	wxKeyCodeDlg dlg(this);
	wxListItem item;
	item.SetColumn(2);
	item.SetId(pos);
	item.SetMask(wxLIST_MASK_TEXT);
	if(m_buttonslist->GetItem(item))
		dlg.m_sKeyCode = item.GetText();
	if (dlg.ShowModal() == wxID_OK)
	{
		long CmdID = m_buttonslist->GetItemData(pos);
		wxGISAcceleratorTable *pAccTab = m_pGxApp->GetGISAcceleratorTable();
		long cmd = pAccTab->Add(wxAcceleratorEntry(dlg.m_Flags, dlg.m_Key, CmdID));
		wxString sKeyCode = pAccTab->GetText(CmdID);
		m_buttonslist->SetItem(pos, 2, sKeyCode);
		if (cmd != wxID_ANY)
		{
			for (size_t i = 0; i< m_buttonslist->GetItemCount(); i++)
			{
				long CahgedCmdID = m_buttonslist->GetItemData(i);
				if(CahgedCmdID == cmd)
					m_buttonslist->SetItem(pos, 2, wxT(""));
			}
		}
	}
}

void wxGISToolBarPanel::OnSetKeyCode(wxCommandEvent& event)
{
	long item = GetSelectedCommandItem();
	if(item != wxNOT_FOUND)
		OnSetKeyCode(item);
}

void wxGISToolBarPanel::OnUpdateUI(wxUpdateUIEvent& event)
{
	if (wxWindow::FindFocus() == m_buttonslist)
	{
		m_bCmdFocus = true;
		m_bToolsFocus = false;
	}
	if (wxWindow::FindFocus() == m_commandbarlist)
	{
		m_bCmdFocus = false;
		m_bToolsFocus = true;
	}

	switch(event.GetId())
	{
	case wxGISToolBarPanel::ID_CREATECB:
		break;
	case wxGISToolBarPanel::ID_DELETECB:
		if(m_commandbarlist->GetSelection() == wxNOT_FOUND)
			event.Enable(false);
		else
		{
			int pos = m_commandbarlist->GetSelection();
			if(pos != m_nContextMenuPos && pos != m_nMenubarPos && pos != m_nToolbarPos)
				event.Enable(true);
			else
				event.Enable(false);
		}
		break;
	case wxGISToolBarPanel::ID_ADDBUTTON:
		if(m_commandbarlist->GetSelection() == wxNOT_FOUND)
			event.Enable(false);
		else
		{
			int pos = m_commandbarlist->GetSelection();
			if(pos != m_nContextMenuPos && pos != m_nMenubarPos && pos != m_nToolbarPos)
				event.Enable(true);
			else
				event.Enable(false);
		}
		break;
	case wxGISToolBarPanel::ID_REMOVEBUTTON:
		if(m_buttonslist->GetSelectedItemCount() == 1)
			event.Enable(true);
		else
			event.Enable(false);
	case wxGISToolBarPanel::ID_MOVECONTROLUP:
		if (m_bCmdFocus)
		{
			int nCurrentButtonSel = GetSelectedCommandItem();
			if(nCurrentButtonSel > 0 && nCurrentButtonSel <= m_buttonslist->GetItemCount() - 1)
				event.Enable(true);
			else
				event.Enable(false);
			break;
		}
		if (m_bToolsFocus)
		{
			int selpos = m_commandbarlist->GetSelection();
			if (selpos != wxNOT_FOUND)
			{
				if (m_CategoryArray[selpos] != NULL && m_CategoryArray[selpos]->GetType() == enumGISCBMenubar)
				{
					if(selpos > m_nMenubarPos + 1 && selpos <= m_nContextMenuPos - 1)
						event.Enable(true);
					else
						event.Enable(false);
					break;
				}
			}
		}
		event.Enable(false);
		break;
	case wxGISToolBarPanel::ID_MOVECONTROLDOWN:
		if (m_bCmdFocus)
		{
			int nCurrentButtonSel = GetSelectedCommandItem();
			if(nCurrentButtonSel >= 0 && nCurrentButtonSel < m_buttonslist->GetItemCount() - 1)
				event.Enable(true);
			else
				event.Enable(false);
			break;
		}
		if (m_bToolsFocus)
		{
			int selpos = m_commandbarlist->GetSelection();
			if (selpos != wxNOT_FOUND)
			{
				if (m_CategoryArray[selpos] != NULL && m_CategoryArray[selpos]->GetType() == enumGISCBMenubar)
				{
					if(selpos >= m_nMenubarPos + 1 && selpos < m_nContextMenuPos - 1)
						event.Enable(true);
					else
						event.Enable(false);
					break;
				}
			}
		}
		event.Enable(false);
		break;
	}
}

void wxGISToolBarPanel::OnCreateCommandBar(wxCommandEvent& event)
{
	wxGISCreateCommandBarDlg dlg(this);
	if(dlg.ShowModal() == wxID_OK)
	{
		switch (dlg.m_CommandbarType)
		{
		case enumGISCBMenubar:
			{
				wxGISMenu* pMenu = new wxGISMenu(dlg.m_sCommandbarName, dlg.m_sCommandbarCaption, enumGISCBMenubar);
				if (m_pGxApp->AddCommandBar(static_cast<IGISCommandBar*>(pMenu)))
				{
					wxString sCaption = pMenu->GetCaption();
					sCaption += wxString(_(" menu"));

					m_CategoryArray.insert(m_CategoryArray.begin() + m_nContextMenuPos, static_cast<IGISCommandBar*>(pMenu));

					m_commandbarlist->Insert(sCaption, m_nContextMenuPos);
					m_commandbarlist->Check(m_nContextMenuPos);
					m_nContextMenuPos++;
					m_nToolbarPos++;
				}
			}
			break;
		case enumGISCBContextmenu:
			{
				wxGISMenu* pMenu = new wxGISMenu(dlg.m_sCommandbarName, dlg.m_sCommandbarCaption, enumGISCBContextmenu);
				if (m_pGxApp->AddCommandBar(static_cast<IGISCommandBar*>(pMenu)))
				{
					wxString sCaption = pMenu->GetCaption();
					sCaption += wxString(_(" context menu"));

					m_CategoryArray.insert(m_CategoryArray.begin() + m_nToolbarPos, static_cast<IGISCommandBar*>(pMenu));

					m_commandbarlist->Insert(sCaption, m_nToolbarPos);
					m_commandbarlist->Check(m_nToolbarPos, false);
					m_nToolbarPos++;
				}
			}
			break;
		case enumGISCBToolbar:
			{

				wxGISToolBar* pGISToolBar = new wxGISToolBar(m_pGxApp, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW, dlg.m_sCommandbarName, dlg.m_sCommandbarCaption, enumGISCBToolbar);
			    pGISToolBar->SetLeftDockable(dlg.m_bLeftDockable);
				pGISToolBar->SetRightDockable(dlg.m_bRightDockable);
				if (m_pGxApp->AddCommandBar(static_cast<IGISCommandBar*>(pGISToolBar)))
				{
					wxAuiToolBarItemArray prepend_items;
					wxAuiToolBarItemArray append_items;
					ICommand* pCmd = m_pGxApp->GetCommand(wxT("wxGISCommonCmd"), 2);
					if (pCmd)
					{
						wxAuiToolBarItem item;
						item.SetKind(wxITEM_SEPARATOR);
						append_items.Add(item);
						item.SetKind(pCmd->GetKind());
						item.SetId(pCmd->GetID());
						item.SetLabel(pCmd->GetCaption());
						append_items.Add(item);
					}
					pGISToolBar->SetCustomOverflowItems(prepend_items, append_items);

					wxString sCaption = pGISToolBar->GetCaption();
					sCaption += wxString(_(" toolbar"));

					m_CategoryArray.push_back(static_cast<IGISCommandBar*>(pGISToolBar));

					m_commandbarlist->Append(sCaption);
					m_commandbarlist->Check(m_commandbarlist->GetCount() - 1, m_pGxApp->IsPaneShown(pGISToolBar->GetName()));
				}
			}
			break;
		case enumGISCBSubMenu:
		case enumGISCBNone:
		default:
			break;
		}
	}
}

void wxGISToolBarPanel::OnDeleteCommandBar(wxCommandEvent& event)
{
	int selpos = m_commandbarlist->GetSelection();
	if (selpos != wxNOT_FOUND)
	{
		m_commandbarlist->Delete(selpos);
		m_pGxApp->RemoveCommandBar(m_CategoryArray[selpos]);
	}
}

void wxGISToolBarPanel::OnAddButton(wxCommandEvent& event)
{
	wxGISAddCommandDlg dlg(m_pGxApp, this);
	if (dlg.ShowModal() == wxID_OK)
	{
		for (size_t i =0; i< dlg.m_IDArray.size(); i++)
		{
			int selpos = m_commandbarlist->GetSelection();
			if(selpos < 0 || selpos >= m_CategoryArray.size())
				return;
			IGISCommandBar* pBar = m_CategoryArray[selpos];
			pBar->AddCommand(m_pGxApp->GetCommand(dlg.m_IDArray[i]));
			wxWindow* pWnd = dynamic_cast<wxWindow*>(pBar);
			if (pWnd)
			{
				wxSize sz = pWnd->GetSize();
				m_pGxApp->GetAuiManager()->GetPane(pWnd).BestSize(sz);
			}
			m_pGxApp->GetAuiManager()->Update();
			LoadCommands();
		}
	}
}

void wxGISToolBarPanel::OnRemoveButton(wxCommandEvent& event)
{
	long item = GetSelectedCommandItem();
	if (item != wxNOT_FOUND)
	{
		int selpos = m_commandbarlist->GetSelection();
		if (selpos != wxNOT_FOUND)
		{
			m_buttonslist->DeleteItem(item);
			m_CategoryArray[selpos]->RemoveCommand(item);
			wxWindow* pWnd = dynamic_cast<wxWindow*>(m_CategoryArray[selpos]);
			if (pWnd)
			{
				wxSize sz = pWnd->GetSize();
				m_pGxApp->GetAuiManager()->Update();
			}
		}
	}
}

void wxGISToolBarPanel::OnMoveUp(wxCommandEvent& event)
{
	int selpos = m_commandbarlist->GetSelection();
	if (selpos == wxNOT_FOUND)
		return;
	if (m_bToolsFocus)
	{
		wxString sCaption = m_commandbarlist->GetString(selpos);
		bool bIsCheck = m_commandbarlist->IsChecked(selpos);
		m_commandbarlist->Delete(selpos);
		m_commandbarlist->Insert(sCaption, selpos - 1);
		m_commandbarlist->Check(selpos - 1, bIsCheck);
		m_commandbarlist->Select(selpos - 1);
		m_commandbarlist->Update();


		//swap
		IGISCommandBar* pBar = m_CategoryArray[selpos];
		m_CategoryArray[selpos] = m_CategoryArray[selpos - 1];
		m_CategoryArray[selpos - 1] = pBar;

		selpos -= (m_nMenubarPos + 1);
		wxGISMenuBar* pwxGISMenuBar = m_pGxApp->GetMenuBar();
		pwxGISMenuBar->MoveLeft(selpos);
		pwxGISMenuBar->Update();	
	}
	if (m_bCmdFocus)
	{
		long item = GetSelectedCommandItem();
		if (item != wxNOT_FOUND)
		{
			IGISCommandBar* pBar = m_CategoryArray[selpos];
			pBar->MoveCommandLeft(item);
			m_pGxApp->GetAuiManager()->Update();

			LoadCommands();
			m_buttonslist->Select(item - 1);
			m_buttonslist->SetFocus();
		}
	}
}

void wxGISToolBarPanel::OnMoveDown(wxCommandEvent& event)
{
	int selpos = m_commandbarlist->GetSelection();
	if(selpos == wxNOT_FOUND)
		return;
	if (m_bToolsFocus)
	{
		wxString sCaption = m_commandbarlist->GetString(selpos);
		bool bIsCheck = m_commandbarlist->IsChecked(selpos);
		m_commandbarlist->Delete(selpos);
		m_commandbarlist->Insert(sCaption, selpos + 1);
		m_commandbarlist->Check(selpos + 1, bIsCheck);
		m_commandbarlist->Select(selpos + 1);
		m_commandbarlist->Update();

		//swap
		IGISCommandBar* pBar = m_CategoryArray[selpos];
		m_CategoryArray[selpos] = m_CategoryArray[selpos + 1];
		m_CategoryArray[selpos + 1] = pBar;

		selpos -= (m_nMenubarPos + 1);
		wxGISMenuBar* pwxGISMenuBar = m_pGxApp->GetMenuBar();
		pwxGISMenuBar->MoveRight(selpos);
		pwxGISMenuBar->Update();
	}
	if(m_bCmdFocus)
	{
		long item = GetSelectedCommandItem();
		if(item != wxNOT_FOUND)
		{
			IGISCommandBar* pBar = m_CategoryArray[selpos];
			pBar->MoveCommandRight(item);
			m_pGxApp->GetAuiManager()->Update();

			LoadCommands();
			m_buttonslist->Select(item + 1);
			m_buttonslist->SetFocus();
		}
	}
}

long wxGISToolBarPanel::GetSelectedCommandItem(void)
{
	return m_buttonslist->GetFirstSelected();
}

/////
/// Class wxGISCommandPanel
////


BEGIN_EVENT_TABLE(wxGISCommandPanel, wxPanel)
	EVT_LISTBOX(wxGISCommandPanel::ID_LSTBX, wxGISCommandPanel::OnListboxSelect)
	EVT_SPLITTER_DCLICK(wxID_ANY, wxGISCommandPanel::OnDoubleClickSash)
	EVT_LIST_ITEM_RIGHT_CLICK(wxGISCommandPanel::ID_LSTCTRL, wxGISCommandPanel::OnListctrlRClick)
	EVT_LIST_ITEM_ACTIVATED(wxGISCommandPanel::ID_LSTCTRL, wxGISCommandPanel::OnListctrlActivated)
	EVT_MENU(ID_ONSETKEYCODE, wxGISCommandPanel::OnSetKeyCode)
END_EVENT_TABLE()

wxGISCommandPanel::wxGISCommandPanel(wxGxApplication* pGxApp, wxWindow* parent,wxWindowID id,const wxPoint& pos, const wxSize& size, long style ) :  wxPanel( parent, id , pos, size, style)
{
	m_pGxApp = pGxApp;
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	m_splitter2 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxNO_BORDER );
	m_splitter2->Connect( wxEVT_IDLE, wxIdleEventHandler( wxGISCommandPanel::m_splitter2OnIdle), NULL, this );
	bSizer5->Add( m_splitter2, 1, wxEXPAND, 5);

	//
	COMMANDARRAY* pArr = m_pGxApp->GetCommands();
	if (pArr)
	{
		for (size_t i = 0; i< pArr->size(); i++)
		{
			wxString sCat = pArr->at(i)->GetCategory();
			if(m_CategoryMap[sCat] == NULL)
				m_CategoryMap[sCat] = new COMMANDARRAY;
			m_CategoryMap[sCat]->push_back(pArr->at(i));
		}
	}
	wxArrayString CatArray;
	for (CATEGORYMAP::iterator IT = m_CategoryMap.begin(); IT !=  m_CategoryMap.end(); ++IT)
	{
		CatArray.Add(IT->first);
	}

	m_listBox1 = new wxListBox( m_splitter2, wxGISCommandPanel::ID_LSTBX, wxDefaultPosition, wxDefaultSize, CatArray, wxLC_SINGLE_SEL| wxLC_SORT_ASCENDING| wxNO_BORDER );
	//
	//

	m_listCtrl3 = new wxListCtrl( m_splitter2, wxGISCommandPanel::ID_LSTCTRL, wxDefaultPosition, wxDefaultSize, wxLC_NO_SORT_HEADER|wxLC_REPORT|wxLC_SORT_ASCENDING|wxNO_BORDER );
	m_listCtrl3->InsertColumn(0, _("Command Name"), wxLIST_FORMAT_LEFT, 90);
	m_listCtrl3->InsertColumn(1, _("Description"), wxLIST_FORMAT_LEFT, 120);
	m_listCtrl3->InsertColumn(2, _("KeyCode"), wxLIST_FORMAT_LEFT, 60);
	m_ImageList.Create(16, 16);
	//
	m_splitter2->SetSashGravity(0.5);
	m_splitter2->SplitVertically(m_listBox1, m_listCtrl3, 150);

	m_pContextMenu = new wxMenu();
	wxMenuItem * item = new wxMenuItem(m_pContextMenu, ID_ONSETKEYCODE, _("Set keycode"));
	item->SetBitmap(wxKeyCodeDlg::GetBitmap());
	m_pContextMenu->Append(item);

	this->SetSizer( bSizer5 );
	this->Layout();

	m_listBox1->Select(0);
	wxCommandEvent event;
	event.SetString(m_listBox1->GetString(0));
	OnListboxSelect(event);
}

wxGISCommandPanel::~wxGISCommandPanel()
{
	for (CATEGORYMAP::iterator IT = m_CategoryMap.begin(); IT != m_CategoryMap.end(); ++IT)
	{
		wxDELETE(IT->second);
	}
	wxDELETE(m_pContextMenu);
}

void wxGISCommandPanel::OnListboxSelect(wxCommandEvent& event)
{
	int selpos = m_listBox1->GetSelection();
	if(selpos == wxNOT_FOUND)
		return;

	wxString selName = m_listBox1->GetString(selpos);
	COMMANDARRAY* pArr = m_CategoryMap[selName];
	if (pArr != NULL)
	{
		m_listCtrl3->DeleteAllItems();
		m_ImageList.RemoveAll();
		for (size_t i = 0; i< pArr->size(); i++)
		{
			wxString sName = wxStripMenuCodes(pArr->at(i)->GetCaption());
			wxString sMessage = pArr->at(i)->GetMessage();
			wxString sKeyCode = m_pGxApp->GetGISAcceleratorTable()->GetText(pArr->at(i)->GetID());
			int nIndex = m_ImageList.Add(pArr->at(i)->GetBitmap());
			long pos = m_listCtrl3->InsertItem(i, sName, nIndex);
			m_listCtrl3->SetItem(pos, 1, sMessage);
			m_listCtrl3->SetItem(pos, 2, sKeyCode);
			m_listCtrl3->SetItemData(pos, pArr->at(i)->GetID());
		}
		m_listCtrl3->Update();
	}
}

void wxGISCommandPanel::OnDoubleClickSash(wxSplitterEvent& event)
{
	event.Veto();
}

void wxGISCommandPanel::OnListctrlActivated(wxListEvent& event)
{
	m_CurSelection = event.GetIndex();
	OnSetKeyCode(event.GetIndex());
}

void wxGISCommandPanel::OnListctrlRClick(wxListEvent& event)
{
	m_CurSelection = event.GetIndex();
	PopupMenu(m_pContextMenu);
}

void wxGISCommandPanel::OnSetKeyCode(int pos)
{
	wxKeyCodeDlg dlg(this);
	wxListItem item;
	item.SetColumn(2);
	item.SetId(pos);
	item.SetMask(wxLIST_MASK_TEXT);
	if(m_listCtrl3->GetItem(item))
		dlg.m_sKeyCode = item.GetText();
	if(dlg.ShowModal() == wxID_OK)
	{
		long CmdID = m_listCtrl3->GetItemData(pos);
		wxGISAcceleratorTable *pAccTab = m_pGxApp->GetGISAcceleratorTable();
		long cmd = pAccTab->Add(wxAcceleratorEntry(dlg.m_Flags, dlg.m_Key, CmdID));
		wxString sKeyCode = pAccTab->GetText(CmdID);
		m_listCtrl3->SetItem(pos, 2, sKeyCode);
		if (cmd != wxID_ANY)
		{
			for (size_t i = 0; i < m_listCtrl3->GetItemCount(); i++)
			{
				long CahgedCmdID = m_listCtrl3->GetItemData(i);
				if(CahgedCmdID == cmd)
					m_listCtrl3->SetItem(pos, 2, wxT(""));
			}
		}
	}
}

void wxGISCommandPanel::OnSetKeyCode(wxCommandEvent& event)
{
	OnSetKeyCode(m_CurSelection);
}

//////
/// Class wxGISCustomizedDlg
/////////

wxGISCustomizeDlg::wxGISCustomizeDlg(wxWindow* parent, wxWindowID id /* = wxID_ANY */, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	m_pGxApp = dynamic_cast<wxGxApplication*>(parent);
	this->SetSizeHints( wxSize( 500, 400 ), wxDefaultSize );
	if(!m_pGxApp)
		return;

	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );

	m_auinotebook = new wxAuiNotebook( this, wxID_ANY,wxDefaultPosition, wxDefaultSize, wxAUI_NB_TOP | wxNO_BORDER | wxAUI_NB_TAB_MOVE );
	m_auinotebook->AddPage(new wxGISToolBarPanel(m_pGxApp, m_auinotebook), _("ToolBars & Menues"));
	m_auinotebook->AddPage(new wxGISToolBarPanel(m_pGxApp, m_auinotebook), _("Commands"));

	bSizerMain->Add( m_auinotebook, 1, wxEXPAND | wxALL, 5);

	m_sdbSizer = new wxStdDialogButtonSizer();
	m_sdbSizerOK = new wxButton( this, wxID_OK, wxString(_("Close")) );
	m_sdbSizer->AddButton( m_sdbSizerOK );
	//
	//
	m_sdbSizer->Realize();
	bSizerMain->Add( m_sdbSizer, 0, wxALL|wxEXPAND, 5);

	this->SetSizer( bSizerMain );
	this->Layout();
}

wxGISCustomizeDlg::~wxGISCustomizeDlg()
{

}