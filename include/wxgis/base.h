#pragma once

#include "wx/wxprec.h"

#ifdef __BORLANDC__
   #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif



#ifdef WXMAKINGDLL_GIS_FRW
#    define WXDLLIMPEXP_GIS_FRW WXEXPORT
#    define WXDLLIMPEXP_DATA_GIS_FRW(type) WXEXPORT type
#elif defined(WXUSINGDLL)
#    define WXDLLIMPEXP_GIS_FRW WXIMPORT
#    define WXDLLIMPEXP_DATA_GIS_FRW(type) WXIMPORT type
#else /* not making nor using DLL */
#    define WXDLLIMPEXP_GIS_FRW
#    define WXDLLIMPEXP_DATA_GIS_FRW(type) type
#endif

#ifdef WXMAKINGDLL_GIS_DSP
#    define WXDLLIMPEXP_GIS_DSP WXEXPORT
#    define WXDLLIMPEXP_DATA_GIS_DSP(type) WXEXPORT type
#elif defined(WXUSINGDLL)
#    define WXDLLIMPEXP_GIS_DSP WXIMPORT
#    define WXDLLIMPEXP_DATA_GIS_DSP(type) WXIMPORT type
#else /* not making nor using DLL */
#    define WXDLLIMPEXP_GIS_DSP
#    define WXDLLIMPEXP_DATA_GIS_DSP(type) type
#endif

#ifdef WXMAKINGDLL_GIS_CRT
#    define WXDLLIMPEXP_GIS_CRT WXEXPORT
#    define WXDLLIMPEXP_DATA_GIS_CRT(type) WXEXPORT type
#elif defined(WXUSINGDLL)
#    define WXDLLIMPEXP_GIS_CRT WXIMPORT
#    define WXDLLIMPEXP_DATA_GIS_CRT(type) WXIMPORT type
#else /* not making nor using DLL */
#    define WXDLLIMPEXP_GIS_CRT
#    define WXDLLIMPEXP_DATA_GIS_CRT(type) type
#endif

#ifdef WXMAKINGDLL_GIS_CLT
#    define WXDLLIMPEXP_GIS_CLT WXEXPORT
#    define WXDLLIMPEXP_DATA_GIS_CLT(type) WXEXPORT type
#elif defined(WXUSINGDLL)
#    define WXDLLIMPEXP_GIS_CLT WXIMPORT
#    define WXDLLIMPEXP_DATA_GIS_CLT(type) WXIMPORT type
#else /* not making nor using DLL */
#    define WXDLLIMPEXP_GIS_CLT
#    define WXDLLIMPEXP_DATA_GIS_CLT(type) type
#endif

#ifdef WXMAKINGDLL_GIS_CLU
#    define WXDLLIMPEXP_GIS_CLU WXEXPORT
#    define WXDLLIMPEXP_DATA_GIS_CLU(type) WXEXPORT type
#elif defined(WXUSINGDLL)
#    define WXDLLIMPEXP_GIS_CLU WXIMPORT
#    define WXDLLIMPEXP_DATA_GIS_CLU(type) WXIMPORT type
#else /* not making nor using DLL */
#    define WXDLLIMPEXP_GIS_CLU
#    define WXDLLIMPEXP_DATA_GIS_CLU(type) type
#endif


//#define WXDLLEXPORP WXMAKINGDLL_GIS
//#define WXDLLEXPORP_DATA WXDLLIMPEXP_DATA_GIS

#include <wx/xml/xml.h>

#include <algorithm>
#include <map>
#include <vector>

#define CONFIG_DIR wxT("wxGIS")
#define DEF wxT("~def")
#define NON wxT("~non")
#define ERR wxT("~err")
#define NONAME _("no name")
#define DELTA 0.0001
#define CACHE_SIZE 3000
#define ZOOM_FACTOR 0.25
#define PI 3.1415926535897932384626433832795
#define PIDEG 0.017453292519943295769236907684886

static wxString DoubleToString(double Val, bool IsLon)
{
	wxString znak;
	if(Val < 0)
	{
		if(IsLon) znak = _(" W");
		else znak = _(" S");
	}
	else
	{
		if (IsLon) znak = _(" E");
		else znak = _(" N");
	}
	Val = fabs(Val);
	int grad = floor(Val);
	int min = floor((Val - grad) * 60);
	int sec = floor((Val - grad - (double) min / 60) * 3600);
	wxString str;
	if(IsLon)
		str.Printf(wxT("%.3d-%2d - %.2d%s"),grad, min, sec, znak.c_str());
	else
		str.Printf(wxT("%.2d-%.2d-%.2d%s"), grad, min, sec, znak.c_str());
	return str;
};

static double StringToDouble(wxString Val, wxString asterisk)
{
	wxString buff;
	unsigned char counter = 0;
	int grad, min, sec;
	for (size_t i = 0; i < Val.Len(); i++)
	{
		wxChar ch = Val[i];

		if (ch == '-' || ch == ' ')
		{
			switch(counter)
			{
			case 0:
				grad = wxAtoi(buff.c_str());
				break;
			case 1:
				min = wxAtoi(buff.c_str());
				break;
			case 2:
				sec = wxAtoi(buff.c_str());
				break;
			}
		}	
	}
	int mul = -1;
	if (buff == _(" E") || buff == _(" N"))
		mul = 1;
	return ((double)grad + (double)min / 60 + (double)sec /3600) * mul;
};

static wxString NumberScale(double fScaleRatio)
{
	wxString str = wxString::Format(wxT("%.2f"), fScaleRatio);
	int pos = str.Find(wxT("."));
	if (pos == wxNOT_FOUND)
		pos = str.Len();
	wxString res = str.Right(str.Len() - pos);
	for (size_t i = 1; i< pos; i++)
	{
		res.Prepend(str[pos - i]);
		if((i % 3) == 0)
			res.Prepend(wxT(" "));
	}
	return res;
};

