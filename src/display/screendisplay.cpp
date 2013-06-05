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

	for (int nDestPixY = nYbeg; nDestPixY < nYend; nDestPixY++)
	{
		// we need to calculate the source pixel to interpolation from - Y-axis
		srcpixy = double(nDestPixY) * rHRatio + rDeltaY;
		srcpixy1 = (int)(srcpixy);
		srcpixy2 = ( srcpixy1 == srcpixymax ) ? srcpixy1 : srcpixy1 + 1.0;
		dy = srcpixy - srcpixy1;
		dy1 = 1.0 - dy;

		for (int nDestPixX = 0; nDestPixX < nDestWidth; nDestPixX++)
		{
			// X-axis of pixel to interpolate from
			srcpixx = double(nDestPixX) * rWRatio + rDeltaX;
			srcpixx1 = (int)(srcpixx);
			srcpixx2 = ( srcpixx1 == srcpixxmax ) ? srcpixx1 : srcpixx1 + 1.0;
			dx = srcpixx - srcpixx1;
			dx1 = 1.0 - dx;

			int x_offset1 = srcpixx1 < 0.0 ? 0 : srcpixx1 > srcpixxmax ? srcpixxmax : (int)(srcpixx1);
			int x_offset2 = srcpixx2 < 0.0 ? 0 : srcpixx2 > srcpixxmax ? srcpixxmax : (int)(srcpixx2);
			int y_offset1 = srcpixy1 < 0.0 ? 0 : srcpixy1 > srcpixymax ? srcpixymax : (int)(srcpixy1);
			int y_offset2 = srcpixy2 < 0.0 ? 0 : srcpixy2 > srcpixymax ? srcpixymax : (int)(srcpixy2);

			int src_pixel_index00 = y_offset1 * nOrigWidth + x_offset1;
			int src_pixel_index01 = y_offset1 * nOrigWidth + x_offset2;
			int src_pixel_index10 = y_offset2 * nOrigWidth + x_offset1;
			int src_pixel_index11 = y_offset2 * nOrigWidth + x_offset2;

            //first line
			r1 = pOrigData[src_pixel_index00 * 3 + 0] * dx1 + pOrigData[src_pixel_index01 * 3 + 0] * dx;
			g1 = pOrigData[src_pixel_index00 * 3 + 1] * dx1 + pOrigData[src_pixel_index01 * 3 + 1] * dx;
		    b1 = pOrigData[src_pixel_index00 * 3 + 2] * dx1 + pOrigData[src_pixel_index01 * 3 + 2] * dx;

			//second line
			r2 = pOrigData[src_pixel_index10 * 3 + 0] * dx1 + pOrigData[src_pixel_index11 * 3 + 0] * dx;
			g2 = pOrigData[src_pixel_index10 * 3 + 1] * dx1 + pOrigData[src_pixel_index11 * 3 + 1] * dx;
			b2 = pOrigData[src_pixel_index10 * 3 + 2] * dx1 + pOrigData[src_pixel_index11 * 3 + 2] * dx;

			//result line
			//

			pDestData[0] = r1 * dy1 + r2 * dy;
			pDestData[1] = g1 * dy1 + g2 * dy;
			pDestData[2] = b1 * dy1 + b2 * dy;
            pDestData += 3;
			if(pTrackCancel && !pTrackCancel->Continue())
				return;
		}
	}
}

