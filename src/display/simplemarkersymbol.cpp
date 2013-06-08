#include "wxgis/display/simplemarkersymbol.h"

wxSimpleMarkerSymbol::wxSimpleMarkerSymbol(void)
{
	m_Brush.SetColour(168, 0, 132);
	m_Brush.SetStyle(wxSOLID);

	m_Pen.SetColour(128, 128, 128);
	m_Pen.SetCap(
		wxCAP_ROUND);
	m_Pen.SetStyle(wxSOLID);
	m_Pen.SetWidth(1)

	m_size = 4;
}

wxSimpleMarkerSymbol::~wxSimpleMarkerSymbol(void)
{

}

void wxSimpleMarkerSymbol::Draw(OGRGeometry* pGeometry, IDisplay* pwxGISDisplay)
{
	IDisplayTransformation* pDisplayTransformation = pwxGISDisplay->GetDisplayTransformation();
	OGRwkbGeometryType type = wkbFlatten(pGeometry->getGeometryType());
	switch(type)
	{
	case wkbPoint:
		{
			OGRPoint *pPoint = (OGRPoint*)pGeometry;
			OGRPoint Point;
			Point.x = pPoint->getX();
			Point.y = pPoint->getY();
			wxPoint* pPoints = pDisplayTransformation->TransformCoordWorld2DC(&Point, 1);
			pwxGISDisplay->SetBrush(m_Brush);
			pwxGISDisplay->SetPen(m_Pen);
			pwxGISDisplay->DrawCircle(pPoints[0], pPoints[0].y, m_Size);
			delete[](pPoints);
		}
		break;
	case wkbPolygon:
	case wkbMultiPolygon:
	case wkbLineString:
	case wkbLinearRing:
	case wkbMultiPoint:
	case wkbMultiLineString:
	case wkbGeometryCollection:
		{
			OGREnvelope sEnvelope;
			pGeometry->getEnvelope(&sEnvelope);
			//
			//
			Point.X = sEnvelope.MinX;
			Point.Y = sEnvelope.MinY;
			wxPoint* pPoints = pDisplayTransformation->TransformCoordWorld2DC(&Point, 1);
			pwxGISDisplay->SetBrush(m_Brush);
			pwxGISDisplay->SetPen(m_Pen);
			pwxGISDisplay->DrawPoint(pPoints[0].x, pPoints[0].y);
			delete[](pPoints);
		}
		break;
	case wkbUnknown:
	case wkbNone:
	default:
		break;
	}
}