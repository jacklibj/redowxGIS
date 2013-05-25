#pragma once

#include "wxgis/display/display.h"
#include "wxgis/datasource.h"

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
	virtual OGRSpatialReference* GetSpatialReference(void) = 0;
	virtual OGREnvelope* GetEnvelop(void) = 0;
	virtual bool IsValid(void) = 0;
	virtual void Draw(wxGISEnumDrawPhase DrawPhase, ICachedDisplay* pDisplay, ITrackCancel* pTrackCancel) = 0;
	//
	virtual void SetMaximunScale(double dMaxScale){m_dMaxScale = dMaxScale;};
	virtual double GetMaximumScale(void){return  m_dMaxScale;};
	virtual void SetMinimunScale(double dMinScale){m_dMinScale = dMinScale;};
	virtual double GetMinimumScale(void){return m_dMinScale;};
	//
	virtual bool GetVisible(void) {return m_bVisible;};
	virtual void SetVisible(bool bVisible){m_bVisible = bVisible;};
	virtual void SetName(wxString sName) {m_sName = sName;};
	virtual wxString GetName(wxString sName){return m_sName;};
	virtual size_t GetCacheID(void){return m_iCacheID;};
	virtual void SetCacheID(size_t iCacheID){ m_iCacheID = iCacheID;}
	virtual bool GetCached(void){return m_bCached;};
	virtual void SetCacheID(bool bCached){m_bCached = bCached;};
protected:
	double m_dMaxScale, m_dMinScale;
	wxString m_sName;
	bool m_bVisible, m_bCached;
	size_t m_iCacheID;
};


class IRenderer
{
public:
	virtual ~IRenderer(void){}
	virtual bool CanRender(wxGISDataset* pDataset) = 0;
};

class IFeatureRenderer : public IRenderer
{
public:
	virtual ~IFeatureRenderer(void){};
	virtual void Draw(wxGISFeatureSet* pSet, wxGISEnumDrawPhase DrawPhase, IDisplay* pDisplay,ITrackCancel* pTrackCancel)=0;
};

class IRasterRenderer : public IRenderer
{
public:
	virtual ~IRasterRenderer(void){};
	virtual void Draw(wxGISDataset* pRasterDataset, wxGISEnumDrawPhase DrawPhase, IDisplay* pDisplay, ITrackCancel* pTrackCancel) = 0;
};