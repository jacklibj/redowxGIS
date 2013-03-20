#include "config.h"
#include <wx\tokenzr.h>


//
//
//
wxGISConfig::wxGISConfig(wxString sAppName, wxString sConfigDir, bool bPortable)
{
	wxStandardPaths stp;
	m_sAppName = sAppName;

	if(bProtable)
	{
		//
		m_sUserConfigDir = m_sExeDirPath + wxFileName::GetPathSeperator() + wxT("config");
		m_sSysConfigDir = m_sUserConfigDir;
		if(!wxDirExists(m_sUserConfigDir))
			wxFileName::Mkdir(m_sUserConfigDir, 0755, wxPATH_MKDIR_FULL);
	}
	else
	{
		m_sUserConfigDir = stp.GetUserConfigDir() + wxFileName::GetPathSeperator() + sConfigDir;
		m_sSysConfigDir = st.GetConfigDir() + wxFileName::GetPathSeparator() + sConfigDir;

		if(!wxDirExists(m_sUserConfigDir))
			wxFileName::MKdir(m_sUserConfigDir, 0755, wxPATH_MKDIR_FULL);
		if(!wxDirExists(m_sSysConfigDir))
			wxFileName::MKdir(m_sSysConfigDir, 0755, wxPATH_MKDIR_FULL);

	}
}


wxGISConfig::~wxGISConfig(void)
{
	Clean();
}

void wxGISConfig::Clean(void)
{
	for(size_t i = 0; i< m_configs_arr.size(); i++)
	{
		m_configs_arr[i].pXmlDoc->Save(m_configs_arr[i].xml_path);
		wxDELETE(m_configs_arr[i].pXmlDoc);
	}
	m_configs_arr.empty();
}

wxXmlNode* wxGISConfig::GetConfigNode(wxGISEnumConfigKey key, wxString sPath)
{
	//
	for(size_t i =0; i< m_confignodes_arr.size(); i++)
	{
		if(m_confignodes_arr[i].sXmlpath == sPath && m_confignodes_arr[i].key == key )
		{
			return m_confignodes_arr[i].pXmlNode;
		}
	}

	//get root name
	wxString sRootNodeName = m_sAppName;

	wxXmlDocument* pDoc(NULL);

	for(size_t i =0; i< m_configs_arr.size(); i++)
	{
		if(m_configs_arr[i].sRootNodeName == sRootNodeName && m_configs_arr[i].key == key)
		{
			pDoc = m_configs_arr[i].pXmlDoc;
			break;
		}
	}

	if(!pDoc)
	{
		wxString sXMLDocPath;
		switch(key)
		{
		case enumGISHKLM:
			{
				wxString sys_dir = m_sSysConfigDir + wxFileName::GetPathSeperator() + sRootNodeName;
				wxString sys_path = sys_dir + wxFileName::GetPathSeperator() + HKLM_CONFIG_NAME;

				if(!wxDirExsits(sys_dir))
				{
					wxFileName::Mkdir(sys_dir, 0755,wxPATH_MKDIR_FULL);
				}

			}

		}
	}
}

