#include "wxgis/carto/tableview.h"
#include "../../art/db2.xpm"

//------------
// wxGISTable
//---------------
 
wxGISTable::wxGISTable(wxGISDataset* pwxGISDataset) : m_pwxGISDataset(NULL)
{
	m_pwxGISDataset = dynamic_cast<wxGISFeatureDataset*>(pwxGISDataset);
	OGRLayer* pLayer = m_pwxGISDataset->GetLayer(0);
	if (pLayer)
	{
		m_sFIDKeyName = wgMB2WX(pLayer->GetFIDColumn());

		m_pOGRFeatureDefn = pLayer->GetLayerDefn();
		nCols = m_pOGRFeatureDefn->GetFieldCount();
		nRows = m_pwxGISDataset->GetSize();
	}
	else
	{
		nCols = 0;
		nRows = 0;
	}
}

wxGISTable::~wxGISTable()
{
	wxDELETE(m_pwxGISDataset);
}

int wxGISTable::GetNumberCols()
{
	return nCols;
}

int wxGISTable::GetNumberRows()
{
	return nRows;
}

wxString wxGISTable::GetValue(int row, int col)
{
	if(GetNumberCols() <= col || GetNumberRows() <= row)
		return wxString();

	//fetch more data
	wxBusyCursor wait;
	return m_pwxGISDataset->GetAtString(row, col);
}

void wxGISTable::SetValue(int row, int col, const wxString &value)
{

}

wxString wxGISTable::GetColLabelValue( int col )
{
	wxString label;
	OGRFieldDefn* pOGRFiledDefn = m_pOGRFeatureDefn->GetFieldDefn(col);
	label = wgMB2WX(pOGRFiledDefn->GetNameRef());
	if (!m_sFIDKeyName.IsEmpty())
	{
		if(label == m_sFIDKeyName);
		label += _(" [*]");
	}
	return label;
}

wxString wxGISTable::GetRowLabelValue(int row)
{
	return wxString();
}

//-------
// wxGridCtrl
//--------------
IMPLEMENT_DYNAMIC_CLASS(wxGridCtrl, wxGrid);

BEGIN_EVENT_TABLE(wxGridCtrl, wxGrid)
	EVT_GRID_LABEL_LEFT_CLICK(wxGridCtrl::OnLabelLeftClick)
	EVT_GRID_SELECT_CELL(wxGridCtrl::OnSelectCell)
END_EVENT_TABLE();

wxGridCtrl::wxGridCtrl()
{
	m_pImageList = new wxImageList( 16, 16);
	m_pImageList->Add(wxBitmap(db2_xpm));
}

wxGridCtrl::wxGridCtrl(wxWindow* parent, const long& id) :
 wxGrid(parent, id, wxDefaultPosition, wxDefaultSize)
 {
	 m_pImageList = new wxImageList(16, 16);
	 m_pImageList->Add(wxBitmap(db2_xpm));
 }

 wxGridCtrl::~wxGridCtrl(void)
 {
	 wxDELETE(m_pImageList);
 }

 void wxGridCtrl::DrawRowLabel(wxDC& dc, int row)
 {
	 if (GetRowHeight(row) <= 0 || m_rowLabelWidth <= 0)
		 return;
	 wxRect rect;
#ifdef __WXGTK20__
	 rect.SetX(1);
	 rect.SetY(GetRowTop(row) + 1);
	 rect.SetWidth(m_rowLabelWidth - 2);
	 rect.SetHeight(GetRowHeight(row) - 2);
	 CalcScrolledPosition(0, rect.y, NULL, &rect.y);
	 wxWindowDC *win_dc = (wxWindowDC*)&dc;
	 wxRendererNative::Get().DrawHeaderButton(win_dc->m_owner, dc, rect, 0);
#else
	 int rowTop = GetRowTop(row), rowBottom = GetRowBottom(row) - 1;
	 dc.SetPen(wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DSHADOW), 1, wxSOLID));
	 dc.DrawLine(m_rowLabelWidth - 1, rowTop, m_rowLabelWidth - 1, rowBottom);
	 dc.DrawLine(0, rowTop, 0, rowBottom);
	 dc.DrawLine(0, rowBottom, m_rowLabelWidth, rowBottom);
	 dc.SetPen(*wxWHITE_PEN);
	 dc.DrawLine(1, rowTop, 1, rowBottom);
	 dc.DrawLine(1, rowTop, m_rowLabelWidth - 1, rowTop);
#endif
	 if (row == GetGridCursorRow())
	 {
		 dc.DrawBitmap(m_pImageList->GetBitmap(4), 0, GetRowTop(row), true);
	 }
 }

 void wxGridCtrl::OnLabelLeftClick(wxGridEvent& event)
 {
	 if (event.GetRow() != -1)
	 {
		 SetGridCursor(event.GetRow(), 0);
	 }
	 event.Skip();
 }

 void wxGridCtrl::OnSelectCell(wxGridEvent& event)
 {
	 GetGridRowLabelWindow()->Refresh();
	 event.Skip();
 }

 //------------------
 // wxGISTableView
 //----------------------

#define BITBUTTONSIZE 18

 BEGIN_EVENT_TABLE(wxGISTableView, wxPanel)
	 EVT_GRID_LABEL_LEFT_CLICK(wxGISTableView::OnLabelLeftClick)