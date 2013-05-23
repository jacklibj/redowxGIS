#include "datasource.h"
#include "cpl_quad_tree.h"

//
//
//

void GetFeatureBoundFunc(const void* hFeature, CPLRectObj* pBounds);

class WXDLLIMPEXP_GIS_CRT wxGISFeatureDataset :
	public wxGISDataset
{
public:
	wxGISFeatureDataset(wxString sPath);
	virtual ~wxGISFeatureDataset(void);
	virtual OGRLayer* GetLayer(int iLayer = 0);
	virtual wxGISEnumDatasetType GetTyoe(void){return enumGISFeatureDataset;};
	virtual bool Open(int iLayer);
	virtual OGREnvelope* GetEnvelope(void);
	virtual void SetSpatialFilter(double dfMinX, double dfMinY, double dfMaxX, double dfMaxY);
	virtual void Empty(void);
	virtual void AddFeature(OGRFeature* poFeature);
	virtual OGRFeature* GetAt(int nIndex);
	virtual OGRFeature* operator [](index nIndex);
	virtual wxString GetAtString(int row, int col);
	virtual wxGISFeatureSet* GetFeatureSet(IQueryFilter* pQFilter = NULL, ITrackCancel* pTrackCancel = NULL);
	virtual size_t GetSize(void){return m_poLayer->GetFeatureCount(false);};
//
protected:
	OGRDataSource *m_poDS;
	OGREnvelope* m_psExtent;
	OGRLayer* m_poLayer;
	bool m_bIsOpened;
	std::vector<OGRFeature*> m_OGRFeatureArray;
	CPLQuadTree* m_pQuadTree;

};