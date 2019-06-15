#include "stdafx.h"
#include "SDK_FileSystemState.h"
#include <shlwapi.h>

namespace Cordis
{
    namespace SDK
    {
    bool SDK_FileSystemState::m_is_staticgeometryempty;
    bool SDK_FileSystemState::m_is_groupsempty;

    void SDK_FileSystemState::MakeInfoAboutRawDataFolder(void)
    {
        xr_string path = Core.WorkingPath;
        if (Core.bRunningOutsideDirectory)
        {
            path.append("rawdata");
        }
        else
        {
            path.append("\\rawdata");
        }

#ifdef _WIN32
        if (PathIsDirectoryEmptyA((path + xr_string(sg_folder)).c_str()))
        {
            m_is_staticgeometryempty = true;
        }
        else
        {
            m_is_staticgeometryempty = false;
        }

        if (PathIsDirectoryEmptyA((path + xr_string(groups_folder)).c_str()))
        {
            m_is_groupsempty = true;
        }
        else
        {
            m_is_groupsempty = false;
        }
#endif
    }

    }
}
