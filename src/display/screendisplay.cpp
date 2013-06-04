#include "wxgis/display/screendisplay.h"

//---
// wxRasterDrawThread
//------------

wxRasterDrawThread::wxRasterDrawThread(unsigned char* pOrigData, unsigned char* pDestData, int nOrigX, int nOrigY, double rOrigX, double rOrigY, int nDestX, int nDestY, double rDeltaX, double rDeltaY, wxGISEnumDrawQuality Quality, ITrackCancel* pTrackCancel, int nYbeg, int nYend) : wxThread(wxTHREAD_JOINABLE)
{
	m_pTrackCancel = pTrackCancel;
	m_pOrigData = pOrigData;
	m_pDestData = pDestData;
	m_nOrigX = nOrigX;
	m_nOrigY = nOrigY;
	m_rOrigX = rOrigX;
	m_rOrigY = rOrigY;
	m_nDestX = nDestX;
	m_nDestY = nDestY;
	m_rDeltaX = nDestX;
	m_rDeltaY = nDestY;
	m_Quality = Quality;
	m_nYbeg = nYbeg;
	m_nYend = nYend;
}

void *wxRasterDrawThread::Entry()
{
	double rWRatio, rHRatio;
	rWRatio = m_rOrigX / m_nDestX;
	rHRatio = m_rOrigX / m_nDestY;
	switch(m_Quality)
	{
	case enumGISQualityHalfBilinear:
		OnBilinearInterpolation(m_pOrigData, m_pDestData, m_nYbeg, m_nYend, m_nOrigX, m_nOrigY, m_nDestX, rWRatio, rHRatio, m_rDeltaX, m_rDeltaY, m_pTrackCancel);
		break;
	case enumGISQualityHalfBilinear:
		OnHalfBilinearInterpolation(m_pOrigData, m_pDestData, m_nYbeg, m_nYend, m_nOrigX, m_nOrigY, m_nDestX,  rWRatio, rHRatio, m_rDeltaX, m_rDeltaY, m_pTrackCancel);
	case enumGISQualityHalfQuadBilinear:
		OnHalfQuadBilinearInterpolation(m_pOrigData, m_pDestData, m_nYbeg, m_nYend, m_nOrigX, m_nOrigY, m_nDestX,  rWRatio, rHRatio, m_rDeltaX, m_rDeltaY, m_pTrackCancel);
	case enumGISQualityBicubic:
		OnBicubicInterpolation(m_pOrigData, m_pDestData, m_nYbeg, m_nYend, m_nOrigX, m_nOrigY, m_nDestX,  rWRatio, rHRatio, m_rDeltaX, m_rDeltaY, m_pTrackCancel);
	case enumGISQualityNearest:
	default:
		OnNearestNeighbourInterpolation(m_pOrigData, m_pDestData, m_nYbeg, m_nYend,  m_nOrigX, m_nDestX, rWRatio, rHRatio, m_rDeltaX, m_rDeltaY, m_pTrackCancel);
		break;
	}

	return NULL;
}

void wxRasterDrawThread::OnExit()
{

}

void wxRasterDrawThread::OnNearestNeighbourInterpolation(unsigned char* pOrigData, unsigned char* pDestData, int nYbeg, int nYend, int nOrigWidth, int nDestWidth, double rWRatio, double rHRatio, double rDeltaX, double rDeltaY, ITrackCancel* pTrackCancel)
{
	for (int nDestPixY = nYbeg; nDestPixY < nYend; nDestPixY)
	{
		int nOriPixY = (int)(rHRatio * double(nDestPixY) + rDeltaY);
		int scan_line = nOriPixY * nOrigWidth;
		for (int nDestPixX = 0; nDestPixX < nDestWidth; nDestPixX++)
		{
			int nOrigPixX = (int)(rWRatio * double(nDestPixX) + rDeltaY);
			int src_pixel_index = scan_line + nOrigPixX;
			src_pixel_index *= 3;
			pDestData[0] = pOrigData[src_pixel_index + 0];
			pDestData[1] = pOrigData[src_pixel_index + 1];
			pDestData[2] = pOrigData[src_pixel_index + 2];
			pDestData += 3;
			if(pTrackCancel && !pTrackCancel->Continue())
				return;
		}
	}
}

void wxRasterDrawThread::OnBilinearInterpolation(unsigned char* pOrigData, unsigned char* pDestData, int nYbeg, int nYend, int nOrigWidth, int nOrigHeight, int nDestWidth, double rWRatio, double rHRatio, double rDeltaX, double rDeltaY, ITrackCancel* pTrackCancel)
{
	int srcpixymax = nOrigHeight - 1;
	int srcpixxmax = nOrigWidth - 1;

	double srcpixy, srcpixy1, srcpixy2, dy, dy1;
	double srcpixx, srcpixx1, srcpixx2, dx, dx1;
	double r1, g1, b1;
	double r2, g2, b2;

	for (int nDestPixY = nYbeg, nDestPixY < nYend; nDestPixY++)
	{
		// we need to calculate the source pixel to interpolation from - Y-axis
		srcpixy = double(nDestPixY) * rHRatio + rDeltaY;
		srcpixy1 = (int)(srcpixy);
		srcpixy2 = ( srcpixy1 == srcpixymax ) ? srcpixy1 : srcpixy1 + 1.0;
		dx = srcpixy - srcpixy1;
		dy1 = 1.0 - dy;

		for (int nDestPixX = 0; nDestPixX < nDestWidth; nDestPixX++)
		{
			// X-axis of pixel to interpolate from
			srcpixx = double(nDestPixX) * rWRatio + rDeltaX;
			srcpixx1 = (int)(srcpixx1);
			srcpixx2 = ( srcpixx1 == srcpixxmax ) ? srcpixx1 : srcpixx1 + 1.0;
			dx = srcpixx - srcpixx1;
			dx1 = 1.0 - dx;

			int x_offset1 = srcpixx1 < 0.0 ? 0 : srcpixx1 > srcpixxmax ? srcpixxmax : (int)(srcpixx1);
			int x_offset2 = srcpixx2 < 0.0 ? 0 : srcpixx2 > srcpixxmax ? srcpixxmax : (int)(srcpixx2);
			int y_offset1 = 
		}
	}

}