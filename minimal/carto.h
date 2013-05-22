#pragma once

#include "display.h"
#include "datasource.h"

enum wxGISEnumMapToolState
{
	enumGISMapNone = 0x0000,
	enumGISMapPanning = 0x0001,
	enumGISMapZooming = 0x0002,
	enumGISMapRotating = 0x0004,
};

class wxGISLayer
{
public:
	wxGISLayer(void)
	{
		m_sName = wxString(_("new layer"));
		m_dMaxScale = -1;
		m_dMinScale = -1;
		m_bVisible = true;
		m_bCached = false;
	}
	virtual ~wxGISLayer(void){};
	//pure virtual
	virtual OGRSpatial
};