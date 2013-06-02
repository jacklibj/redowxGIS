#include "wxgis/catalogui/gxlocationcombobox.h"

BEGIN_EVENT_TABLE(wxGxLocationComboBox, wxComboBox)
	EVT_TEXT_ENTER(wxID_ANY, wxGxLocationComboBox::OnTextEnter)
	EVT_COMBOBOX(wxID_ANY, wxGxLocationComboBox::OnTextEnter)
END_EVENT_TABLE()

wxGxLocationComboBox::wxGxLocationComboBox(WXWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size, 
const wxArrayString& choices, long style /* = 0 */, const wxValidator& validator /* = wxDefaultValidator */, const wxString& name /* = wxT */) :
wxComboBox(parent, id , value, pos, size, choices, style, validator, name), m_pApp(NULL)
{

}

wxGxLocationComboBox::~wxGxLocationComboBox(void)
{

}

void wxGxLocationComboBox::OnTextEnter(wxCommandEvent& event)
{
	wxGxApplication* pGxApp = dynamic_cast<wxGxApplication*>(m_pApp);
	if (pGxApp)
	{
		IGxCatalog* pCatalog = pGxApp->GetCatalog();
		if (pCatalog)
		{
			pCatalog->SetLocation(GetValue());
			for(size_t i = 0; i < m_ValuesArr.size(); i++)
				if(m_ValuesArr[i] == GetValue())
					return;
			Append(GetValue());
			m_ValuesArr.push_back(GetValue());
		}
	}
}

void wxGxLocationComboBox::OnSelectionChanged(IGxSelection* Selection, long nInitiator)
{
	if(nInitiator != TREECTRLID)
		return;
	GxObjectArray* pGxObjectArray = Selection->GetSelectedObjects();
	if(pGxObjectArray == NULL || pGxObjectArray->size() == 0)
		return;
	IGxObject* pGxObj = pGxObjectArray->at(pGxObjectArray->size() - 1);
	wxString sPath = pGxObj->GetFullName();
	if(sPath.IsEmpty())
		sPath = pGxObj->GetName();
	SetValue(sPath);
}

void wxGxLocationComboBox::Activate(IApplication* pApp)
{
	m_pApp = pApp;
	wxGxApplication* pGxApp = dynamic_cast<wxGxApplication*>(pApp);
	m_pConnectionPointSelection = dynamic_cast<IConnectionPointContainer*>( pGxApp->GetCatalog()->GetSelection() );
	if(m_pConnectionPointSelection != NULL)
		m_pConnectionPointSelectionCookie = m_pConnectionPointSelection->Advise(this);
}

void wxGxLocationComboBox::Deactivate(void)
{
	if(m_pConnectionPointSelectionCookie != -1)
		m_pConnectionPointSelection->Unadvise(m_pConnectionPointSelectionCookie);
}