#include "wxgis/catalogui/catalogui.h"
#include "wxgis/catalogui/gxapplication.h"

class wxGxLocationComboBox :
	public wxComboBox,
	public IGxSelectionEvents,
	public IToolBarControl
{
public:
	wxGxLocationComboBox(WXWindow*parent, WXWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size,
		const wxArrayString& choices, long style = 0, const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxT("LocationComboBox"));
	virtual ~wxGxLocationComboBox(void);
	//events
	void OnTextEnter(wxCommandEvent& event);
	//IGxSelectionEvent
	virtual void OnSelectionChanged(IGxSelection* Selection, long nInitiator);
	//IToolBatControl
	virtual void Activate(IApplication* pApp);
	virtual void Deactivate(void);
protected:
	IApplication* m_pApp;
	std::vector<wxString> m_ValuesArr;
	IConnectionPointContainer* m_pConnectionPointSelection;
	long m_pConnectionPointSelectionCookie;

	DECLARE_EVENT_TABLE()

};