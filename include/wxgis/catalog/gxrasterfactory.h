#pragma once

#include "wxgis/catalog/catalog.h"

class wxGxRasterFactory :
	public IGxObjectFactory,
	public wxObject
{
	DECLARE_DYNAMIC_CLASS(wxGxRasterFactory)
public:
	wxGxRasterFactory(void);
	virtual ~wxGxRasterFactory(void);
	//IGxObjectFactory
	virtual bool GetChildren(wxString sParentDir, wxArrayString* pFileNames, GxObjectArray* pObjArray);
};