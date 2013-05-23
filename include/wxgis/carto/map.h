#pragma once

#include "carto.h"

class WXDLLIMPEXP_GIS_CRT wxGISMap
{
public:
	wxGISMap(void);
	virtual ~wxGISMap(void);
	virtual void SetName(wxString sName){m_sMapName = sName;};
	virtual wxString GetName(void){return m_sMapName;};
	virtual void AddLayer(wxGISLayer* pLayer);
	virtual void ClearLayers(void);
	virtual size_t GetLayerCount(void){return m_Layers.size();};
	virtual wxString GetDescription(void){return m_sDescription;};
	virtual void SetDescription(wxString sDescription){m_sDescription = sDescription;}
	virtual OGREnvelope GetFullExtent(void);
protected:
	wxString m_sMapName, m_sDescription;
	std::vector<wxGISLayer*> m_Layers;
};