#pragma once

#include "wx/wxprec.h"

#ifdef __BORLANDC__
   #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#ifdef WX_PRECOMP
    #include "wx/wx.h"
#endif

#ifdef WXMAKINGGDAL_GIS_FRW
#define WXDLLIMPEXP_GIS_FRW WXEXPORT
#define WXDLLIMPEXP_DATA_GIS_FRW(type) WXEXPORT type
#elif defined(WXUSINGGDALL)
#define WXDLLIMPEXP_GIS_FRW WXIMPORT
#define WXDLLIMPEXP_DATA_GIS_FRW(type) WXIMPORT type
#else /* not making nor using DLL */
#     define WXDLLIMPEXP_GIS_FRW
#     define WXDLLIMPEXP_DATA_GIS_FRW(type) typename
#endif

#ifdef WXNAKINGDLL_GIS_CLT
#    define WXDLLIMPEXP_GIS WXEXPORT
#    define WXDLLIMPEXP_DATA_GIS_CLT(type) WXEXPORT type
#elif defined(WXUSINGDLL)
#     define WXDLLIMPEXP_GIS_CLT WXIMPORT
#     define WXDLLIMPEXP_DATA_GIS_CLT(type) WXIMPORT type


