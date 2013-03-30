#pragma once

#include "catalog.h"

//----------------
// wxGxSelection
//-----------------

class WXDLLIMPEXP_GIS_CLT wxGxSelection :
	public IConnectionPointContainer,
	public IGxSelection
{
public:
	wxGxSelection(void);
	virtual ~wxGxSelection(void);
	//IGxSelction
	virtual void Select( IGxObject* pObject, bool appendToExistingSelection, long nInitiator);
	virtual void Unselect(IGxObject* pObject, long nInitiator);
	virtual void Clear( long nInitiator);
	//IConnectionPointContainer
	virtual long Advise(wxObject* pObject);
};
