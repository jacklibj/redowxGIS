#pragma once

#include "wxgis/display/display.h"

class wxGISDisplayTransformation :
	public IDisplayTransformation
{
public:
	wxGISDisplayTransformation(void);
	virtual ~wxGISDisplayTransformation(void);
	virtual void Reset(void);
	virtual void SetDeviceFrame(wxRect rc);
	virtual wxRect GetDeviceFrame(void);
	virtual bool IsBoundsSet(void);
	virtual void SetBounds(OGREnvelope bounds);
	virtual OGREnvelope GetBounds(void);
	virtual OGREnvelope GetVisibleBounds(void);
	virtual double GetRatio(void);
	virtual double GetScaleRatio(void);
	virtual void SetSpatialReference(OGRSpatialReference* pSpatialReference);
	virtual OGRSpatialReference* GetSpatialReference(void);
	virtual wxPoint* TransformCoordWorld2DC(OGRRawPoint* pPoints, size_t nPointCount);
	virtual void TransformCoordWorldDC2World(wxPoint* pPoints, size_t nPointCount, wxPoint* pResult);
	virtual OGRRawPoint* TransformCoordDC2World(wxPoint* pPoints, size_t nPointCount);
	virtual void SetPPI(wxSize new_res);
	virtual OGREnvelope TransfromRect(wxRect rect);
protected:
	//World2DC
	double xWorld2DC(double x);
	double yWorld2DC(double y);
	// DC2World
	double xDC2World(int x);
	double yDC2World(int y);
protected:
	double m_World2DC, m_DC2World, m_DCXDelta, m_DCYDelta, m_WolrdXDelta, m_WorldYDelta;
	wxRect m_DeviceFrameRect;
	OGREnvelope m_Bounds;
	OGRSpatialReference* m_pSpatialReference;
	bool m_bIsBoundsSet;
	wxSize m_ppi;
};