void wxRasterDrawThread::OnHalfBilinearInterpolation(unsigned char* pOrigData, unsigned char* pDestData, int nYbeg, int nYend, int nOrigWidth, int nOrigHeight, int nDestWidth, double rWRatio, double rHRatio, double rDeltaX, double rDeltaY, ITrackCancel* pTrackCancel)
{
	int srcpixymax = nOrigHeight - 1;
	int srcpixxmax = nOrigWidth - 1;

	double srcpixy, srcpixy1, srcpixy2, dy, dy1;
	double srcpixx, srcpixx1, srcpixx2, dx, dx1;
	double r1, g1, b1;
	double r2, g2, b2;
	long W = nDestWidth * 3;

	for (int nDestPixY = nYbeg; nDestPixY < nYend; nDestPixY += 2)
	{
		// we need to calculate the source pixel to interpolation from - Y-axis
		srcpixy = double(nDestPixY) * rHRatio + rDeltaY;
		srcpixy1 = (int)(srcpixy);
		srcpixy2 = ( srcpixy1 == srcpixymax ) ? srcpixy1 : srcpixy1 + 1.0;
		dy = srcpixy - srcpixy1;
		dy1 = 1.0 - dy;

		for (int nDestPixX = 0; nDestPixX < nDestWidth; nDestPixX++)
		{
			// X-axis of pixel to interpolate from
			srcpixx = double(nDestPixX) * rWRatio + rDeltaX;
			srcpixx1 = (int)(srcpixx);
			srcpixx2 = ( srcpixx1 == srcpixxmax ) ? srcpixx1 : srcpixx1 + 1.0;
			dx = srcpixx - srcpixx1;
			dx1 = 1.0 - dx;

			int x_offset1 = srcpixx1 < 0.0 ? 0 : srcpixx1 > srcpixxmax ? srcpixxmax : (int)(srcpixx1);
			int x_offset2 = srcpixx2 < 0.0 ? 0 : srcpixx2 > srcpixxmax ? srcpixxmax : (int)(srcpixx2);
			int y_offset1 = srcpixy1 < 0.0 ? 0 : srcpixy1 > srcpixymax ? srcpixymax : (int)(srcpixy1);
			int y_offset2 = srcpixy2 < 0.0 ? 0 : srcpixy2 > srcpixymax ? srcpixymax : (int)(srcpixy2);

			int src_pixel_index00 = y_offset1 * nOrigWidth + x_offset1;
			int src_pixel_index01 = y_offset1 * nOrigWidth + x_offset2;
			int src_pixel_index10 = y_offset2 * nOrigWidth + x_offset1;
			int src_pixel_index11 = y_offset2 * nOrigWidth + x_offset2;

			//first line
			r1 = pOrigData[src_pixel_index00 * 3 + 0] * dx1 + pOrigData[src_pixel_index01 * 3 + 0] * dx;
			g1 = pOrigData[src_pixel_index00 * 3 + 1] * dx1 + pOrigData[src_pixel_index01 * 3 + 1] * dx;
			b1 = pOrigData[src_pixel_index00 * 3 + 2] * dx1 + pOrigData[src_pixel_index01 * 3 + 2] * dx;

			//second line
			r2 = pOrigData[src_pixel_index10 * 3 + 0] * dx1 + pOrigData[src_pixel_index11 * 3 + 0] * dx;
			g2 = pOrigData[src_pixel_index10 * 3 + 1] * dx1 + pOrigData[src_pixel_index11 * 3 + 1] * dx;
			b2 = pOrigData[src_pixel_index10 * 3 + 2] * dx1 + pOrigData[src_pixel_index11 * 3 + 2] * dx;

			//result line
			//

			pDestData[0] = r1 * dy1 + r2 * dy;
			pDestData[1] = g1 * dy1 + g2 * dy;
			pDestData[2] = b1 * dy1 + b2 * dy;

			if (nDestPixX < nYend - 1)
			{
				pDestData[0 + W] = pDestData[0];
				pDestData[1 + W] = pDestData[1];
				pDestData[2 + W] = pDestData[2];
			}

			pDestData += 3;
			if(pTrackCancel && !pTrackCancel->Continue())
				return;
		}
		pDestData += W;
	}
}

