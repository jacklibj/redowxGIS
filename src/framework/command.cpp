#include "wxgis/framework/command.h"
#include "../../art/mainframe.xpm"

//
//
//

IMPLEMENT_DYNAMIC_CLASS(wxGISCommonCmd, wxObject)

wxGISCommonCmd::wxGISCommonCmd(void) : ICommand()
{

}

wxGISCommonCmd::~wxGISCommonCmd(void)
{

}

wxIcon wxGISCommonCmd::GetBitmap(void)
{
	switch(m_subtype)
	{
	case 1:
		return wxIcon(mainframe_xpm);
	case 0:
	case 2:
	case 3:
	case 4:
	default:
		return wxIcon();
	}
}

wxString wxGISCommonCmd::GetCaption(void)
{
	switch(m_subtype)
	{
	case 0:
		return wxString(_("E&xit"));
	case 1:
		return wxString(_("&About..."));
	case 2:
		return wxString(_("Customize..."));
	case 3:
		return wxString(_("Separator"));
	case 4:
		return wxString(_("StatusBar"));
	default:
		return wxString();
	}
}

wxString wxGISCommonCmd::GetCategory(void)
{
	switch(m_subtype)
	{
	case 0:
		return wxString(_("File"));
	case 1:
		return wxString(_("Help"));
	case 2:
	case 4:
		return wxString(_("View"));
	case 3:
	default:
		return wxString(_("[No category]"));
	}
}

bool wxGISCommonCmd::GetChecked(void)
{
	switch(m_subtype)
	{
	case 1:
	case 0:
	case 2:
	case 3:
	default:
		return false;
			case 4:
				return m_pApp->IsStatusBarShown();
	}
	return false;
}

bool wxGISCommonCmd::GetEnabled(void)
{
	switch(m_subtype)
	{
	case 0://Exit
		return true;
	case 1://About
		return true;
	case 2://Customize
		return true;
	case 3:
		return true;
	case 4:
		return true;
	default:
		return false;
	}
}

wxGISEnumCommandKind wxGISCommonCmd::GetKind(void)
{
	switch(m_subtype)
	{
	case 0://Exit
		return enumGISCommandNormal;
	case 1://About
		return enumGISCommandNormal;
	case 2://Customize
		return enumGISCommandNormal;
	case 3://Separator
		return enumGISCommandSeparator;
	case 4://StatusBar
		return enumGISCommandCheck;
	default:
		return enumGISCommandNormal;
	}
}

wxString wxGISCommand::GetMessage(void)
{
	switch (m_subtype)
	{
	case 0:
		return wxString(_("Exit application"));
	case 1:
		return wxString(_("About application"));
	case 2:
		return wxString(_("Customize application"));
	case 3:
		return wxString(_("Separator"));
	case 4:
		return wxString(_("Show/hide statusbar"));
	default:
		return wxString();
	}
}

void wxGISCommonCmd::OnClick(void)
{
	switch(m_subtype)
	{
	case 0:
		{
			wxFrame* pFrame = dynamic_cast<wxFrame*>(m_pApp));
			if(pFrame)
				pFrame->Close();
			break;
		}
	case 1:
          m_pApp->OnAppAbout();
	case 2:
		m_pApp->Customize();
	case 4:
		m_pApp->ShowStatusBar(!m_pApp->IsStatusBarShow())
			break;
	case 3:
	default:
		return;
	}
}

bool wxGISCommonCmd::OnCreate(IApplication* pApp)
{
	m_pApp = pApp;
	return true;
}

wxString wxGISCommonCmd::GetTooltip(void)
{
	switch(m_subtype)
	{
	case 0:
		return wxString(_("Exit"));
	case 1:
		return wxString(_("About"));
	case 2:
		return wxString(_("Customize"));
	case 4:
		return wxString(_("Show/hide statusbar"));
	case 3:
	default:
		return wxString();
	}
}

unsigned char wxGISCommonCmd::GetCount(void)
{
	return 5;
}