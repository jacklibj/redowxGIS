#pragma once
#include "wxgis\framework\framework.h"
#include "wx\accel.h"

class WXDLLIMPEXP_GIS_FRW wxGISAcceleratorTable
{
public:
	wxGISAcceleratorTable(IApplication* pApp, IGISConfig* pConf);
	virtual ~wxGISAcceleratorTable(void);
	virtual int Add(wxAcceleratorEntry entry);
	virtual void Remove(wxAcceleratorEntry entry);
	virtual wxAcceleratorTable GetAcceleratorTable(void);
	virtual bool HasChanges(void){return bHasChanges;};
	virtual wxString GetText(int cmd);
	virtual void Store(void);
protected:
	int GetKeyCode(wxString sKeyCode);
	WXWORD GetFlags(wxString sFlags);
private:
	std::vector<wxAcceleratorEntry> m_AccelEntryArray;
	bool bHasChanges;
	wxAcceleratorTable m_ATab;
	IGISConfig* m_pConf;
	IApplication* m_pApp;
};