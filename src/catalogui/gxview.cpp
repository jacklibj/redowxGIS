#include "wxgis/catalogui/gxview.h"
#include "wxgis/catalog/gxcatalog.h"

wxGxView::wxGxView(void)
{
}


wxGxView::~wxGxView(void)
{
}

bool wxGxView::Activate(wxGxApplication* application, IGxCatalog* Catalog, wxXmlNode* pConf)
{
	m_pCatalog = Catalog;
	m_pApplication = application;
	m_pXmlConf = pConf;
	return true;
}

void wxGxView::Deactivate(void)
{
	m_pCatalog = NULL;
	m_pApplication = NULL;
	m_pXmlConf = NULL;
}

bool wxGxView::Applies(IGxSelection* Selection)
{
	return NULL == Selection ? false : true;
}

wxString wxGxView::GetName(void)
{
	return m_sViewName;
}