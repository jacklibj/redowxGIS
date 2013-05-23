#include "gxtabview.h"
#include "viewsfactory.h"
#include "gxapplication.h"


//---
// wxGxTab
//----
BEGIN_EVENT_TABLE(wxGxTab, wxPanel)
	EVT_CHOICE(wxID_ANY, wxGxTab::OnChoice)
END_EVENT_TABLE()

wxGxTab::wxGxTab(wxGxApplication* application, IGxCatalog* Catalog, wxXmlNode* pTabDesc, wxWindow* parent,
wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style), m_bShowChoices(false), m_pCurrentWnd(NULL)
{
	m_sName = wxGetTranslation( pTabDesc->GetPropVal(wxT("name"), NONAME) );
	m_bShowChoices = pTabDesc->GetPropVal( wxT("show_choices"), wxT("f")) == wxT("f") ? false : true;

	wxXmlNode* pChild = pTabDesc->GetChildren();
	while(pChild)
	{
		wxString sClass = pChild->GetPropVal(wxT("class"), ERR);
		wxString sName = pChild->GetPropVal(wxT("name"), NONAME);
		int nPriority = wxAtoi(pChild->GetPropVal(wxT("priority"), wxT("0")));

		wxObject *obj = wxCreateDynamicObject(sClass);
		IGxViewsFactory *pFactory =dynamic_cast<IGxViewsFactory*>(obj);
		if(pFactory != NULL)
		{
			wxWindow* pWnd = pFactory->CreateView(sName, this);
			if(pWnd != NULL)
			{
				pWnd->Hide();
				application->RegisterChildWindow(pWnd);

				wxGxView* pView = dynamic_cast<wxGxView*>(pWnd);
				if(pView != NULL)
				{
					pView->Activate(application, Catalog, pChild);
					if(m_pWindows.size() < nPriority)
						for(size_t i =0; i < nPriority + 1; i++)
							m_pWindows.push_back(NULL);

					m_pWindows[nPriority] = pWnd;
					wxLogMessage(_("wxGxTab: View class %s.%s in '%s' tab initialise"), sClass.c_str(),sName.c_str(), m_sName.c_str());
				}
				else
				{
					wxLogError(_("wxGxTab: This is not inherited IGxView class (%s.%s)"), sClass.c_str(), sName.c_str());
					wxDELETE(pWnd);
				}
			}
			else
			{
				wxLogError(_("wxGxTab: Error creating view %s.%s"), sClass.c_str(), sName.c_str());
				wxDELETE(pFactory);
			}
		}
		else
		{
			wxLogError(_("wxGxTab: Error initialing ViewsFacory %s"), sClass.c_str());
			wxDELETE(obj);
		}

		wxDELETE(pFactory);

		pChild = pChild->GetNext();
	}

	m_bSizerMain = new wxBoxSizer( wxVERTICAL );




	m_tabwnd = new wxPanel( this, wxID_ANY, wxDefaultPosition,wxDefaultSize, wxTAB_TRAVERSAL );
	m_bSizerMain->Add( m_tabwnd, 1, m_bShowChoices == true ? wxEXPAND | wxALL : wxEXPAND, 5 );
	m_pCurrentWnd = m_tabwnd;

	if(m_bShowChoices)
	{
		m_tabselector = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxTAB_TRAVERSAL );
		m_tabselector->SetMaxSize( wxSize(-1, 30) );

		wxBoxSizer* bSizerMinor = new wxBoxSizer( wxHORIZONTAL );

		m_staticText = new wxStaticText( m_tabselector, wxID_ANY, _("Preview: "), wxDefaultPosition, wxDefaultSize, 0);
		m_staticText->Wrap( -1 );
		bSizerMinor->Add( m_staticText, 0, wxALIGN_CENTER_VERTICAL, 5);

		m_choice = new wxChoice( m_tabselector, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0);
		m_choice->Disable();
		m_choice->SetMinSize( wxSize( 170, -1) );

		bSizerMinor->Add( m_choice, 0, wxALIGN_CENTER_VERTICAL, 5);

		m_tabselector->SetSizer( bSizerMinor );
		m_tabselector->Layout();
		bSizerMinor->Fit( m_tabselector );
		m_bSizerMain->Add( m_tabselector, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5);
	}
	this->SetSizer( m_bSizerMain );
	this->Layout();

	m_pSelection = Catalog->GetSelection();
}

