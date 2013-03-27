#pragma once
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
	virtual void RemoveCommand(size_t nIndex) = 0;
	virtual void MoveCommandLeft(size_t nIndex) = 0;
	virtual void MoveCommandRight(size_t nIndex) = 0;
	virtual size_t GetCommandCount(void) = 0;
	virtual ICommand* GetCommand(void) = 0;
	virtual void Serialize(IApplication* pApp, wxXmlNode* pNode, bool bStore = false) = 0;
};

typedef std::vector<IGISCommandBar*> COMMANDBARARRAY;
typedef std::vector<wxWindow*> WINDOWARRAY;

class IApplication
{
public:
	virtual ~IApplication(void) {};
	//pure virtual
	virtual void OnApplicaiton(void) = 0;
	virtual IStatusBar* GetStatusBar(void) = 0;
	virtual IGISConfig* GetConfig(void) = 0;
	virtual IGISCommandBar* GetCommandBar(wxString sName) = 0;
	virtual void RemoveCommandBar(IGISCommandBar* pBar) = 0;
	virtual bool AddCommandBar(IGISCommandBar* pBar) = 0 ;
	virtual void Customize(void) = 0;
	virtual ICommand* GetCommand(long CmdID) = 0;
	virtual ICommand* GetCommand(wxString sCmdName, unsigned char rCmdSubType) = 0;
	virtual void ShowStatusBar(bool bShow) = 0;
	virtual bool IsStatusBarShown(void) = 0;
	virtual void ShowApplicationWindow(wxWindow* pWnd, bool bShow = true) = 0;
	virtual WINDOWARRAY* GetChildWindows(void) = 0;
    virtual void RegisterChildWindow(wxWindow* pWnd) = 0;
};

class ICommand :
	public wxObject
{
public:
	ICommand(void) : m_subtype(0) {};
	virtual ~ICommand(void) {};

	//pure vitual
	virtual  wxIcon GetBitmap(void) = 0;
	virtual wxString getCaption(void) = 0;
	virtual wxString GetCategory(void ) = 0;
	virtual bool GetChecked(void) = 0;
	virtual bool GetEnabled(void) = 0;
	virtual wxString GetMessage(void) = 0;
	virtual wxGISEnumCommandKind GetKind(void) =0;
	virtual void OnClick(void) = 0;
	virtual bool OnCreate(IApplication* pApp) = 0;
	virtual wxString GetTooltip(void) = 0;
	virtual unsigned char GetCount(void) = 0;
	virtual void SetID(long nID) { m_CommandID = nID; };
	virtual long GetID(void) {return m_CommandID; };

	//
	virtual void SetSubType(unsigned char SubType){m_subtype = SubType;};
	virtual unsigned char GetSubType(void){return m_subtype; };
protected:
	unsigned char m_subtype;
	long m_CommandID;
};

typedef std::vector<ICommand*> COMMANDARRAY;

class IToolBarControl
{
public:
	virtual ~IToolBarControl(void) {};
	virtual void Activate(IApplication* pApp) = 0;
	virtual void Deactivate(void) = 0;
};

class IToolControl :
      public ICommand
{
public:
	virtual ~IToolControl(void) {};
	virtual IToolBarControl* GetControl(void) = 0;
	virtual wxString GetToolLabel(void) = 0;
	virtual bool HasToolLabel(void) = 0;
};

class ITool :
	public ICommand
{
	virtual ~ITool(void) {};
	virtual void SetChecked(bool bCheck) = 0;
	virtual wxCursor GetCursor( void )  = 0;
	virtual void OnMouseDowm(wxMouseEvent& event) = 0;
	virtual void OnMouseUP(wxMouseEvent& event) = 0;
	virtual void OnMouseMove(wxMouseEvent& event) = 0;	
	virtual void OnMouseDoubleClick(wxMouseEvent& event) = 0;
};