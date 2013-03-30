#include "gxcatalog.h"
#include "gxdisconnection.h"
#include "wxgis\framework\config.h"
#include "wx\volume.h"
#include "wx\msgdlg.h"

//--------------
// wxGxCatalog
//---------------

wxGxCatalog::wxGxCatalog(void) : m_bIsChildrenLoaded(false), m_pConf(NULL)
{
	m_pSelection = new wxGxSelection();
	m_pCatalog = NULL;
	m_pParent = NULL;
}

wxGxCatalog::~wxGxCatalog(void) 
{
	Detach();
}

void wxGxCatalog::Detach(void)
{
	wxXmlNode* pNode = m_pConf->GetConfigNode(enumGISHKCU, wxString(wxT("catalog/rootitems")));
	if(pNode)
	{
		wxXmlNode* pChildren = pNode->GetChildren();
		while(pChildren)
		{
			if(pChildren->GetPropVal(wxT("name"), NONAME) == DISCCONNCAT)
			{
				SerializeDiscConnections(pChildren, true);
				break;
			}
			else
			{
				//
			}
			pChildren = pChildren->GetNext();
		}
	}
	else
		SerializeDiscConnections(NULL, true);

	SetShowExt(m_bShowExt);
	SetShowHidden(m_bShowHidden);

	EmptyObjectFactories();
	wxDELETE(m_pSelection);
	EmptyChildren();
	wxDELETE(m_pConf);
}

void wxGxCatalog::Refresh(void)
{
	for(size_t i = 0; i< m_Children.size(); i++)
		m_Children[i]->Refresh();
}

void wxGxCatalog::EditProperties(wxWindow * parent)
{
}

bool wxGxCatalog::AddChild(IGxObject* pChild)
{
	if(pChild == NULL)
		return false;
	pChild->Attach(this, this);
	m_Children.push_back(pChild);
	return true;
}

bool wxGxCatalog::DeleteChild(IGxObject* pChild)
{
	if(pChild == NULL)
		return false;
	GxObjectArray::iterator pos = std::find(m_Children.begin(), m_Children.end(), pChild);
	if(pos != m_Children.end())
	{
		pChild->Detach();
		delete pChild;
		m_Children.erase(pos);
	}
	return true;
}

void wxGxCatalog::EmptyChildren(void)
{
	for(size_t i =0; i < m_Children.size(); i++)
	{
		wxDELETE(m_ObjectFactoriesArray[i].pFactory);
	}
	m_ObjectFactoriesArray.empty();
}

void wxGxCatalog::Init(void)
{
	if(m_bIsChildrenLoaded)
		return;

	m_pConf = new wxGISConfig(wxString(wxT("wxCatalog")), CONFIG_DIR);

	wxXmlNode* pConfXmlNode = m_pConf->GetConfigNode(enumGISHKCU, wxString(wxT("catalog")));
	if(!pConfXmlNode)
		pConfXmlNode = m_pConf->GetConfigNode(enumGISHKLM, wxString(wxT("catalog")));
	if(!pConfXmlNode)
		return;

	m_bShowHidden = wxAtoi(pConfXmlNode->GetPropVal(wxT("Show_hidden"), wxT("0")));
	m_bShowExt = wxAtoi(pConfXmlNode->GetPropVal(wxT("show_ext"), wxT("1")));

	wxXmlNode* pObjectFactoriesNode = m_pConf->GetConfigNode(enumGISHKLM, wxString(wxT("catalog/objectfactories")));
	LoadObjectFactories(pObjectFactoriesNode);

	//loads current user and when local machine items
	wxXmlNode* pRootItemsNode = m_pConf->GetConfigNode(enumGISHKCU, wxString(wxT("Catalog/rootitems")));
	LoadChildren(pRootItemsNode);
}


void wxGxCatalog::LoadObjectFactories(wxXmlNode* pNode)
{
	if(pNode == NULL)
		return;

	wxXmlNode* pChildren = pNode->GetChildren();
	while(pChildren)
	{
		wxString sName = pChildren->GetPropVal(wxT("factory_name"), wxT(""));
		if(!sName.IsEmpty())
		{
			wxObject *obj = wxCreateDynamicObject(sName);
			IGxObjectFactory *Factory = dynamic_cast<IGxObjectFactory*>(obj);
			if(!Factory != NULL)
			{
				Factory->PutCatalogRef(this);
				OBJFACTORYDESC desc = {Factory, bool(wxAtoi(pChildren->GetPropVal(wxT("is_enabled"), wxT("1"))))};
				m_ObjectFactoriesArray.push_back(desc);
				wxLogMessage(_("wxGxCatalog: ObjectFactory %s initilize"), sName.c_str());
				//plugin->Init(child);
			}
			else
				wxLogError(_("wxGxCatalog: Error initializing ObjectFactory %s"), sName.c_str());
		}
		pChildren = pChildren->GetNext();
	}
}

void wxGxCatalog::LoadChildren(wxXmlNode* pNode)
{
	if(!pNode)
		return;

}
