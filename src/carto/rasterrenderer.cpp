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
	OGREnvelope RasterEnvelope, DisplayEnvelope;

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
		DrawBounds.MaxX = MIN(RasterEnvelope.MaxX, VisibleBounds.MaxX);
		DrawBounds.MaxY = MIN(RasterEnvelope.MaxY, VisibleBounds.MaxY);

		OGRRawPoint OGRRawPoints[2];
		OGRRawPoints[0].x = DrawBounds.MinX;
		OGRRawPoints[0].y = DrawBounds.MinY;
		OGRRawPoints[1].x = DrawBounds.MaxX;
		OGRRawPoints[1].y = DrawBounds.MaxY;
		wxPoint* pDCPoints = pDisplayTransformation->TransformCoordWorld2DC(OGRRawPoints, 2);

		if (!pDCPoints)
		{
			wxDELETEA(pDCPoints);
			return;
		}

		int nDCXOrig = pDCPoints[0].x;
		int nDCYOrig = pDCPoints[1].y;
		int nWidth = pDCPoints[1].x - pDCPoints[0].x;
		int nHeight = pDCPoints[0].y - pDCPoints[1].y;
		wxDELETEA(pDCPoints);

		if(nWidth <= 20 || nHeight <= 20)
			return;

		GDALDataset* pGDALDataset = pRaster->GetRaster();

		int nBandCount = pGDALDataset->GetRasterCount();
		//hack!
		int bands[3];
		if (nBandCount < 3)
		{
			bands[0] = 1;
			bands[1] = 1;
			bands[2] = 1;
		}
		else
		{
			bands[0] = 1;
			bands[1] = 2;
			bands[2] = 3;
		}

		double adfGeoTransform[6] = { 0, 0, 0, 0, 0, 0 };
		double adfReverseGeoTransform[6] = { 0, 0, 0, 0, 0, 0 };
		CPLErr err = pGDALDataset->GetGeoTransform(adfGeoTransform);
		bool bNoTransform(false);
		if (err != CE_None)
		{
			bNoTransform = true;
		}
		else
		{
			int nRes = GDALInvGeoTransform( adfGeoTransform, adfReverseGeoTransform );
		}

		// 2. get image data from raster - drag part of the raster
		if (IsSpaRefSame)
		{
			double rMinX, rMinY, rMaxX, rMaxY;

			int nXSize = pGDALDataset->GetRasterXSize();
			int nYSize = pGDALDataset->GetRasterYSize();

			if (bNoTransform)
			{
				rMinX = DrawBounds.MinX;
				rMaxX = DrawBounds.MaxX;
				rMaxY = nYSize - DrawBounds.MinY;
				rMinY = nYSize - DrawBounds.MaxY;
			}
			else
			{
				GDALApplyGeoTransform( adfReverseGeoTransform, DrawBounds.MinX, DrawBounds.MinY, &rMinX, &rMaxY);
				GDALApplyGeoTransform( adfReverseGeoTransform, DrawBounds.MaxX, DrawBounds.MaxY, &rMaxX, &rMinY);
			}

			double rImgWidth = rMaxX - rMinX;
			double rImgHeight = rMaxY - rMinY;
			int nImgWidth = ceil(rImgWidth) + 1;
			int nImgHeight = ceil(rImgHeight) + 1;

			//read in buffer
			int nMinX = floor(rMinX);
			int nMinY = floor(rMinY);
			if(nMinX < 0) nMinX = 0;
			if(nMinY < 0) nMinY = 0;
			
			if(nImgWidth > nXSize - nMinX) nImgWidth -= 1;
			if(nImgHeight > nYSize - nMinY) nImgHeight -= 1;

			//create buffer
			int nWidthOut = nWidth > nImgWidth ? nImgWidth : (double)nWidth + 1.0;
			int nHeightOut = nHeight > nImgHeight ? nImgHeight : (double)nHeight + 1.0;	
			double rImgWidthOut = nWidth > nImgWidth ? rImgWidth : (double)nWidthOut * rImgWidth / nImgWidth;
			double rImgHeightOut = nHeight > nImgHeight ? rImgHeight : (double)nHeightOut * rImgHeight / nImgHeight;

			unsigned char* data = new unsigned char[nWidthOut * nHeightOut * 3];

			err = pGDALDataset->AdviseRead(nMinX, nMinY, nImgWidth, nImgHeight, nWidthOut, nHeightOut, GDT_Byte, 3, bands, NULL);
			if (err != CE_None)
			{
				wxDELETEA(data);
				return;
			}

			err = pGDALDataset->RasterIO(GF_Read, nMinX, nMinY, nImgWidth, nImgHeight, data, nWidthOut, nHeightOut, GDT_Byte, 3, bands, sizeof(unsigned char) * 3, 0, sizeof(unsigned char));
			if (err != CE_None)
			{
				wxDELETEA(data);
				return;
			}
			//scale
			pDisplay->DrawBitmap(Scale(data, nWidthOut, nHeightOut, rImgWidthOut, rImgHeightOut, 
				nWidth + 1, nHeight + 1, rMinX - nMinX, rMinY - nMinY, enumGISQualityBilinear, pTrackCancel), nDCXOrig, nDCYOrig);

			wxDELETEA(data);
		}
		else
		{

		}
	}
	else
	{
		//1. convert newrasterenvelope to DC
		OGRRawPoint OGRRawPoints[2];
		OGRRawPoints[0].x = RasterEnvelope.MinX;
		OGRRawPoints[0].y = RasterEnvelope.MinY;
		OGRRawPoints[1].x = RasterEnvelope.MaxX;
		OGRRawPoints[1].y = RasterEnvelope.MaxY;
		wxPoint* pDCPoints = pDisplayTransformation->TransformCoordWorld2DC(OGRRawPoints, 2);

		if (!pDCPoints)
		{
			wxDELETEA(pDCPoints);
			return;
		}

		int nDCXOrig = pDCPoints[0].x;
		int nDCYOrig = pDCPoints[1].y;
		int nWidth = pDCPoints[1].x - pDCPoints[0].x;
		int nHeight = pDCPoints[0].y - pDCPoints[1].y;
		delete[] (pDCPoints);

	
		GDALDataset* pGDALDataset = pRaster->GetRaster();
		int nImgWidth = pGDALDataset->GetRasterXSize();
		int nImgHeight = pGDALDataset->GetRasterYSize();

		int nBandCount = pGDALDataset->GetRasterCount();
		//hack!
		int bands[3];
		if (nBandCount < 3)
		{
			bands[0] = 1;
			bands[1] = 1;
			bands[2] = 1;
		}
		else
		{
			bands[0] = 1;
			bands[1] = 2;
			bands[2] = 3;
		}

		//Create buffer
		unsigned char* data = new unsigned char[nWidth * nHeight * 3];
		if (IsSpaRefSame)
		{
			//read in budder
			CPLErr err = pGDALDataset->RasterIO(GF_Read, 0, 0, nImgWidth, nImgHeight, data, nWidth, nHeight, GDT_Byte, 3, bands, sizeof(unsigned char) * 3, 0, sizeof(unsigned char));
			if (err != CE_None)
			{
				wxDELETEA(data);
				return;
			}
		}
		else
		{
			//1. 
			//2. 
			unsigned char* pTempData;
			//3.
		   //4.
			//
		}
		//3.
		wxImage ResultImage(nWidth, nHeight, data);
		pDisplay->DrawBitmap(ResultImage, nDCXOrig, nDCYOrig);

		//

    }

}

