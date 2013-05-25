#include "wxgis/carto/map.h"


wxGISMap::wxGISMap(void)
{
	m_sMapName = wxString(_("new map"));
}

wxGISMap::~wxGISMap(void)
{
	//
	ClearLayers();
}

//
void wxGISMap::AddLayer(wxGISLayer* pLayer)
{
	if(!pLayer)
		return;
	m_Layers.push_back(pLayer);
}

void wxGISMap::ClearLayers(void)
{
	for (size_t i = 0 ; i < m_Layers.size(); i++ )
		delete m_Layers[i];
	m_Layers.clear();
}

OGREnvelope wxGISMap::GetFullExtent(void)
{
	OGREnvelope res;
	for(size_t i = 0; i < m_Layers.size(); i++)
		//
		res.Merge(*m_Layers[i]->GetEnvelop());
	return res;
}
