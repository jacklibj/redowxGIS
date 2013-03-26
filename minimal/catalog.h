#pragma once

#include "datasource.h"
#include "wx\filename.h"

#define APP_NAME wxT("wxGISCatalog")
#define APP_VER wxT("0.0.1 alpha")

#define NOTFIRESELID 1010

class IGxObject;
typedef std::vector<IGxPbject*> GxObjectArray;

class IGxSelection
{

};

class IGxSelectionEvents
{
public:

};

class IGxCatalog
{
};

class IGxObject
{
};

class IGxObjectUI
{
};

class IGxObjectWizard
{
};

class IGxObjectContainer :
	public IGxObject
{

};

class IGxObjectFactory
{
};

class IGxCatalogEvents
{

};

class IGxDataset
{
};