wxString wxGxTab::GetName(void)
{
	return m_sName;
}

wxWindow* wxGxTab::GetWindow(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_pWindows.size())
		return NULL;
	return m_pWindows[iIndex];
}

void wxGxTab::OnSelectionChanged(IGxSelection* Selection, long nInitiator)
{
	if(!Selection)
		return;

	wxBusyCursor wait;

	if(nInitiator == GetId())
		return;
	//
	if(nInitiator != TREECTRLID && nInitiator != LISTCTRLID)
	{
		GxObjectArray* pGxObjectArray = Selection->GetSelectedObjects();
		if(!pGxObjectArray || pGxObjectArray->size() == 0)
			return;

		IGxObject* pGxObj = pGxObjectArray->at(pGxObjectArray->size() - 1);
		//
		Selection->Select(pGxObj, false, GetId());
		return;
	}

	if(m_bShowChoices)
	{
		m_choice->Clear();

		//
		wxWindow* pNoWnd(NULL);
		//
		for(size_t i =0; i< m_pWindows.size(); i++)
		{
			wxWindow* pWnd = m_pWindows[i];
			wxGxView* pView = dynamic_cast<wxGxView*>(pWnd);
			if(pView == NULL)
				continue;
			if(pView->Applies(Selection))
			{
				if(pView->GetName() != _("noView"))
					m_choice->Append(pView->GetName(), pWnd);
				else
					pNoWnd = pWnd;
			}
		}
		if(m_choice->GetCount() > 0)
		{
			m_choice->Enable(true);
			bool bWndPresent(false);

			for(size_t i = 0; i< m_choice->GetCount(); i++)
			{
				wxWindow* pWnd = (wxWindow*) m_choice->GetClientData(i);
				if(m_pCurrentWnd == pWnd)
				{
					m_choice->Select(i);
					bWndPresent = true;
					break;
				}
			}

			if(!bWndPresent)
			{
				m_choice->Select(0);
				wxWindow* pWnd = (wxWindow*) m_choice->GetClientData(0);
				if(pWnd != NULL)
				{
					m_pCurrentWnd->Hide();
					m_bSizerMain->Replace(m_pCurrentWnd, pWnd);
					pWnd->Show();
					m_pCurrentWnd = pWnd;
					this->Layout();
				}
			}

		}
		else
		{
			m_choice->Enable(false);
			if(pNoWnd != NULL)
			{
				m_pCurrentWnd->Hide();
				m_bSizerMain->Replace(m_pCurrentWnd, pNoWnd);
				pNoWnd->Show();
				m_pCurrentWnd = pNoWnd;
				this->Layout();
			}
		}
		goto END;
	}
	else
	{
		//
		wxGxView* pCurrView = dynamic_cast<wxGxView*>(m_pCurrentWnd);
		if(pCurrView != NULL && pCurrView->GetName() != _("NoView"))
		{
			if(pCurrView->Applies(Selection))
				goto END;
		}
		for(size_t i =0; i < m_pWindows.size() ; i++)
		{
			wxWindow* pWnd = m_pWindows[i];
			wxGxView* pView = dynamic_cast<wxGxView*>(pWnd);
			if(pView == NULL)
				continue;
			if(pView->Applies(Selection))
			{
				if(pCurrView != pView)
				{
					m_pCurrentWnd->Hide();
					m_bSizerMain->Replace(m_pCurrentWnd, pWnd);
					pWnd->Show();
					m_pCurrentWnd = pWnd;
					this->Layout();
				}
				goto END;
			}

		}
	}
END:
	IGxSelectionEvents* pGxSelectionEvents = dynamic_cast<IGxSelectionEvents*>(m_pCurrentWnd);
	if(pGxSelectionEvents != NULL)
		pGxSelectionEvents->OnSelectionChanged(Selection, nInitiator);
}

