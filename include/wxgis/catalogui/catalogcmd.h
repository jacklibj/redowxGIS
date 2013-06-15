#pragma once
#include "wxgis/framework/framework.h"

class wxGISCatalogMainCmd :
	public IToolControl
{
	DECLARE_DYNAMIC_CLASS(wxGISCatalogMainCmd)
public:
	wxGISCatalogMainCmd(void);
	virtual ~wxGISCatalogMainCmd(void);
	//ICommand
	virtual wxIcon GetBitmap(void);
	virtual wxString GetCaption(void);
	virtual wxString GetCategory(void);
	virtual bool GetChecked(void);
	virtual bool GetEnabled(void);
	virtual wxString GetMessage(void);
	virtual wxGISEnumCommandKind GetKind(void);
	virtual void OnClick(void);
	virtual bool OnCreate(IApplication* pApp);
	virtual wxString GetTooltip(void);
	virtual unsigned char GetCount(void);
	//IToolControl
	virtual IToolBarControl* GetControl(void);
	virtual wxString GetToolLabel(void);
	virtual bool HasToolLabel(void);
private:
	IApplication* m_pApp;
	wxImageList m_ImageList;
};