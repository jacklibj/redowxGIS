#include "wxgis/catalogui/gxcontentview.h"
#include "../../art/cont_view_16.xpm"
#include "../../art/cont_view_48.xpm"

BEGIN_EVENT_TABLE(wxGxContentView, wxListCtrl)
	EVT_LIST_BEGIN_LABEL_EDIT(LISTCTRLID, wxGxContentView::OnBeginLabelEdit)