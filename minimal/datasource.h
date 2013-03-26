#pragma once

#include "ogrsf_frmts.h"

#include "gdal_priv.h"
#include "gdal_alg_priv.h"

#include "wxgis\framework\framework.h"
#include "wx\datetime.h"

enum wxGISEnumDatasetType
{
	enumGISFeatureDataset = 1,
    enumGISTableDataset = 2,
	enumGISRasterDataset = 4
};

enum wxGISEnumShapefileDatasetType
{
	enumESRIShapefile = 1,
	enumMapinfoTablefile =2
};

enum wxGISEnumRasterType
{
	enumRasterUnknow = 0
};


//this class should be in wxGISGeodatabase
class wxGISDataset :
	public IPointer
{
	wxGISDataset(wxString sPath) : IPointer() , m_sPath(sPath){};
	virtual ~wxGISDataset(void){};
	virtual wxGISEnumDatasetType GetType(void) = 0;
	virtual wxString GetPath(void){return m_sPath;};
protected:
	wxString m_sPath;
	wxCriticalSection m_CritSect;

};

class wxGISFeatureSet
{
public:
	wxGISFeatureSet(void)
	{
		m_OGRFeatureArray.reserve(1000);
	}

	wxGISFeatureSet(size_t nReserve)
	{
		m_OGRFeatureArray.reserve(nReserve);
	}
	virtual ~wxGISFeatureSet(void){};
	virtual void AddFeature(OGRFeature* poFeature){m_OGRFeatureArray.push_back(poFeature);};
	virtual size_t GetSize(void){return m_OGRFeatureArray.size();};
	virtual OGRFeature* GetAt(int nIndex) //
	{
		wxASSERT(nIndex >= 0);
		//
		return m_OGRFeatureArray[nIndex];
	}
	virtual bool IsEmpty(void){return m_OGRFeatureArray.empty();};
	virtual OGRFeature* operator [](int nIndex)//const same as GetAt
	{
		return GetAt(nIndex);
	}
protected:
	std::vector<OGRFeature*> m_OGRFeatureArray;
};

class IQueryFilter
{
public:
	virtual ~IQueryFilter(void){};
};

class wxGISSpatialFilter : public IQueryFilter
{
	virtual ~wxGISSpatialFilter(void){};
	virtual void SetEnvelope(double dfMinX, double dfMinY, double dfMaxX, double dfMaxY)
	{
		m_Env.MinX = dfMinX;
		m_Env.MaxX = dfMaxY;
		m_Env.MinY = dfMinY;
		m_Env.MaxY = dfMaxY;
	}

	virtual void SetEnvelope(OGREnvelope Env)
	{
		m_Env = Env;
	}
protected:
	OGREnvelope m_Env;
};