#pragma once

#include "wxgis/catalog/catalog.h"

class WXDLLIMPEXP_GIS_CLT wxGxFolderFactory :
	public IGxObjectFactory,
	public wxObject
{
	DECLARE_DYNAMIC_CLASS(wxGxFolderFactory)
public:
	wxGxFolderFactory(void);
	virtual ~wxGxFolderFactory(void);
	//iGxObjectFactory
	virtual bool GetChildren(wxString sParentDir, wxArrayString* pFileNames, GxObjectArray* pObjArray);
};