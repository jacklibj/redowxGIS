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
		return wxString();
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

void wxGISCartoMainTool::OnClick(void)
{
	switch(m_subtype)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		break;
	default:
		break;
	}
}

bool wxGISCartoMainTool::OnCreate(IApplication* pApp)
{
	m_pApp = pApp;
	return true;
}

wxString wxGISCartoMainTool::GetTooltip(void)
{
	switch(m_subtype)
	{
	case 0:
		return wxString(_("Zoom In"));
	case 1:
		return wxString(_("Zoom Out"));
	case 2:
		return wxString(_("Pan"));
	case 3:
		return wxString(_("Information"));
	default:
		return wxString();
	}
}

unsigned char wxGISCartoMainTool::GetCount(void)
{
	return 4;
}

wxCursor wxGISCartoMainTool::GetCursor(void)
{
	switch(m_subtype)
	{
	case 0: // zoom in
		{
			wxImage CursorImage = m_ImageList.GetBitmap(13).ConvertToImage();
			CursorImage.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 6);
			CursorImage.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 6);
			return wxCursor(CursorImage);
		}
	case 1: // zoom out
		{
			wxImage CursorImage = m_ImageList.GetBitmap(14).ConvertToImage();
			CursorImage.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 6);
			CursorImage.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 6);
			return wxCursor(CursorImage);
		}
	case 2: // pan
		{
			wxImage CursorImage = m_ImageList.GetBitmap(1).ConvertToImage();
			CursorImage.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 6);
			CursorImage.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 6);
			return wxCursor(CursorImage); //
		}
	case 3: // info
		{
			wxImage CursorImage = m_ImageList.GetBitmap(12).ConvertToImage();
			CursorImage.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 6);
			CursorImage.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 6);
			return wxCursor(CursorImage); //
		}
	default:
		return wxNullCursor;
	}
}

void wxGISCartoMainTool::SetChecked(bool bCheck)
{
	m_bCheck = bCheck;
	if (m_bCheck)
	{
		m_pMapView->SetCursor(GetCursor());
	}
}

void wxGISCartoMainTool::OnMouseDown(wxMouseEvent& event)
{
	switch(m_subtype)
	{
	case 0:
		{


			wxGISRubberEnvelope RubberEnvelope;
			wxPen Pen(wxColor(0, 0, 255, 100), 2);
			Pen.SetCap(wxCAP_BUTT);
			wxBrush Brush(wxColour(0, 0, 255, 20));
			wxSimpleFillSymbol symbol(Pen, Brush);
			OGRGeometry* pGeom = RubberEnvelope.TrackNew(event.GetX(), event.GetY(), m_pMapView, m_pMapView->GetCachedDisplay(), &symbol);
			if (pGeom)
			{
				OGREnvelope Env;
				pGeom->getEnvelope(&Env);
				if (Env.MaxX == Env.MinX || Env.MaxY == Env.MinY)
				{
					OGREnvelope CurrentEnv = m_pMapView->GetCachedDisplay()->GetDisplayTransformation()->GetBounds();
					int widthdiv4 = (CurrentEnv.MaxX - CurrentEnv.MinX) / 4;
					int heightdiv4 = (CurrentEnv.MaxY - CurrentEnv.MinY) / 4;

					Env.MinX -= widthdiv4;
					Env.MinY -= heightdiv4;
					Env.MaxX += widthdiv4;
					Env.MaxY += heightdiv4;
				}
				m_pMapView->SetExtent(Env);
			}
			wxDELETE(pGeom);
			break;
		}
	case 1:
		{
			wxGISRubberEnvelope RubberEnvelope;
			wxPen Pen(wxColor(0, 0, 255, 100), 2);
			Pen.SetCap(wxCAP_BUTT);
			wxBrush Brush(wxColour(0, 0, 255, 20));
			wxSimpleFillSymbol symbol(Pen, Brush);
			OGRGeometry* pGeom = RubberEnvelope.TrackNew(event.GetX(), event.GetY(), m_pMapView, m_pMapView->GetCachedDisplay(), &symbol);
			if (pGeom)
			{
				OGREnvelope Env;
				pGeom->getEnvelope(&Env);
				OGREnvelope CurrentEnv = m_pMapView->GetCachedDisplay()->GetDisplayTransformation()->GetBounds();
				OGREnvelope NewEnv;
				NewEnv.MinX = CurrentEnv.MinX + CurrentEnv.MinX - Env.MinX;
				NewEnv.MinY = CurrentEnv.MinY + CurrentEnv.MinY - Env.MinY;
				NewEnv.MaxX = CurrentEnv.MaxX + CurrentEnv.MaxX - Env.MaxX;
				NewEnv.MaxY = CurrentEnv.MaxY + CurrentEnv.MaxY - Env.MaxY;
				m_pMapView->SetExtent(Env);

			}
			wxDELETE(pGeom);
			break;
		}
	case 2: // pan
		{
			wxImage CursorImage = m_ImageList.GetBitmap(11).ConvertToImage();
			CursorImage.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 7);
			CursorImage.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 7);
			m_pMapView->SetCursor(wxCursor(CursorImage));
			m_pMapView->PanStart(event.GetPosition());
			break;
		}
	case 3: // info
		{
			wxGISRubberEnvelope RubberEnvelope;
			wxPen Pen(wxColor(0, 0, 255, 100), 2);
			Pen.SetCap(wxCAP_BUTT);
			wxBrush Brush(wxColour(0, 0, 255, 20));
			wxSimpleFillSymbol symbol(Pen, Brush);
			OGRGeometry* pGeom = RubberEnvelope.TrackNew(event.GetX(), event.GetY(), m_pMapView, m_pMapView->GetCachedDisplay(), &symbol);
			if (pGeom)
			{
				//
			}
			wxDELETE(pGeom);
			break;
		}
	default:
		break;

	}
}

void wxGISCartoMainTool::OnMouseUp(wxMouseEvent& event)
{
	switch(m_subtype)
	{
	case 0: // in
		break;
	case 1://out
		break;
	case 2: //pan
		m_pMapView->SetCursor(GetCursor());
		m_pMapView->PanStop(event.GetPosition());
		break;
	case 3://info
		break;
	default:
		break;
	}
}

void wxGISCartoMainTool::OnMouseMove(wxMouseEvent& event)
{
	switch(m_subtype)
	{
	case 0: // in
		break;
	case 1://out
		break;
	case 2: //pan
		if(event.Dragging())
			m_pMapView->PanMoveTo(event.GetPosition());
		break;
	case 3://info
		break;
	default:
		break;
	}
}

void wxGISCartoMainTool::OnMouseDoubleClick(wxMouseEvent& event)
{

}
