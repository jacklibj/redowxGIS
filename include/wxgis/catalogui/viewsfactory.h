#include "catalogui.h"

class wxGISViewsFactory :
	public wxObject,
	public IGxViewsFactory
{
	DECLARE_DYNAMIC_CLASS(wxGISViewsFactory)
public:
	wxGISViewsFactory(void);
	virtual ~wxGISViewsFactory(void);
	virtual wxWindow* CreateView(wxString sName, wxWindow* parent);
};