void wxRasterDrawThread::OnHalfQuadBilinearInterpolation(unsigned char* pOrigData, unsigned char* pDestData, int nYBeg, int nYend, int nOrigWidth, int nOrigHeight, int nDestWidth, double rWRatio, double rHRatio, double rDeltaX, double rDeltaY, ITrackCancel* pTrackCancel)
{
	int srcpixymax = nOrigHeight - 1;
	int srcpixxmax = nOrigWidth - 1;

	double srcpixy, srcpixy1, srcpixy2, dy, dy1;
	double srcpixx, srcpixx1, srcpixx2, dx, dx1;
	double r1, g1, b1;
	double r2, g2, b2;
	long W = nDestWidth * 3;

	for (int nDestPixY = nYbeg; nDestPixY < nYend; nDestPixY += 2)
	{
		// We need to calculate the source pixel to interpolate from - Y-axis
		srcpixy = double(nDestPixY) * rHRatio + rDeltaY;
		srcpixy1 = (int)(srcpixy);
		srcpixy2 = ( srcpixy1 == srcpixymax ) ? srcpixy1 : srcpixy1 + 1.0;
		dy = srcpixy - srcpixy1;
		dy1 = 1.0 - dy;

		for (int nDestPixX = 0; nDestPixX < nDestWidth; nDestPixX += 2)
		{
			// X-axis of pixel to interpolate from
			srcpixx = double(nDestPixX) * rWRatio + rDeltaX;
			srcpixx1 = (int)(srcpixx);
			srcpixx2 = ( srcpixx1 == srcpixxmax ) ? srcpixx1 : srcpixx1 + 1.0;
			dx = srcpixx - srcpixx1;
			dx1 = 1.0 - dx;

			int x_offset1 = srcpixx1 < 0.0 ? 0 : srcpixx1 > srcpixxmax ? srcpixxmax : (int)(srcpixx1);
			int x_offset2 = srcpixx2 < 0.0 ? 0 : srcpixx2 > srcpixxmax ? srcpixxmax : (int)(srcpixx2);
			int y_offset1 = srcpixy1 < 0.0 ? 0 : srcpixy1 > srcpixymax ? srcpixymax : (int)(srcpixy1);
			int y_offset2 = srcpixy2 < 0.0 ? 0 : srcpixy2 > srcpixymax ? srcpixymax : (int)(srcpixy2);

			int src_pixel_index00 = y_offset1 * nOrigWidth + x_offset1;
			int src_pixel_index01 = y_offset1 * nOrigWidth + x_offset2;
			int src_pixel_index10 = y_offset2 * nOrigWidth + x_offset1;
			int src_pixel_index11 = y_offset2 * nOrigWidth + x_offset2;

			//first line
			r1 = pOrigData[src_pixel_index00 * 3 + 0] * dx1 + pOrigData[src_pixel_index01 * 3 + 0] * dx;
			g1 = pOrigData[src_pixel_index00 * 3 + 1] * dx1 + pOrigData[src_pixel_index01 * 3 + 1] * dx;
			b1 = pOrigData[src_pixel_index00 * 3 + 2] * dx1 + pOrigData[src_pixel_index01 * 3 + 2] * dx;

			//second line
			r2 = pOrigData[src_pixel_index10 * 3 + 0] * dx1 + pOrigData[src_pixel_index11 * 3 + 0] * dx;
			g2 = pOrigData[src_pixel_index10 * 3 + 1] * dx1 + pOrigData[src_pixel_index11 * 3 + 1] * dx;
			b2 = pOrigData[src_pixel_index10 * 3 + 2] * dx1 + pOrigData[src_pixel_index11 * 3 + 2] * dx;

			//result lines
			//result = p1 * 0.2 + p2 * 0.8

			pDestData[0] = r1 * dy1 + r2 * dy;
			pDestData[1] = g1 * dy1 + g2 * dy;
			pDestData[2] = b1 * dy1 + b2 * dy;
			pDestData[3] = pDestData[0];
			pDestData[4] = pDestData[1];
			pDestData[5] = pDestData[2];

			if (nDestPixY < nYend - 1)
			{
				pDestData[0 + W] = pDestData[0];
				pDestData[1 + W] = pDestData[1];
				pDestData[2 + W] = pDestData[2];
				pDestData[3 + W] = pDestData[0];
				pDestData[4 + W] = pDestData[1];
				pDestData[5 + W] = pDestData[2];
			}

			pDestData += 6;
			if(pTrackCancel && !pTrackCancel->Continue())
				return;
		}
		pDestData += W;
	}
}

