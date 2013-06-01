#include "wxgis/carto/cartocmd.h"
#include "../../art/geography16.xpm"
#include "wxgis/display/rubberband.h"
#include "wxgis/display/simplefillsymbol.h"

// 0 Full Extent
// 1 Prev Extent
// 2 Next Extent
// 3 ?

//------
// wxGISCartoMainCmd
//-----

IMPLEMENT_DYNAMIC_CLASS(wxGISCartoMainCmd, wxObject)

wxGISCartoMainCmd::wxGISCartoMainCmd(void) : m_pMapView(NULL)
{
	m_ImageList.Create(16, 16);
	m_ImageList.Add(wxBitmap(geography16_xpm));
}

wxGISCartoMainCmd::~wxGISCartoMainCmd(void)
{

}

wxIcon wxGISCartoMainCmd::GetBitmap(void)
{
	switch(m_subtype)
	{
	case 0:
		return m_ImageList.GetIcon(9);
	case 1:
		return m_ImageList.GetIcon(6);
	case 2:
		return m_ImageList.GetIcon(7);
	default:
		return wxIcon();
	}
}

wxString wxGISCartoMainCmd::GetCaption(void)
{
	switch(m_subtype)
	{
	case 0:
		return wxString(_("&Full Extent"));
	case 1:
		return wxString(_("&Previous Extent"));
	case 2:
		return wxString(_("&Next Extent"));
	default:
		return wxString();
	}
}

wxString wxGISCartoMainCmd::GetCategory(void)
{
	switch(m_subtype)
	{
	case 0:
	case 1:
	case 2:
		return wxString(_("Geography"));
	default:
		return wxString(_("[No category]"));
	}
}

bool wxGISCartoMainCmd::GetChecked(void)
{
	switch(m_subtype)
	{
	case 0:
	case 1:
	case 2:
		return false;
	default:
		return false;
	}
}

bool wxGISCartoMainCmd::GetEnabled(void)
{
	if (!m_pMapView)
	{
		WINDOWARRAY* pWinArr = m_pApp->GetChildWindows();
		if (pWinArr)
		{
			for(size_t i = 0; i < pWinArr->size(); i++)
			{

				wxGISMapView* pMapView = dynamic_cast<wxGISMapView*>(pWinArr->at(i));
				if (pMapView)
				{
					m_pMapView = pMapView;
					break;
				}
			}
		}
	}

	switch(m_subtype)
	{
	case 0:
		return m_pMapView->IsShown();
	case 1:
		return m_pMapView->IsShown() && m_pMapView->GetExtentStack()->CanUndo();
	case 2:
		return m_pMapView->IsShown() && m_pMapView->GetExtentStack()->CanRedo();
	default:
		return false;
	}
}

wxGISEnumCommandKind wxGISCartoMainCmd::GetKind(void)
{
	switch(m_subtype)
	{
	case 0:
	case 1:
	case 2:
		return enumGISCommandNormal;
	default:
		return enumGISCommandNormal;
	}
}

wxString wxGISCartoMainCmd::GetMessage(void)
{
	switch(m_subtype)
	{
	case 0:
		return wxString(_("Full extent of the map"));
	case 1:
		return wxString(_("Previous extent of the map"));
	case 2:
		return wxString(_("Next extent of the map"));
	default:
		return wxString();
	}
}

void wxGISCartoMainCmd::OnClick(void)
{
	switch(m_subtype)
	{
	case 0:
		m_pMapView->SetFullExtent();
		break;
	case 1:
		return m_pMapView->GetExtentStack()->Undo();
	case 2:
		return m_pMapView->GetExtentStack()->Redo();
	default:
		break;
	}
}

bool wxGISCartoMainCmd::OnCreate(IApplication* pApp)
{
	m_pApp = pApp;
	return true;
}

wxString wxGISCartoMainCmd::GetTooltip(void)
{
	switch(m_subtype)
	{
	case 0:
		return wxString(_("Full Extent"));
	case 1:
		return wxString(_("Previous Extent"));
	case 2:
		return wxString(_("Next Extent"));
	default:
		return wxString();

	}
}

unsigned char wxGISCartoMainCmd::GetCount(void)
{
	return 3;
}

//----
// wxGISCartoMainTool
//-------------

// 0 //zooom in
// 1 //zoom out
// 2 //Pan
// 3 //Info
// 4 //?

IMPLEMENT_DYNAMIC_CLASS(wxGISCartoMainTool, wxObject)

wxGISCartoMainTool::wxGISCartoMainTool(void) : m_pMapView(NULL), m_bCheck(false)
{
   m_ImageList.Create(16, 16);
   m_ImageList.Add(wxBitmap(geography16_xpm));
}

wxGISCartoMainTool::~wxGISCartoMainTool(void)
{

}

wxIcon wxGISCartoMainTool::GetBitmap(void)
{
	switch(m_subtype)
	{
	case 0:
		return m_ImageList.GetIcon(5);
	case 1:
		return m_ImageList.GetIcon(4);
	case 2:
		return m_ImageList.GetIcon(1);
	case 3:
		return m_ImageList.GetIcon(10);
	default:
		wxIcon();
	}
}

wxString wxGISCartoMainTool::GetCaption(void)
{
	switch(m_subtype)
	{
	case 0:
		return wxString(_("Zoom &In"));
	case 1:
		return wxString(_("Zoom &Out"));
	case 2:
		return wxString(_("&Pan"));
	case 3:
		return wxString(_("&Information"));
	default:
		return wxString()
	}
}

wxString wxGISCartoMainTool::GetCategory(void)
{
	switch(m_subtype)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		return wxString(_("Geography"));
	default:
		return wxString(_("[No Category]"));
	}
}

bool wxGISCartoMainTool::GetChecked(void)
{

	return m_bCheck;
}

bool wxGISCartoMainTool::GetEnabled(void)
{
	if(!m_pMapView)
	{
		WINDOWARRAY* pWinArr = m_pApp->GetChildWindows();
		if(pWinArr)
		{
			for(size_t i = 0; i < pWinArr->size(); i++)
			{
				wxGISMapView* pMapView = dynamic_cast<wxGISMapView*>(pWinArr->at(i));
				if (m_pMapView)
				{
					m_pMapView = pMapView;
					break;
				}
			}
		}
	}

	switch(m_subtype)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		return m_pMapView->IsShown();
	default:
		return false;
	}
}

wxGISEnumCommandKind wxGISCartoMainTool::GetKind(void)
{
	switch(m_subtype)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		return enumGISCommandCheck;
	default:
		return enumGISCommandNormal;
	}
}

wxString wxGISCartoMainTool::GetMessage(void)
{
	switch(m_subtype)
	{
	case 0:
		return wxString(_("Zoom map in"));
	case 1:
		return wxString(_("Zoom map out"));
	case 2:
		return wxString(_("Pan map"));
	case 3:
		return wxString(_("Feature information"));
	default:
		return wxString();
	}
}