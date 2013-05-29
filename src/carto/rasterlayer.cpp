#include "wxgis/carto/rasterlayer.h"
#include "wxgis/carto/rasterrenderer.h"

wxGISRasterLayer::wxGISRasterLayer(wxGISDataset* pwxGISDataset) : wxGISLayer(), m_pwxGISRasterDataset(NULL), m_pRasterRenderer(NULL)
{
	m_pwxGISRasterDataset = dynamic_cast<wxGISRasterDataset*>(pwxGISDataset);
	if(m_pwxGISRasterDataset)
	{
		m_pwxGISRasterDataset->Reference();
		//
		m_pRasterRenderer = new wxGISRasterRGBRenderer();
	}
}

wxGISRasterLayer::~wxGISRasterLayer(void)
{
	wxDELETE(m_pRasterRenderer);
	wsDELETE(m_pwxGISRasterDataset);
}

void wxGISRasterLayer::Draw(wxGISEnumDrawPhase DrawPhase, ICachedDisplay* pDisplay, ITrackCancel* pTrackCancel)
{

}