void wxRasterDrawThread::OnFourQuadBilinearInterpolation(unsigned char* pOrigData, unsigned char* pDestData, int nYbeg, int nYend, int nOrigWidth, int nOrigHeight, int nDestWidth, double rWRatio, double rHRatio, double rDelatX, double rDelstaY, ITrackCancel* pTrackCancel)
{
	int srcpixymax = nOrigHeight - 1;
	int srcpixxmax = nOrigWidth - 1;

	double srcpixy, srcpixy1, srcpixy2, dy, dy1;
	double srcpixx, srcpixx1, srcpixx2, dx, dx1;
	double r1, g1, b1;
	double r2, g2, b2;
	long W = nDestWidth * 3;

	for (int nDestPixY = nYbeg; nDestPixY < nYend; nDestPixY += 3)
	{
		// We need to calculate the source pixel to interpolate from - Y-axis
		srcpixy = double(nDestPixY) * rHRatio + rDeltaY;
		srcpixy1 = (int)(srcpixy);
		srcpixy2 = ( srcpixy1 == srcpixymax ) ? srcpixy1 : srcpixy1 + 1.0;
		dy = srcpixy - srcpixy1;
		dy1 = 1.0 - dy;

		for (int nDestPixX = 0; nDestPixX < nDestWidth; nDestPixX += 2)
		{
			// X-axis of pixel to interpolate from
			srcpixx = double(nDestPixX) * rWRatio + rDeltaX;
			srcpixx1 = (int)(srcpixx);
			srcpixx2 = ( srcpixx1 == srcpixxmax ) ? srcpixx1 : srcpixx1 + 1.0;
			dx = srcpixx - srcpixx1;
			dx1 = 1.0 - dx;

			int x_offset1 = srcpixx1 < 0.0 ? 0 : srcpixx1 > srcpixxmax ? srcpixxmax : (int)(srcpixx1);
			int x_offset2 = srcpixx2 < 0.0 ? 0 : srcpixx2 > srcpixxmax ? srcpixxmax : (int)(srcpixx2);
			int y_offset1 = srcpixy1 < 0.0 ? 0 : srcpixy1 > srcpixymax ? srcpixymax : (int)(srcpixy1);
			int y_offset2 = srcpixy2 < 0.0 ? 0 : srcpixy2 > srcpixymax ? srcpixymax : (int)(srcpixy2);

			int src_pixel_index00 = y_offset1 * nOrigWidth + x_offset1;
			int src_pixel_index01 = y_offset1 * nOrigWidth + x_offset2;
			int src_pixel_index10 = y_offset2 * nOrigWidth + x_offset1;
			int src_pixel_index11 = y_offset2 * nOrigWidth + x_offset2;

			//first line
			r1 = pOrigData[src_pixel_index00 * 3 + 0] * dx1 + pOrigData[src_pixel_index01 * 3 + 0] * dx;
			g1 = pOrigData[src_pixel_index00 * 3 + 1] * dx1 + pOrigData[src_pixel_index01 * 3 + 1] * dx;
			b1 = pOrigData[src_pixel_index00 * 3 + 2] * dx1 + pOrigData[src_pixel_index01 * 3 + 2] * dx;

			//second line
			r2 = pOrigData[src_pixel_index10 * 3 + 0] * dx1 + pOrigData[src_pixel_index11 * 3 + 0] * dx;
			g2 = pOrigData[src_pixel_index10 * 3 + 1] * dx1 + pOrigData[src_pixel_index11 * 3 + 1] * dx;
			b2 = pOrigData[src_pixel_index10 * 3 + 2] * dx1 + pOrigData[src_pixel_index11 * 3 + 2] * dx;

			//result lines
			//result = p1 * 0.2 + p2 * 0.8

			pDestData[0] = r1 * dy1 + r2 * dy;
			pDestData[1] = g1 * dy1 + g2 * dy;
			pDestData[2] = b1 * dy1 + b2 * dy;
			pDestData[3] = pDestData[0];
			pDestData[4] = pDestData[1];
			pDestData[5] = pDestData[2];

			if (nDestPixY < nYend - 2)
			{
				pDestData[0 + W] = pDestData[0];
				pDestData[1 + W] = pDestData[1];
				pDestData[2 + W] = pDestData[2];
				pDestData[3 + W] = pDestData[0];
				pDestData[4 + W] = pDestData[1];
				pDestData[5 + W] = pDestData[2];

				pDestData[0 + W * 2] = pDestData[0];
				pDestData[1 + W * 2] = pDestData[1];
				pDestData[2 + W * 2] = pDestData[2];
				pDestData[3 + W * 2] = pDestData[0];
				pDestData[4 + W * 2] = pDestData[1];
				pDestData[5 + W * 2] = pDestData[2];
			}

			pDestData += 6;
			if(pTrackCancel && !pTrackCancel->Continue())
				return;
		}
		pDestData += W * 2;
	}
}

