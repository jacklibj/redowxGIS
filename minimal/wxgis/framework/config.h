
#include "framework.h"

class WXDLLIMPEXP_GIS_FRW wxGISConfig :
	public IGISConfig
{

public:
	wxGISConfig(wxString sAppName, wxString sConfigDir, bool bPortable = false);
	virtual ~wxGISConfig(void);

	//IGISConfig
	virtual wxXmlNode* GetConfigNode(wxGISEnumConfigKey key, wxString sPath);
	virtual wxXmlNode* CreateConfigNode(wxGISEnumConfigKey key, wxString sPath, bool bUniq = true);

	static void DeleteNodeChildren(wxXmlNode* pNode);

	virtual void Clean(void);
	typedef struct wxxmlconfnode
	{
		wxXmlNode* pXmlNode;
		wxString sXmlpath;
		wxGISEnumConfigKey key;
	}WXXMLCONFNODE;

	typedef struct wxxmlconf
	{
		wxString sRootNodeName;
		wxXmlDocument* pXmlDoc;
		wxString xml_path;
		wxGISEnumConfigKey key;
	}WXXMLCONF;

protected:
	wxString m_sUserConfigDir, m_sSysConfigDir, m_sExeDirPath;
	wxString m_sAppName;
	std::vector<WXXMLCONF> m_configs_arr;
	std::vector<WXXMLCONFNODE> m_confignodes_arr;
};

//
//
//

class WXDLLIMPEXP_GIS_FRW wxGISAppConfig :
	public wxGISConfig
{
	public:
		wxGISAppConfig(wxString sAppName, wxString sConfigDir, bool bbPortable = false );
		virtual ~wxGISAppConfig(void);

		virtual wxString GetLocale(void);
		virtual wxString GetLocaleDir(void);
		virtual wxString GetSysDir(void);
		virtual wxString GetLogDir(void);
		virtual bool GetDebugMode(void);
		virtual void SetLocale(wxString sLocale);
		virtual void SetLocaleDir(wxString sLocaleDir);
		virtual void SetSysDir(wxString sSysDir);
		virtual void SetLogDir(wxString sLogDir);
		virtual void SetDebugMode(bool bDebug);
};