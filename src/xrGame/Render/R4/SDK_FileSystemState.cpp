#include "StdAfx.h"
#include "SDK_FileSystemState.h"
#include <shlwapi.h>

namespace Cordis
{
    namespace SDK
    {
    bool SDK_FileSystemState::m_is_staticgeometryempty;
    bool SDK_FileSystemState::m_is_groupsempty;
	bool SDK_FileSystemState::m_is_levelsempty;
	bool SDK_FileSystemState::m_is_particlesempty;
	bool SDK_FileSystemState::m_is_soundsempty;
	bool SDK_FileSystemState::m_is_translationempty;
	bool SDK_FileSystemState::m_is_texturesempty;
	xr_string SDK_FileSystemState::m_path_rawdata;

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

		m_path_rawdata = path;

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

		if (PathIsDirectoryEmptyA((path + xr_string(sounds_folder)).c_str()))
		{
			m_is_soundsempty = true;
		}
		else
		{
			m_is_soundsempty = false;
		}

		if (PathIsDirectoryEmptyA((path + xr_string(particles_folder)).c_str()))
		{
			m_is_particlesempty = true;
		}
		else
		{
			m_is_particlesempty = false;
		}

		if (PathIsDirectoryEmptyA((path + xr_string(textures_folder)).c_str()))
		{
			m_is_texturesempty = true;
		}
		else
		{
			m_is_texturesempty = false;
		}

		if (PathIsDirectoryEmptyA((path + xr_string(translation_folder)).c_str()))
		{
			m_is_translationempty = true;
		}
		else
		{
			m_is_translationempty = false;
		}
#endif
    }

    }
}
