#include "stdafx.h"
#include "SDK_FileSystemState.h"
#include <shlwapi.h>

bool SDK_FileSystemState::bStaticGeometryEmpty;
bool SDK_FileSystemState::bGroupsEmpty;

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
        bStaticGeometryEmpty = true;
    }
    else
    {
        bStaticGeometryEmpty = false;
    }

    if (PathIsDirectoryEmptyA((path + xr_string(groups_folder)).c_str()))
    {
        bGroupsEmpty = true;
    }
    else
    {
        bGroupsEmpty = false;
    }
    #endif
}
