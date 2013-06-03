#pragma once
#include "wxgis/catalogui/gxview.h"

class wxGxNoView :
	public wxControl,
	public wxGxView
{
public:
	wxGxNoView(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		                 const wxSize& size = wxDefaultSize);

	virtual ~wxGxNoView(void);
	void Serialized(wxXmlNode* pRootNode, bool bStore);
	void OnPaint(wxPaintEvent& WXUNUSED(evt));
	void OnEraseBackground(wxEraseEvent& WXUNUSED(evt));
	void OnSize(wxSizeEvent& WXUNUSED(evt));
//IGxView
	virtual bool Activate(wxGxApplication* application, IGxCatalog* Catalog, wxXmlNode* pConf);
	virtual void Deactivate(void);
private:
	unsigned char R,G,B;
	unsigned char FontSize;
	bool IsBold, IsItalic;

	DECLARE_EVENT_TABLE();
};