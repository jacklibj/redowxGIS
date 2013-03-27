#pragma once

#include "wxgis\framework\base.h"

#define TREECTRLID  1005 //wxGxCatalog tree
#define LISTCTRLID  1006 //wxGxCatalog content view
#define TABLECTRLID 1007 //wxGxCatalog table view
#define TABCTRLID   1008 //wxGxCatalog tab with views - contents, preview & etc.
#define MAPCTRLID   1009 //wxGxCatalog map view

class IGxViewsFactory
{
public:
	virtual ~IGxViewsFactory(void){};
	virtual wxWindow* CreateView(wxString sName, wxWindow* parent) = 0;

};