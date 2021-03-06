﻿//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include <memory>

namespace msf
{

inline ATL::CComPtr<IEnumFORMATETC> SHCreateStdEnumFmtEtc(UINT cfmt, const FORMATETC* pformatetc)
{
    ATL::CComPtr<IEnumFORMATETC> renumformatetc;
    RaiseExceptionIfFailed(::SHCreateStdEnumFmtEtc(cfmt, pformatetc, &renumformatetc));
    return renumformatetc;
}


// Purpose: copy policy class: required for the ATL::CComEnumOnSTL template.
class CFORMATETCToFORMATETC
{
public:
    static void init(FORMATETC*) noexcept
    {
        // No init needed.
    }


    static HRESULT copy(FORMATETC* pTo, const FORMATETC* pFrom) noexcept
    {
        *pTo = *pFrom;
        return S_OK;
    }


    static void destroy(FORMATETC*) noexcept
    {
    }
};


class CEnumFORMATETC :
    public ATL::CComEnumOnSTL<IEnumFORMATETC,
                         &IID_IEnumFORMATETC,    // name and IID of enumerator interface
                         FORMATETC,              // type of object to return
                         CFORMATETCToFORMATETC,  // copy policy class
                         std::vector<FORMATETC>> // type of collection holding the data
{
public:
    using CFormatEtcs = std::vector<FORMATETC>;

    CEnumFORMATETC(const CEnumFORMATETC&) = delete;
    CEnumFORMATETC(CEnumFORMATETC&&) = delete;
    CEnumFORMATETC& operator=(const CEnumFORMATETC&) = delete;
    CEnumFORMATETC& operator=(CEnumFORMATETC&&) = delete;

    static ATL::CComPtr<CEnumFORMATETC> CreateInstance(std::unique_ptr<CFormatEtcs> qformatetcs)
    {
        ATL::CComObject<CEnumFORMATETC>* pEnum;
        RaiseExceptionIfFailed(ATL::CComObject<CEnumFORMATETC>::CreateInstance(&pEnum));

        ATL::CComPtr<CEnumFORMATETC> renum(pEnum);
        renum->Initialize(move(qformatetcs));

        return renum;
    }

protected:
    CEnumFORMATETC() noexcept
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"CEnumFORMATETC::CEnumFORMATETC (instance=%p)\n", this);
    }

    ~CEnumFORMATETC()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"CEnumFORMATETC::~CEnumFORMATETC (instance=%p)\n", this);
    }

private:
    void Initialize(std::unique_ptr<CFormatEtcs> qformatetcs) noexcept
    {
        m_qformatetcs = std::move(qformatetcs);

        ATLVERIFY(SUCCEEDED(__super::Init(this, *m_qformatetcs)));
    }

    // Member variables.
    std::unique_ptr<CFormatEtcs> m_qformatetcs;
};


// Purpose: alternative for the SHCreateStdEnumFmtEtc function.
//          Created before I discovered SHCreateStdEnumFmtEtc.
//          Source can act as sample, or as easy debug alternative.
inline HRESULT CreateStdEnumFmtEtc(std::unique_ptr<std::vector<FORMATETC> > qformatetcs, IEnumFORMATETC** ppenumFormatEtc)
{
    try
    {
        ATL::CComPtr<CEnumFORMATETC> renum = CEnumFORMATETC::CreateInstance(std::move(qformatetcs));
        *ppenumFormatEtc = renum.Detach();

        return S_OK;
    }
    catch (...)
    {
        return ExceptionToHResult();
    }
}

} // end of msf namespace
