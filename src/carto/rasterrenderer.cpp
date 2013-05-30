#include "wxgis/carto/rasterrenderer.h"
#include "wxgis/carto/rasterdataset.h"
#include "wxgis/display/screendisplay.h"

//-------
// wxGISRasterRGBRender
//-------

wxGISRasterRGBRenderer::wxGISRasterRGBRenderer(void)
{

}

wxGISRasterRGBRenderer::~wxGISRasterRGBRenderer(void)
{

}

bool wxGISRasterRGBRenderer::CanRender(wxGISDataset* pDataset)
{
	return pDataset->GetType() == enumGISRasterDataset ? true : false;
}

void wxGISRasterRGBRenderer::Draw(wxGISDataset* pRasterDataset, wxGISEnumDrawPhase DrawPhase, IDisplay* pDisplay, ITrackCancel* pTrackCancel)
{
	wxGISRasterDataset* pRaster = dynamic_cast<wxGISRasterDataset*>(pRasterDataset);
	if(!pRaster)
		return;
	IDisplayTransformation* pDisplayTransformation = pDisplay->GetDisplayTransformation();
	OGRSpatialReference* pDisplaySpatialReference = pDisplayTransformation->GetSpatialReference();
	OGRSpatialReference* pRasterSpatialReference = pRaster->GetSpatialReference();
	bool IsSpaRefSame(true);
	if(pDisplaySpatialReference && pDisplaySpatialReference)
		IsSpaRefSame = pDisplaySpatialReference->IsSame(pRasterSpatialReference);
	OGREnvelope VisibleBounds = pDisplayTransformation->GetVisibleBounds();
	OGREnvelope* pRasterExtent = pRaster->GetEnvelope();
	OGREnvelope RasterEnvelope, DisplayEnvelop;

	if(!IsSpaRefSame)
	{
		RasterEnvelope = TransformEnvelope(pRasterExtent, pRasterSpatialReference, pDisplaySpatialReference);
	}
	else
	{
		RasterEnvelope = *pRasterExtent;
	}
	bool IsZoomIn(false);
	//
	IsZoomIn = RasterEnvelope.MaxX > VisibleBounds.MaxX || RasterEnvelope.MaxY > VisibleBounds.MaxY || RasterEnvelope.MinX < VisibleBounds.MinX || RasterEnvelope.MinY < VisibleBounds.MinY;
	if (IsZoomIn)
	{
		//Intersect bounds
		OGREnvelope DrawBounds;
		DrawBounds.MinX = MAX(RasterEnvelope.MinX, VisibleBounds.MinX);
		DrawBounds.MinY = MAX(RasterEnvelope.MinY, VisibleBounds.MinY);
		DrawBounds
	}

}