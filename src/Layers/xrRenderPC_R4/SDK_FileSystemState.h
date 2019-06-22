#pragma once

namespace Cordis
{
    namespace SDK
    {
    struct SDK_FileSystemState
    {
        // @ That's about folders
        static bool m_is_staticgeometryempty;
        static bool m_is_groupsempty;
		static bool m_is_particlesempty;
		static xr_string m_path_rawdata;
        static void MakeInfoAboutRawDataFolder(void);
    };
    }
}

