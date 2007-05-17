//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"
#include "../include/shellpropsheetextimpl.h"
#include "propertypagevvv.h"
#include "shellpropsheetextclsid.h"
#include "shellfolderclsid.h"
#include "resource.h"


class ATL_NO_VTABLE CShellPropSheetExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CShellPropSheetExt, &__uuidof(CShellPropSheetExt)>,
	public IShellPropSheetExtImpl<CShellPropSheetExt>
{
public:
	static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()
	{
		return IShellPropSheetExtImpl<CShellPropSheetExt>::UpdateRegistry(
			IDR_PROPERTYSHEETEXT, bRegister,
			L"Sample PropertySheet Extension", __uuidof(CShellFolder), wszVVVExtension);
	}

	BEGIN_COM_MAP(CShellPropSheetExt)
		COM_INTERFACE_ENTRY(IShellExtInit)
		COM_INTERFACE_ENTRY(IShellPropSheetExt)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()


	CShellPropSheetExt()
	{
		RegisterExtension(tszVVVExtension);
	}


	// Purpose: called by MSF when it is time to add our pages to the propertysheet.
	void OnAddPages(const CAddPage& addpage, const vector<CString>& filenames)
	{
		// Only add the page if only 1 file is selected and is of our own extension.
		if (filenames.size() != 1 || ContainsUnknownExtension(filenames))
			return;

		addpage(CPropertyPageVVV::CreateInstance(filenames[0]));
	}
};


OBJECT_ENTRY_AUTO(__uuidof(CShellPropSheetExt), CShellPropSheetExt)