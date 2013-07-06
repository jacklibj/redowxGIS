/******************************************************************************
 * Project:  wxGIS (GIS Catalog)
 * Purpose:  wxGxLocationComboBox class
 * Author:   Bishop (aka Barishnikov Dmitriy), polimax@mail.ru
 ******************************************************************************
*   Copyright (C) 2009  Bishop
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include "wxgis/catalogui/catalogui.h"
#include "wxgis/catalogui/gxapplication.h"

class wxGxLocationComboBox :
	public wxComboBox,
	public IGxSelectionEvents,
	public IToolBarControl
{
public:
	wxGxLocationComboBox(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size, const wxArrayString& choices, long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxT("LocationComboBox"));
	virtual ~wxGxLocationComboBox(void);
	//events
	void OnTextEnter(wxCommandEvent& event);
	//IGxSelectionEvent
	virtual void OnSelectionChanged(IGxSelection* Selection, long nInitiator);
	//IToolBatControl
	virtual void Activate(IApplication* pApp);
	virtual void Deactivate(void);
protected:
	IApplication* m_pApp;
	std::vector<wxString> m_ValuesArr;
	IConnectionPointContainer* m_pConnectionPointSelection;
	long m_ConnectionPointSelectionCookie;

	DECLARE_EVENT_TABLE()

};