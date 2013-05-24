#pragma once
#include "wxgis/framework/framework.h"
#include "wxgis/carto/mapview.h"

//------------
// wxGISCartoMainCmd
//----------------------
class wxGISCartoMainCmd :
	public ICommand
{
	DECLARE_DYNAMIC_CLASS(wxGISCartoMainCmd)
public:
	wxGISCartoMainCmd(void);
	virtual ~wxGISCartoMainCmd(void);
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
	wxImageList m_ImageList;
	wxGISMapView* m_pMapView;
};

//----
// wxGISCartoMainTool
//-----

class wxGISCartoMainTool :
{
	DECLARE_DYNAMIC_CLASS(wxGISCartoMainTool)
public:
	wxGISCartoMainTool(void);
	virtual ~wxGISCartoMainTool(void);
	//ICommand
	virtual wxIcon GetBitmap(void);
	virtual wxString GetCaption(void);
	virtual wxString GetCategory(void);
	virtual bool GetChecked(void);
	virtual bool GetEnabled(void);
	virtual wxString GetMessage(void);
	virtual wxGISEnumCommandKind GetKind(void);
	virtual void OnClick(void);

};