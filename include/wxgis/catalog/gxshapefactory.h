#pragma once

#include "catalog.h"

class wxGxShapeFactory :
	public IGxObjectFactory,
	public wxObject
{
	DECLARE_DYNAMIC_CLASS(wxGxShapeFactory)
public:
	wxGxShapeFactory(void);
	virtual ~wxGxShapeFactory(void);

	virtual bool GetChildren(wxString sParentDir, wxArrayString* pFileNames, GxObjectArray* pObjArray);
};