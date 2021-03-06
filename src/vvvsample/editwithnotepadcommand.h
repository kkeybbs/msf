﻿//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include <msf.h>


class EditWithNotepadCommand : public msf::ContextMenuCommand
{
public:
    void operator()(const CMINVOKECOMMANDINFO* /* pici */, const std::vector<std::wstring>& filenames) override
    {
        ATLASSERT(filenames.size() == 1); // can only handle 1 file.

        // Use the command line parameters to pass the exe filename. This causes
        // Windows to use the path to find notepad.
        auto command = L"notepad.exe \"" + filenames[0] + L"\"";

        msf::CreateProcess(nullptr, command);
    }
};
