#pragma once
#include "wxgis/framework/framework.h"

class wxGISCommonCmd :
	public ICommand
{
	DECLARE_DYNAMIC_CLASS(wxGISCommonCmd)
public:
	wxGISCommonCmd(void);
	virtual ~wxGISCommonCmd(void);
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
private:
	IApplication* m_pApp;
};