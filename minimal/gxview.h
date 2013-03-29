#pragma once
#include "catalogui.h"
#include "catalog.h"


class wxGxApplication;

class wxGxView
{
public:
	wxGxView(void);
	virtual ~wxGxView(void);
	virtual bool Activate(wxGxApplication* application, IGxCatalog* Catalog, wxXmlNode* pConf);
	virtual void Deactivate(void);
	virtual bool Applies(IGxSelection* Selection);
	virtual void Refresh(void) {};
	virtual wxString GetName(void);
protected:
	wxString m_sViewName;
	IGxCatalog* m_pCatalog;
	wxGxApplication* m_pApplication;
	wxXmlNode* m_pXmlConf;
};