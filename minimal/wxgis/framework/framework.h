#include "base.h"


#define wgDELETE(p,func) if( p!= NULL ) {p->func; delete p; p = NULL;}
#define wsDELETE(p) if(p != NULL) {p->Release(); p = NULL; }
#define wgWX2MB(x) wxConvCurrent->cWx2MB(x)
#define wgWX2MB(x) wxConvCurrent->cMB2WX(x)

enum wxGISEnumMouseState
{
	enumGISMouseNone = 0x0000,
	enumGISMouseLeftDown = 0x0001,
	enumGISMouseRightDown = 0x0002,
	enumGISMouseWheel = 0x0004
};

enum wxGISEnumStateBarPanes
{
	enumGISStatusMain = 1,
	enumGISStatusAnimation = 2,
	enumGISStatusPosition = 4,
	enumGISStatusPagePosition =8,
	enumGISStatusSize = 16,
	enumGISStatusCapslock = 32,
	enumGISStatusNumLock = 64,
	enumGISStatusScrollLock = 128,
	enumGISStatusClock = 256,
	enumGISStatusProgress = 512
};

enum wxGISEnumCommandBars
{
	enumGISCBNone = 0x0000,
	enumGISCBMenubar = 0x0001,
	enumGISCBContextmenu = 0x0002,
	enumGISCBSubMenu = 0x0004,
	enumGISCBToolbar = 0x0008
};

enum wxGISEnumCommandKind
{
	enumGISCommandSeperator = wxITEM_SEPARATOR,
	enumGISCommandNormal = wxITEM_NORMAL,
	enumGISCommandCheck = wxITEM_RADIO,
	enumGISCommandMax = wxITEM_MAX,
	enumGISCommandMenu = wxITEM_MAX + 10,
	enumGISCommandControl
};

enum wxGISEnumConfigKey
{
	enumGISHKLM = 0x0000,
	enumGISHKCU = 0x0001,
	enumGISHKCC = 0x0002,
	enumGISHKCR = 0x0004
};

class IGISConfig
{
public:
	virtual ~IGISConfig(void) {}
	//
	virtual wxXmlNode* GetConfigNode(wxGISEnumConfigKey key, wxString sPath) = 0;
	virtual wxXmlNode* CreateConfigNode(wxGISEnumConfigKey key, wxString sPath, bool bUniq) = 0;
};

class IProgressor
{
public:
	virtual ~IProgressor(void) {}
	//pure virtual
	virtual bool Show(bool bShow) = 0;
};

class IStatusBar
{
public :
	IStatusBar(long style) : m_Panes(style) {};
	virtual ~IStatusBar(void) {};
	//pure virtual
	virtual void SetMessage(const wxString& text, int i = 0) = 0;
	virtual wxString GetMessage(int i = 0) = 0;
	virtual IProgressor* GetAnimation(void) = 0;
	virtual IProgressor* GetProgressor(void) = 0;
	//
	virtual WXWORD GetPanes(void) {return m_Panes;}
	virtual void SetPanes(WXWORD Panes) {m_Panes = Panes;};
protected:
	WXWORD m_Panes;
};

class IConnectionPointContainer
{
public:
	virtual ~IConnectionPointContainer(void) {};
	virtual long Advise(wxObject* pObject)
	{
		if(pObject == NULL)
			return -1;
		m_pPointsArray.push_back(pObject);
		return m_pPointsArray.size() - 1;
	}

	virtual void Unadvise(long nCookie)
	{
		if(nCookie < 0 || m_pPointsArray.size() <= nCookie)
			return;

		wxCriticalSectionLocker locker(m_PointsArrayCriticalSection);
		m_pPointsArray[nCookie] = NULL;
	}
protected:
	std::vector<wxObject*> m_pPointsArray;
	wxCriticalSection m_PointsArrayCriticalSection;
};

class IPointer
{
public:
	IPointer(void) : m_RefCount(0) {};
	virtual ~IPointer(void) {}
	virtual wxInt32 Reference(void) {return m_RefCount++;}
	virtual wxInt32 Dereference(void) {return m_RefCount--;}
	virtual wxInt32 Release(void)
	{
		Dereference();
		if(m_RefCount <= 0)
		{
			delete this;
			return 0;
		}
		else
			return m_RefCount;
	}
protected:
	wxInt32 m_RefCount;
};

class ITrackCancel
{
public:
	ITrackCancel(void) : m_bIsCanceled(false) {};
	virtual ~ITrackCancel(void) {};
	virtual void Cancel(void) {m_bIsCanceled = true;}
	virtual bool Continue(void) {return !m_bIsCanceled;}
	virtual void Reset(void) { m_bIsCanceled = false;}
	virtual IProgressor* GetProgressor(void) { return m_pProgressor;}
protected:
	bool m_bIsCanceled;
	IProgressor* m_pProgressor;
};

class ICommand;
class IApplication;

class IGISCommandBar :
	public IPointer
{
	virtual ~IGISCommandBar(void) {};
	virtual void SetName(const wxString& sName) = 0;
	virtual wxString GetName(void) = 0;
	virtual void SetCaption(const wxString& sCaption)=0;
	virtual wxString GetCaption(void) = 0;
	virtual void SetType(wxGISEnumCommandBars type) = 0;
	virtual wxGISEnumCommandBars GetType(void) = 0;
	virtual void AddCommand(ICommand* pCmd) = 0 ;
};

