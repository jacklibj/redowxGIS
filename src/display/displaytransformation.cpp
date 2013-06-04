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
	m_WorldYDelta /= 2;
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

}