#pragma once

#include "catalog.h"
#include "wx/imaglist.h"

//
//
//
class WXDLLIMPEXP_GIS_CLT wxGxDataset :
	public IGxObject,
	public IGxObjectUI,
	public IGxObjectEdit,
	public IGxDataset
{
public:
	wxGxDataset(wxString Path, wxString Name, wxGISEnumDatasetType Type);
	virtual ~wxGxDataset(void);
	//
	virtual wxString GetName(void){return m_sName;};
	virtual wxString GetCategory(void) {return wxString(_("Table"));};
	//IGxObjectUI
	virtual wxIcon GetLargeImage(void);
	virtual wxIcon GetSmallImage(void);
	virtual wxString ContextMenu(void) {return wxString(wxT("wxGxDataset.ContextMenu"));};
	virtual wxString NewMenu(void) {return wxString(wxT("wxGxDataset.NewMenu"));}
	//IGxObjectEdit
	virtual bool Delete(void);
	virtual bool CanDelete(void){return true;};
	virtual bool Rename(wxString NewName);
	virtual bool CanRename(void){return true;};
	virtual void EditProperties(wxWindow * parent);
	//IGxDataset
	virtual wxGISDataset* GetDataset(void);
	virtual wxGISEnumDatasetType GetType(void){return m_type;};
protected:
	wxString m_sName, m_sPath;
	wxImageList m_ImageListSmall, m_ImageListLarge;
	wxGISDataset* m_pwxGISDataset;
	wxGISEnumDatasetType m_type;
};

//
//
//
class WXDLLIMPEXP_GIS_CLT wxGxShapefileDataset :
	public IGxObject,
	public IGxObjectUI,
	public IGxObjectEdit,
	public IGxDataset
{
public:
	wxGxShapefileDataset(wxString Path, wxString Name, wxGISEnumShapefileDatasetType Type);
	virtual ~wxGxShapefileDataset(void);
	//IGxObject
	virtual wxString GetName(void){return m_sName;};
	virtual wxString GetCategory(void);
	//IGxObjectUI
	virtual wxIcon GetLargeImage(void);
	virtual wxIcon GetSmallImage(void);
	virtual wxString ContextMenu(void){return wxString(wxT("wxGxShapefileDataset.ContextMenu"));};
	virtual wxString NewMenu(void){return wxString(wxT("wxGxShapefileDataset.NewMenu"));};
	//IGxObjectEdit
	virtual bool Delete(void);
	virtual bool CanDelete(void){return true;};
	virtual bool Rename(wxString NewName);
	virtual bool CanRename(void){return true;};
	virtual void EditProperties(wxWindow *parent);
	//IGxDataset
	virtual wxGISDataset* GetDataset(void);
	virtual wxGISEnumDatasetType GetType(void){return enumGISFeatureDataset;};
protected:
	wxString m_sName, m_sPath;
	wxImageList m_ImageListSmall, m_ImageListLarge;
	wxGISDataset* m_pwxGISDataset;
	wxGISEnumShapefileDatasetType m_type;
};
//
//
//
class WXDLLIMPEXP_GIS_CLT wxGxRasterDataset :
	public IGxObject,
	public IGxObjectUI,
	public IGxObjectEdit,
	public IGxDataset
{
public:
	wxGxRasterDataset(wxString Path, wxString Name, wxGISEnumRasterType Type);
	virtual ~wxGxRasterDataset(void);
	//IGxObject
	virtual wxString GetName(void){return m_sName;};
	virtual wxString GetCategory(void);
	//IGxObjectUI
	virtual wxIcon GetLargeImage(void);
	virtual wxIcon GetSmallImage(void);
	virtual wxString ContextMenu(void){return wxString(wxT("wxGxRasterDataset.ContextMenu"));};
	virtual wxString NewMenu(void){return wxString(wxT("wxGxRasterDataset.NewMenu"));};
	//IGxObjectEdit
	virtual bool Delete(void);
	virtual bool CanDelete(void){return true;};
	virtual bool Rename(wxString NewName);
	virtual bool CanRename(void){return true;};
	virtual void EditProperties(wxWindow *parent);
	//IGxDataset
	virtual wxGISDataset* GetDataset(void);
	virtual wxGISEnumDatasetType GetType(void){return enumGISRasterDataset;};
protected:
	wxString m_sName, m_sPath;
	wxImageList m_ImageListSmall, m_ImageListLarge;
	wxGISDataset* m_pwxGISDataset;
	wxGISEnumRasterDatasetType m_type;


};