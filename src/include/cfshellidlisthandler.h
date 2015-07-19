//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "cfhandler.h"
#include "pidl.h"
#include "util.h"


namespace MSF
{

// Note: handler expect that pidls are alive as long as handler is used.

class CCfShellIdListHandler : public CCfHandler
{
public:
    typedef std::vector<CPidl> CPidls;

    CCfShellIdListHandler(LPCITEMIDLIST pidlFolder, CPidls& pidls) :
        CCfHandler(CFSTR_SHELLIDLIST, true, false),
        m_pidlFolder(pidlFolder),
        m_pidls(pidls)
    {
    }


    virtual HRESULT Validate(const FORMATETC& formatetc) const
    {
        if (formatetc.dwAspect != DVASPECT_CONTENT)
            return DV_E_DVASPECT;

        if (!IsBitSet(formatetc.tymed, TYMED_HGLOBAL))
            return DV_E_TYMED;

        if (formatetc.lindex != -1)
            return DV_E_LINDEX;

        return S_OK;
    }


    virtual void GetData(const FORMATETC&, STGMEDIUM& stgmedium) const
    {
        UINT sizeheader = static_cast<UINT>(sizeof(CIDA) + (sizeof(UINT) * m_pidls.size()));

        UINT size = sizeheader + m_pidlFolder.GetSize();

        for (CPidls::const_iterator it = m_pidls.begin(); it != m_pidls.end(); ++it)
        {
            size += it->GetSize();
        }

        CStgMedium medium(GlobalAllocThrow(size));

        CIDA* pcida = static_cast<CIDA*>(medium.GetHGlobal());

        pcida->cidl = static_cast<UINT>(m_pidls.size());

        UINT offset = sizeheader;
        offset += AddPidlToCida(pcida, m_pidlFolder, 0, offset);

        for (UINT i = 0; i < m_pidls.size(); ++i)
        {
            offset += AddPidlToCida(pcida, m_pidls[i], i + 1, offset);
        }

        medium.Detach(stgmedium);
    }

private:

    CCfShellIdListHandler& operator=(const CCfShellIdListHandler&); // not implemented by design.

    unsigned int AddPidlToCida(CIDA* pcida, const CPidl& pidl, UINT index, UINT offset) const MSF_NOEXCEPT
    {
        pcida->aoffset[index] = offset;

        unsigned int size = pidl.GetSize();
        memcpy(reinterpret_cast<BYTE*>(pcida) + offset, pidl.get(), size);
        return size;
    }

    CPidl   m_pidlFolder;
    CPidls& m_pidls;
};

} // end of MSF namespace
