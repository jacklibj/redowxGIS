#include "wxgis/catalog/gxshapefactory.h"
#include "wxgis/catalog/gxdataset.h"

IMPLEMENT_DYNAMIC_CLASS(wxGxShapeFactory, wxObject)

wxGxShapeFactory::wxGxShapeFactory(void)
{

}

wxGxShapeFactory::~wxGxShapeFactory(void)
{

}

bool wxGxShapeFactory::GetChildren(wxString sParentDir, wxArrayString* pFileNames, GxObjectArray* pObjArray)
{
	typedef struct _data{
		wxString path;
		int bHasShp, bHasDbf, bHasPrj;
	}DATA;
	std::map<wxString, DATA> data_map;

    for (size_t i = 0; i < pFileNames->GetCount(); i++)
    {
		wxString path = pFileNames->Item(i);
		//test is dir
		if(wxFileName::DirExists(path))
			continue;

		path.MakeLower();
		wxString name, ext;
		wxFileName::SplitPath(path, NULL, NULL, &name, &ext);

		if(data_map[name].bHasShp != 1)
			data_map[name].bHasShp = (ext == wxT("shp")) ? 1 : 0;
		if(data_map[name].bHasDbf != 1)
			data_map[name].bHasDbf = (ext == wxT("dbf")) ? 1 : 0;
		if(data_map[name].bHasPrj != 1)
			data_map[name].bHasPrj = (ext == wxT("prj")) ? 1 : 0;
		if(data_map[name].path.IsEmpty() && (data_map[name].bHasShp || data_map[name].bHasDbf || data_map[name].bHasPrj))
			data_map[name].path = sParentDir+ wxFileName::GetPathSeparator() + name;
		if(data_map[name].path.IsEmpty())	
			data_map.erase(name);

		if(path.Find(wxT(".shp")) != wxNOT_FOUND)
			goto REMOVE;
		if(path.Find(wxT(".dbf")) != wxNOT_FOUND)
			goto REMOVE;
		if(path.Find(wxT(".prj")) != wxNOT_FOUND)
			goto REMOVE;
		if(path.Find(wxT(".sbn")) != wxNOT_FOUND)
			goto REMOVE;
		if(path.Find(wxT(".sbx")) != wxNOT_FOUND)
			goto REMOVE;
		if(path.Find(wxT(".shx")) != wxNOT_FOUND)
			goto REMOVE;
		continue;
REMOVE:
		pFileNames->RemoveAt(i);
		i--;
    }

	for (std::map<wxString, DATA>::const_iterator CI = data_map.begin(); CI != data_map.end(); ++CI)
	{
		IGxObject* pGxObj = NULL;

		if (CI->second.bHasShp)
		{
			wxString name;
			if(m_pCatalog->GetShowExt())
				name = CI->first + wxT(".shp");
			else
				name = CI->first;
			wxString path = CI->second.path + wxT(".shp");
			//create shp
			wxGxShapefileDataset* pDataset = new wxGxShapefileDataset(path, name, enumESRIShapefile);
			pGxObj = dynamic_cast<IGxObject*>(pDataset);
		}
		if (CI->second.bHasDbf && !CI->second.bHasShp)
		{
			wxString name = CI->first + wxT(".dbf");
			wxString path = CI->second.path + wxT(".dbf");
			//create dbf
			wxGxDataset* pDataset = new wxGxDataset(path, name, enumGISTableDataset);
			pGxObj = dynamic_cast<IGxObject*>(pDataset);
		}
		if (CI->second.bHasPrj && !CI->second.bHasShp)
		{
			CI->first + wxT(".prj");
			//create prj
		}
		if(pGxObj != NULL)
			pObjArray->push_back(pGxObj);
	}
	return true;

}
