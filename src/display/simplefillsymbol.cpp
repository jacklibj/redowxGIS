#include "wxgis/display/simplefillsymbol.h"

wxSimpleFillSymbol::wxSimpleFillSymbol(void)
{
	srand ( time(NULL) );
	int random_number1 = (rand() % 50);
	int random_number2 = (rand() % 50);
	int random_number3 = (rand() % 50);
	//srand ( time(NULL) );
	//
    //
	m_Brush.SetColour(205 + random_number1, 205 + random_number2, 205 + random_number3);
	m_Brush.SetStyle(wxSOLID);
	m_Pen.SetColour(128, 128, 128);
	m_Pen.SetCap(wxCAP_ROUND);
	m_Pen.SetStyle(wxSOLID);
	m_Pen.SetWidth(1);

	m_Font = wxNullFont;
}

wxSimpleFillSymbol::wxSimpleFillSymbol(wxPen, wxBrush Brush)
{
	m_Pen = Pen;
	m_Brush = Brush;
	m_Font = wxNullFont;
}

wxSimpleFillSymbol::~wxSimpleFillSymbol(void)
{

}
void wxSimpleFillSymbol::Draw(OGRGeometry* pGeometry, IDisplay* pwxGISDisplay)
{
	OGRwkbGeometryType type = wkbFlatten(pGeometry->getGeometryType());
	switch(type)
	{
	case wkbPolygon:
		pwxGISDisplay->SetBrush(m_Brush);
		pwxGISDisplay->SetPen(m_Pen);
		DrawPolygon((OGRPolygon*)pGeometry, pwxGISDisplay);
		break;
	case wkbMultiPolygon:
		pwxGISDisplay->SetBrush(m_Brush);
		pwxGISDisplay->SetPen(m_Pen);
		DrawPolyPolygon((OGRMultiPolygon*)pGeometry, pwxGISDisplay);
		break;
	case wkbPoint:
	case wkbLineString:
	case wkbLinearRing:
	case wkbMultiPoint:
	case wkbMultiLineString:
	case wkbGeometryCollection:
	case wkbUnknown:
	case wkbNone:
	default:
		break;
	}
}

void wxSimpleFillSymbol::DrawPolygon(OGRPolygon* pPoly, IDisplay* pwxGISDisplay)
{
	IDisplayTransformation* pDisplayTransformation = pwxGISDisplay->GetDisplayTransformation();

	int NumInteriorRings = pPoly->getNumInteriorRings();
	OGRLinearRing *pRing = pPoly->getExteriorRing();
	OGRLineString *pLStr = (OGRLineString*)pRing;
	int nPointCount = pLStr->getNumPoints();
	if (nPointCount > 2)
	{
		if (NumInteriorRings == 0)
		{
			OGRRawPoint* pOGRRawPoints = new OGRRawPoint[nPointCount];
			pLStr->getPoint(OGRRawPoint);
			wxPoint* pPoints = pDisplayTransformation->TransformCoordWorld2DC(pOGRRawPoints, nPointCount);
			delete[](pOGRRawPoints);
			pwxGISDisplay->DrawPolygon(nPointCount, pPoints);
			delete[](pPoints);
		}
		else
		{
			int *nN = new int[NumInteriorRings + 1];
			nN[0] = nPointCount;
			int jPoint(nPointCount);
			for (int iPart = 0; iPart < NumInteriorRings; iPart++)
			{
				pRing = pPoly->getInteriorRing(iPart);
				OGRLineString *pLStrInt = (OGRLineString*)pRing;
				nPointCount = pLStrInt->getNumPoints();
				jPoint += (nN[iPart + 1] = nPointCount > 2 ? nPointCount : 0);
			}

			wxPoint *pFullPoints = new wxPoint[jPoint];
			OGRRawPoint* pOGRRawPoints = new OGRRawPoint[nN[0]];
			pLStr->getNumPoints(pOGRRawPoints);
			int counter(0);
			pDisplayTransformation->TransformCoordWorld2DC(pOGRRawPoints, nN[0], &pFullPoints[counter]);
			delete[](pOGRRawPoints);
			counter += nN[0];

			for (int iPart = 0; iPart < NumInteriorRings; iPart++)
			{
				pRing = pPoly->getInteriorRing(iPart);
				OGRLineString *pLStrInt = (OGRLineString*)pRing;
				pOGRRawPoints = new OGRRawPoint[nN[iPart + 1]];
				pLStrInt->getPoints(pOGRRawPoints);
				pDisplayTransformation->TransformCoordWorld2DC(pOGRRawPoints, nN[iPart + 1], &pFullPoints[nPointCount]);
				delete[](pOGRRawPoints);
				counter += nN[iPart + 1];
			}

			pwxGISDisplay->DrawPolyPolygon(NumInteriorRings + 1, nN, pFullPoints, 0, 0, wxODDEVEN_RULE);

			delete[](pFullPoints);
			delete[](nN);
		}

	}
}

