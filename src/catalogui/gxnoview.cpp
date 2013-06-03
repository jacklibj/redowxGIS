#include "wxgis/catalogui/gxnoview.h"

BEGIN_EVENT_TABLE(wxGxNoView, wxControl)
	EVT_PAINT(wxGxNoView::OnPaint)
	EVT_SIZE(wxGxNoView::OnSize)
	EVT_ERASE_BACKGROUND(wxGxNoView::OnEraseBackground)
END_EVENT_TABLE()

bool wxGxNoView::Activate(wxGxApplication* application, IGxCatalog* Catalog, wxXmlNode* pConf)
{
	wxGxView::Activate(application, Catalog, pConf);
	Serialized(m_pXmlConf, false);
	return true;
}

void wxGxNoView::Deactivate(void)
{
	Serialized(m_pXmlConf, true);
	wxGxView::Deactivate();
}

wxGxNoView::wxGxNoView(wxWindow* parent, wxWindowID id /* = wxID_ANY */, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */)
	                           : wxControl(parent, id, pos, size, wxNO_BORDER)
{
	m_sViewName = wxString(_("NoView"));
	R = 255, G = 255, B = 230;
	IsBold = true;
	IsItalic = false;
	FontSize = 12;
}

wxGxNoView::~wxGxNoView(void)
{

}

void wxGxNoView::Serialized(wxXmlNode* pRootNode, bool bStore)
{
	if(pRootNode == NULL)
		return;
	if (bStore)
	{
		pRootNode->DeleteProperty(wxT("red"));
		pRootNode->AddProperty(wxT("red"), wxString::Format(wxT("%d"), R);
		pRootNode->DeleteProperty(wxT("green"));
		pRootNode->AddProperty(wxT("green"), wxString::Format(wxT("%d"), G);
		pRootNode->DeleteProperty(wxT("blue"));
		pRootNode->AddProperty(wxT("blue"), wxString::Format(wxT("%d"), B);
		pRootNode->DeleteProperty(wxT("bold"));
		pRootNode->AddProperty(wxT("bold"), wxString::Format(wxT("%d"), IsBold));
		pRootNode->DeleteProperty(wxT("italic"));
		pRootNode->AddProperty(wxT("italic"), wxString::Format(wxT("%d"), IsItalic));
		pRootNode->DeleteProperty(wxT("size"));
		pRootNode->AddProperty(wxT("size"), wxString::Format(wxT("%d"), FontSize));
	}
	else
	{
		R = wxAtoi(pRootNode->GetPropVal(wxT("red"), wxT("255")));
		G = wxAtoi(pRootNode->GetPropVal(wxT("green"), wxT("255")));
		B = wxAtoi(pRootNode->GetPropVal(wxT("blue"), wxT("230")));
		IsBold = wxAtoi(pRootNode->GetPropVal(wxT("bold"), wxT("1")));
		IsItalic = wxAtoi(pRootNode->GetPropVal(wxT("italic"), wxT("0")));
		FontSize = wxAtoi(pRootNode->GetPropVal(wxT("size"), wxT("10")));
	}
}

void wxGxNoView::OnPaint(wxPaintEvent& WXUNUSED)
{
	wxPaintDC dc(this);
	wxString s(_("Selection can not be viewed"));
	wxSize size = GetClientSize();
	int width, height;

	wxFont font(FontSize, wxFONTFAMILY_DEFAULT, IsItalic == true ? wxFONTSTYLE_ITALIC : wxFONTSTYLE_NORMAL,
		IsBold ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL);
	dc.SetFont(font);
	dc.GetTextExtent(s, &width, &height);
	height += 3;
	dc.SetBrush(*wxWHITE_BRUSH);
	dc.SetPen(*wxGREY_PEN);
	dc.DrawRectangle(0, 0, size.x, size.y);

	wxBrush br(wxColour(R,G,B);
	dc.SetBrush(br);
	dc.SetPen(*wxGREY_PEN);
	int x1 = (size.x - width) / 2, y1 = (size.y - height) / 2;
	dc.DrawRectangle( x1 - 5, y1 - 2, width + 10, height + 4);

	dc.DrawText(s, x1, y1);

}

void wxGxNoView::OnEraseBackground(wxEraseEvent& WXUNUSED(evt))
{

}

void wxGxNoView::OnSize(wxSizeEvent& WXUNUSED(evt))
{
	wxControl::Refresh();
}