#include "wxgis/display/simplelinesymbol.h"

wxSimpleLineSymbol::wxSimpleLineSymbol(void)
{
	srand ( time(NULL) );
	int random_number1 = (rand() % 50);
	int random_number2 = (rand() % 50);
	int random_number3 = (rand() % 50);
	m_Pen.SetColour(105 + random_number1, 105 + random_number2, 105 + random_number3);
	m_Pen.SetCap(wxCAP_ROUND);
	m_Pen.SetStyle(wxSOLID);
	m_Pen.SetWidth(1);

	m_Brush = wxNullBrush;
	m_Font = wxNullFont;
}

wxSimpleLineSymbol::wxSimpleLineSymbol(wxPen Pen)
{
	m_Pen = Pen;
	m_Brush = wxNullBrush;
	m_Font = wxNullFont;
}

wxSimpleLineSymbol::~wxSimpleLineSymbol(void)
{

}

void wxSimpleLineSymbol::Draw(OGRGeometry* pGeometry, IDisplay* pwxGISDisplay)
{
	OGRwkbGeometryType type = wkbFlatten(pGeometry->getGeometryType());
	switch(type)
	{
	case wkbLineString:
	case wkbLinearRing:
		pwxGISDisplay->SetPen(m_Pen);
		DrawLine((OGRLineString*)pGeometry, pwxGISDisplay);
		break;
	case wkbMultiLineString:
		pwxGISDisplay->SetPen(m_Pen);
		DrawPolyLine((OGRMultiLineString*)pGeometry, pwxGISDisplay);
		break;
	case wkbPoint:
	case wkbMultiPoint:
	case wkbPolygon:
	case wkbMultiPolygon:
	case wkbGeometryCollection:
	case wkbUnknown:
	case wkbNone:
	default:
		break;
	}
}

void wxSimpleLineSymbol::DrawLine(OGRLineString* pLine, IDisplay* pwxGISDisplay)
{
	IDisplayTransformation* pDisplayTransformation = pwxGISDisplay->GetDisplayTransformation();

	int nPointCount = pLine->getNumPoints();
	if (nPointCount > 2)
	{
		OGRRawPoint* pOGRRawPoints = new OGRRawPoint[nPointCount];
		pLine->getPoint(pOGRRawPoints);
		wxPoint* pPoints = pDisplayTransformation->TransformCoordWorld2DC(pOGRRawPoints, nPointCount);
		wxDELETEA(pOGRRawPoints);
		pwxGISDisplay->DrawLines(nPointCount, pPoints);
		wxDELETEA(pPoints);
	}
}

void wxSimpleLineSymbol::DrawPolyLine(OGRMultiLineString* pLine, IDisplay* pwxGISDisplay)
{
	IDisplayTransformation* pDisplayTransformation = pwxGISDisplay->GetDisplayTransformation();
	OGRGeometryCollection* pOGRGeometryCollection = (OGRGeometryCollection*)pLine;
	for (int i = 0; i < pOGRGeometryCollection->getNumGeometries(); i++)
	{
		OGRLineString* pLineString = (OGRLineString*)pOGRGeometryCollection->getGeometryRef(i);
		DrawLine(pLineString, pwxGISDisplay);
	}
}