double wxRasterDrawThread::BiCubicKernel(double x)
{
	if( x > 2.0)
		return 0.0;

	double a, b, c, d;
	double xm1 = x - 1.0;
	double xp1 = x + 1.0;
	double xp2 = x + 2.0;
	
	a = ( xp2 <= 0.0 ) ? 0.0 : xp2 * xp2 * xp2;
	b = ( xp1 <= 0.0 ) ? 0.0 : xp1 * xp1 * xp1;
	c = ( x   <= 0.0 ) ? 0.0 : x * x * x;
	d = ( xm1 <= 0.0 ) ? 0.0 : xm1 * xm1 * xm1;

	return ( 0.16666666666666666667 * ( a - ( 4.0 * b ) + ( 6.0 * c ) - ( 4.0 * d ) ) );
}

void wxRasterDrawThread::OnBicubicInterpolation(unsigned char* pOrigData, unsigned char* pDestData, int nYbeg, int nYend, int nOrigWidth, int nOrigHeight, int nDestWidth, double rWRatio, double rHRatio, double rDelatX, double rDelstaY, ITrackCancel* pTrackCancel)
{
	int srcpixymax = nOrigHeight - 1;
	int srcpixymax = nOrigWidth - 1;

	double srcpixy, dy;
	double srcpixx, dx;

	for (int nDestPixY = nYbeg; nDestPixY < nYend; nDestPixY++)
	{
		// We need to calculate the source pixel to interpolate from - Y-axis
		srcpixy = (double)(nDestPixY) * rHRatio + rDelstaY;
		dy = srcpixy - (int)srcpixy;

		for (int nDestPixX = 0; nDestPixX < nDestWidth; nDestPixX++)
		{
			// X-axis of pixel to interpolate from
			srcpixx = (double)(nDestPixX) * rWRatio + rDelatX;
			dx = srcpixx - (int)srcpixx;

			// Sums for each color chanel
			double sum_r = 0, sum_g = 0, sum_b = 0, sum_a = 0;

			//Here we actually determine the RGBA values for the destination pixe;
			for ( int k = -1; k <= 2; k++)
			{
				// Y offset
				int y_offset = srcpixy + k < 0.0 ? 0 : srcpixy + k >= nOrigHeight ? nOrigHeight - 1 : (int)(srcpixy + k);

				// Loop across the X axis
				for ( int i = -1; i <= 2; i++)
				{
					// X offset
					int x_offset = srcpixx + i < 0.0 ? 0 : srcpixx + i >= nOrigWidth ? nOrigWidth - 1 : (int)(srcpixx + i);

					//Calculate the exact position where the source data
					// should be pulled from based on the x_offset and y_offset
					int src_pixel_index = y_offset * nOrigWidth _ x_offset;

					//Caculate the weight for the specified pixel according
					// to the bicubic b-spline kernel we're using for
					//interpolation
					double pixel_weight = BiCubicKernel(i - dx) * BiCubicKernel(k - dy);

					//Create a sum of all values for each color chanel
					//adjusted for the pixel's calculated weight
					sum_r += pOrigData[src_pixel_index * 3 + 0] * pixel_weight;
					sum_g += pOrigData[src_pixel_index * 3 + 1] * pixel_weight;
					sum_b += pOrigData[src_pixel_index * 3 + 2] * pixel_weight;
				}
			}

			//put the data into the destination image. The summed valued are of
			//double data type and are routed here for accuracy
			pDestData[0] = (unsigned char)(sum_r + 0.5);
			pDestData[1] = (unsigned char)(sum_g + 0.5);
			pDestData[2] = (unsigned char)(sum_b + 0.5);
			pDestData += 3;
			if(pTrackCancel && !pTrackCancel->continue())
				return;
		}
	}
}

//-----------------------
// wxGISDisplay
//------------------------------
wxGISDisplay::wxGISDisplay(void)