void wxGxTab::OnChoice(wxCommandEvent& event)
{
	event.Skip();
	int pos = event.GetSelection();
	if(pos < 0)
		return;

	wxWindow* pWnd = (wxWindow*) m_choice->GetClientData(pos);
	if( pWnd != NULL && m_pCurrentWnd != pWnd)
	{
		m_pCurrentWnd->Hide();
		m_bSizerMain->Replace(m_pCurrentWnd, pWnd);
		pWnd->Show();
		m_pCurrentWnd = pWnd;
		IGxSelectionEvents* pGxSelectionEvents = dynamic_cast<IGxSelectionEvents*>(m_pCurrentWnd);
		if(pGxSelectionEvents != NULL)
			pGxSelectionEvents->OnSelectionChanged(m_pSelection, GetId());

		this->Layout();
	}
}

bool wxGxTab::Show(bool bShow)
{
	if(m_pCurrentWnd)
		m_pCurrentWnd->Show(bShow);
	return wxWindow::Show(bShow);
}

wxGxTab::~wxGxTab(void)
{
	for(size_t i = 0; i < m_pWindows.size(); i++)
	{
		wxGxView* pView = dynamic_cast<wxGxView*>(m_pWindows[i]);
		if(pView != NULL)
			pView->Deactivate();
		wxDELETE(m_pWindows[i]);
	}
}

//------
// wxGxTabView
//------

BEGIN_EVENT_TABLE(wxGxTabView, wxAuiNotebook)
	EVT_AUINOTEBOOK_PAGE_CHANGED(TABCTRLID, wxGxTabView::OnAUINotebookPageChanged)
END_EVENT_TABLE()

wxGxTabView::wxGxTabView(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : wxAuiNotebook(parent, id, pos, size, wxAUI_NB_TOP | wxNO_BORDER | wxAUI_NB_TAB_MOVE),m_pSelection(NULL)
{
}

wxGxTabView::~wxGxTabView(void)
{
}

bool wxGxTabView::Activate(wxGxApplication* application, IGxCatalog* Catalog, wxXmlNode* pConf)
{
	if(!application || !Catalog || !pConf)
		return false;
	wxGxView::Activate(application, Catalog, pConf);

	wxXmlNode* pChild = m_pXmlConf->GetChildren();
	wxUint8 count(0);
	while(pChild)
	{
		wxGxTab* pGxTab = new wxGxTab(application, Catalog, pConf, this);



		m_Tabs.push_back(pGxTab);

		AddPage(static_cast<wxWindow*>(pGxTab), pGxTab->GetName(), count == 0 ? true : false );

		count++;

		pChild = pChild->GetNext();
	}

	m_pSelection = m_pCatalog->GetSelection();
	m_pConnectionPointSelection = dynamic_cast<IConnectionPointContainer*>( m_pSelection );
	if(m_pConnectionPointSelection != NULL)
		m_ConnectionPointSelectionCookie = m_pConnectionPointSelection->Advise(this);

	return true;
}

void wxGxTabView::Deactivate(void)
{
	if(m_ConnectionPointSelectionCookie != -1)
		m_pConnectionPointSelection->Unadvise(m_ConnectionPointSelectionCookie);
}

void wxGxTabView::OnSelectionChanged(IGxSelection* Selection, long nInitiator)
{
	int nSelTab = GetSelection();
	wxASSERT(nSelTab >= 0 && nSelTab < m_Tabs.size());

	wxGxTab* pCurrTab = m_Tabs[nSelTab];
	if(pCurrTab != NULL)
		pCurrTab->OnSelectionChanged(Selection, nInitiator);
}

void wxGxTabView::OnAUINotebookPageChanged(wxAuiNotebookEvent& event)
{
	event.Skip();
	int nSelTab = event.GetSelection();
	wxASSERT(nSelTab >= 0 && nSelTab < m_Tabs.size());

	wxGxTab* pCurrTab = m_Tabs[nSelTab];
	if(pCurrTab != NULL && m_pSelection != NULL)
		pCurrTab->OnSelectionChanged(m_pSelection, IGxSelection::INIT_ALL);
}

wxWindow* wxGxTabView::GetCurrentWnd(void)
{
	int nSelTab = GetSelection();
	wxASSERT(nSelTab >= 0 && nSelTab < m_Tabs.size());

	wxGxTab* pCurrTab = m_Tabs[nSelTab];
	if(pCurrTab)
		return pCurrTab->GetCurrentWindow();
	return NULL;
}
