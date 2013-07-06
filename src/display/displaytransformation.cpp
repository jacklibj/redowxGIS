/******************************************************************************
 * Project:  wxGIS (GIS Catalog)
 * Purpose:  display transformation. Transform from world to screen coordinates and vice versa
 * Author:   Bishop (aka Barishnikov Dmitriy), polimax@mail.ru
 ******************************************************************************
*   Copyright (C) 2009  Bishop
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include "wxgis/display/displaytransformation.h"

wxGISDisplayTransformation::wxGISDisplayTransformation(void)
{
	m_DeviceFrameRect.x = 0;
	m_DeviceFrameRect.y = 0;
	m_DeviceFrameRect.width = 800;
	m_DeviceFrameRect.height = 600;
	Reset();
}

wxGISDisplayTransformation::~wxGISDisplayTransformation(void)
{
}

void wxGISDisplayTransformation::Reset(void)
{
	m_Bounds.MaxX = 84;
	m_Bounds.MinX = -6;
	m_Bounds.MaxY = 80;
	m_Bounds.MinY = -80;

	m_pSpatialReference = NULL;
	m_bIsBoundsSet = false;
}

void wxGISDisplayTransformation::SetDeviceFrame(wxRect rc)
{
	m_DeviceFrameRect = rc;
	m_WorldXDelta = (m_Bounds.MaxX - m_Bounds.MinX);
	m_WorldYDelta = (m_Bounds.MaxY - m_Bounds.MinY);
	double sc1 = fabs((double)m_DeviceFrameRect.GetWidth() / m_WorldXDelta);
	double sc2 = fabs((double)m_DeviceFrameRect.GetHeight() / m_WorldYDelta);
	m_WorldXDelta /= 2;
	m_WorldYDelta /= 2;
	m_DCXDelta = m_DeviceFrameRect.GetWidth() / 2;
	m_DCYDelta = m_DeviceFrameRect.GetHeight() / 2;
	m_World2DC = MIN(sc1, sc2);
}

wxRect wxGISDisplayTransformation::GetDeviceFrame(void)
{
	return m_DeviceFrameRect;
}

bool wxGISDisplayTransformation::IsBoundsSet(void)
{
	return m_bIsBoundsSet;
}

void wxGISDisplayTransformation::SetBounds(OGREnvelope bounds)
{
	m_Bounds = bounds;
	m_WorldXDelta = (m_Bounds.MaxX - m_Bounds.MinX);
	m_WorldYDelta = (m_Bounds.MaxY - m_Bounds.MinY);
	double sc1 = fabs((double)m_DeviceFrameRect.GetWidth() / m_WorldXDelta);
	double sc2 = fabs((double)m_DeviceFrameRect.GetHeight() / m_WorldYDelta);
	m_WorldXDelta /= 2;
	m_WorldYDelta /= 2;
	m_World2DC = MIN(sc1, sc2);
	m_bIsBoundsSet = true;
}

OGREnvelope wxGISDisplayTransformation::GetBounds(void)
{
	return m_Bounds;
}

OGREnvelope wxGISDisplayTransformation::GetVisibleBounds(void)
{
	return TransformRect(m_DeviceFrameRect);
}

double wxGISDisplayTransformation::GetRatio(void)
{
	return m_World2DC;
}

double wxGISDisplayTransformation::GetScaleRatio(void)
{
	double screen_w = (double)m_DeviceFrameRect.GetWidth() / m_ppi.GetWidth() * 2.54; // width in cm!!!
	double screen_h = (double)m_DeviceFrameRect.GetHeight() / m_ppi.GetHeight() * 2.54; //width in cm!!
	OGREnvelope VisBounds = GetVisibleBounds();

	double w_w = fabs(VisBounds.MaxX - VisBounds.MinX);
	double w_h = fabs(VisBounds.MaxY - VisBounds.MinY);
	if (m_pSpatialReference && m_pSpatialReference->IsGeographic())
	{
		w_w = w_w * PIDEG * m_pSpatialReference->GetSemiMajor();
		w_h = w_h * PIDEG * m_pSpatialReference->GetSemiMinor();
	}

	double screen = MIN(screen_w, screen_h);
	double world = MIN(w_w, w_h);

	return (world * 100) / screen;
}

void wxGISDisplayTransformation::SetSpatialReference(OGRSpatialReference* pSpatialReference)
{
	m_pSpatialReference = pSpatialReference;
}

OGRSpatialReference* wxGISDisplayTransformation::GetSpatialReference(void)
{
	return m_pSpatialReference;
}

wxPoint* wxGISDisplayTransformation::TransformCoordWorld2DC(OGRRawPoint* pPoints, size_t nPointCount)
{
	wxPoint* pResult = new wxPoint[nPointCount];
	for (size_t i = 0; i < nPointCount; i++ )
	{
		pResult[i].x = (int)xWorld2DC(pPoints[i].x);
		pResult[i].y = (int)yWorld2DC(pPoints[i].y);
	}
	return pResult;
}

void wxGISDisplayTransformation::TransformCoordWorld2DC(OGRRawPoint* pPoints, size_t nPointCount, wxPoint* pResult)
{
	if(pResult == NULL)
		return;
	for (size_t i = 0; i < nPointCount; i++)
	{
		pResult[i].x = (int)xWorld2DC(pPoints[i].x);
		pResult[i].y = (int)yWorld2DC(pPoints[i].y);
 	}
}

OGRRawPoint* wxGISDisplayTransformation::TransformCoordDC2World(wxPoint* pPoints, size_t nPointCount)
{
	OGRRawPoint* pResult = new OGRRawPoint[nPointCount];
	for(size_t i = 0; i < nPointCount; i++)
	{
		pResult[i].x = xDC2World(pPoints[i].x);
		pResult[i].y = yDC2World(pPoints[i].y);
	}
	return pResult;
}

void wxGISDisplayTransformation::SetPPI(wxSize new_res)
{
	m_ppi = new_res;
}

double wxGISDisplayTransformation::xWorld2DC(double x)
{
	return m_DCXDelta - (m_WorldXDelta - x + m_Bounds.MinX) * m_World2DC;
}

double wxGISDisplayTransformation::yWorld2DC(double y)
{
	return m_DCYDelta + (m_WorldYDelta - y + m_Bounds.MinY) * m_World2DC;
}

double wxGISDisplayTransformation::xDC2World(int x)
{
	return m_WorldXDelta + m_Bounds.MinX + double(x - m_DCXDelta) / m_World2DC;
}

double wxGISDisplayTransformation::yDC2World(int y)
{
	return m_WorldYDelta + m_Bounds.MinY - double(y - m_DCYDelta) / m_World2DC;
}

OGREnvelope wxGISDisplayTransformation::TransformRect(wxRect rect)
{
	OGREnvelope ResEnv;
	wxPoint LTPoint = rect.GetLeftTop();
	wxPoint RBPoint = rect.GetRightBottom();
	ResEnv.MaxX = xDC2World(RBPoint.x);
	ResEnv.MinX = xDC2World(LTPoint.x);
	ResEnv.MaxY = yDC2World(LTPoint.y);
	ResEnv.MinY = yDC2World(RBPoint.y);
	return ResEnv;
}