void wxSimpleFillSymbol::DrawPolyPolygon(OGRMultiPolygon* pPoly, IDisplay* pwxGISDisplay)
{
	IDisplayTransformation* pDisplayTransformation = pwxGISDisplay->GetDisplayTransformation();
	long nNumPolys(0);
	OGRGeometryCollection* pOGRGeometryCollection = (OGRGeometryCollection*)pPoly;
	for (int i = 0; i < pOGRGeometryCollection->getNumGeometries(); i++)
	{
		OGRPolygon* pPolygon = (OGRPolygon*)pOGRGeometryCollection->getGeometryRef(i);
		nNumPolys += pPolygon->getNumInteriorRings() + 1;
	}

	int *nN = new int[nNumPolys];
	long counter(0);
	long point_count(0);
	for (int i = 0; i < pOGRGeometryCollection->getNumGeometries(); i++)
	{
		OGRPolygon* pPolygon = (OGRPolygon*)pOGRGeometryCollection->getGeometryRef(i);
		OGRLinearRing *pRing = pPolygon->getExteriorRing();
		OGRLineString *pLStr = (OGRLineString*)pRing;
		nN[counter] = pLStr->getNumPoints();
		point_count += nN[counter];
		counter++;
		int NumInteriorRings = pPolygon->getNumInteriorRings();
		for (int iPart = 0; iPart < NumInteriorRings; iPart++)
		{
			pRing = pPolygon->getInteriorRing(iPart);
			OGRLineString *pLStrInt = (OGRLineString*)pRing;
			nN[counter] = pLStrInt->getNumPoints();
			point_count += nN[counter];
			counter++;
		}
	}

	wxPoint *pFullPoints = new wxPoint[point_count];

	counter = 0;
	long pos = 0;
	for (int i = 0; i< pOGRGeometryCollection->getNumGeometries(); i++)
	{
		OGRPolygon* pPolygon = (OGRPolygon*)pOGRGeometryCollection->getGeometryRef(i);
		OGRLinearRing *pRing = pPolygon->getExteriorRing();
		OGRLineString *pLStr = (OGRLineString*)pRing;

		OGRRawPoint* pOGRRawPoints = new OGRRawPoint[nN[counter]];
		pLStr->getPoint(pOGRRawPoints);
		pDisplayTransformation->TransformCoordWorld2DC(pOGRRawPoints, nN[counter], &pFullPoints[pos]);
		pos += nN[counter];
		counter++;
		int NumInteriorRings = pPolygon->getNumInteriorRings();
		for (int iPart = 0; iPart < NumInteriorRings; iPart++ )
		{
			pRing = pPolygon->getInteriorRing(iPart);
			OGRLineString *pLstrInt = (OGRLineString*)pRing;
			pOGRRawPoints->getPoints(pOGRRawPoints);
			pDisplayTransformation->TransformCoordWorld2DC(pOGRRawPoints, nN[counter], &pFullPoints[pos]);
			pos += nN[counter];
			delete[](pOGRRawPoints);
			counter++;
		}
	}

	pwxGISDisplay->DrawPolyPolygon(nNumPolys, nN, pFullPoints, 0, 0, wxODDEVEN_RULE);
	delete[](pFullPoints);
	delete[](nN);
}