OGREnvelope wxGISRasterRGBRenderer::TransformEnvelope(OGREnvelope* pEnvelope, OGRSpatialReference* pSrsSpatialReference, OGRSpatialReference* pDstSpatialReference)
{
	//get new envelope - it may rotate
	OGRCoordinateTransformation *poCT = OGRCreateCoordinateTransformation( pSrsSpatialReference, pDstSpatialReference);
	double pointsx[4];
	double pointsy[4];
	pointsx[0] = pEnvelope->MaxX;
	pointsy[0] = pEnvelope->MaxY;
	pointsx[1] = pEnvelope->MinX;
	pointsy[1] = pEnvelope->MinY;
	pointsx[2] = pEnvelope->MaxX;
	pointsy[2] = pEnvelope->MinY;
	pointsx[3] = pEnvelope->MinX;
	pointsy[3] = pEnvelope->MaxY;
	//get real envelope
	poCT->Transform(4, pointsx, pointsy);
	OCTDestroyCoordinateTransformation(poCT);
	OGREnvelope out;
	out.MinX = MIN(pointsx[0], MIN(pointsx[1], MIN(pointsx[2], pointsx[3])));
	out.MaxX = MAX(pointsx[0], MAX(pointsx[1], MAX(pointsx[2], pointsx[3])));
	out.MinY = MIN(pointsy[0], MIN(pointsy[1], MIN(pointsy[2], pointsy[3])));
	out.MaxY = MAX(pointsy[0], MAX(pointsy[1], MAX(pointsy[2], pointsy[3])));
	return out;
}

wxImage wxGISRasterRGBRenderer::Scale(unsigned char* pData, int nOrigX, int nOrigY, double rOrigX, double rOrigY, int nDestX, int nDestY, double rDeltaX, double rDeltaY, wxGISEnumDrawQuality Quality, ITrackCancel* pTrackCancel)
{

	wxImage ResultImage(nDestX, nDestY, false);
	unsigned char* pDestData = ResultImage.GetData();
	//multithread
	int CPUCount(2); //check cpu count
	std::vector<wxRasterDrawThread*> threadarray;
	int nPartSize = nDestY /CPUCount;
	int nBegY(0), nEndY;
	for(int i = 0; i < CPUCount; i++)
	{
		if (i == CPUCount - 1)
		   nEndY = nDestY;
		else
			nEndY = nPartSize * (i + 1);

		unsigned char* pDestInputData = pDestData + (nDestX * nBegY * 3);
		wxRasterDrawThread * thread = new wxRasterDrawThread(pData, pDestInputData, nOrigX, nOrigY, rOrigX, rOrigY, nDestX, nDestY, rDeltaX, rDeltaY, Quality, pTrackCancel, nBegY, nEndY);
		thread->Create();
		thread->Run();
		threadarray.push_back(thread);
		nBegY = nEndY ;
	}

	for (size_t i = 0; i < threadarray.size(); i++)
	{
		wgDELETE(threadarray[i], Wait());
	}
	return ResultImage;
}