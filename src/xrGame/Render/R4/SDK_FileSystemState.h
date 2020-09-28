#pragma once

namespace Cordis
{
    namespace SDK
    {
		// Lord: всё время обновлять!
    struct SDK_FileSystemState
    {
        // @ That's about folders
 
        static bool m_is_staticgeometryempty;
        static bool m_is_groupsempty;
		static bool m_is_particlesempty;
		static bool m_is_levelsempty;
		static bool m_is_soundsempty;
		static bool m_is_texturesempty;
		static bool m_is_translationempty;
		static xr_string m_path_rawdata;
        static void MakeInfoAboutRawDataFolder(void);
    };
    }
}

