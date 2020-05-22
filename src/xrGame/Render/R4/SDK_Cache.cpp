#include "StdAfx.h"
#include "SDK_Cache.h"
#include "SDKUI_Helpers.h"
#include "SDK_IconManager.h"
#include <filesystem>
#include <shlwapi.h>

namespace Cordis
{
    namespace SDK
	{
    void SDK_Cache::DeleteResources(void)
    {
        for (xr_map<xr_string, CEditableObject*>::iterator it = this->m_staticgeometry.begin();
             it != this->m_staticgeometry.end(); ++it)
        {
            if (it->second)
            {
                delete it->second;
                it->second = nullptr;
            }

            this->m_staticgeometry.erase(it->first);
        }
    }

    void SDK_Cache::MakeCache(void)
    {
        this->MakeStaticGeometryCache();
		this->MakeSpawnElementsCache();
        this->LoadDataForIconManager();
    }

    void SDK_Cache::UpdateCache(void) { this->UpdateStaticGeometryCache(); }

    void SDK_Cache::MakeStaticGeometryCache(void)
    {
        if (FS.IsRawDataEmpty() || SDK_FileSystemState::m_is_staticgeometryempty)
        {
            // @ Lord отсюда не убирать breakpoint до тех пор пока не поймёшь ошибку почему она здесь может появляться
            SDKUI_Log::Widget().SetColor(warning);
            SDKUI_Log::Widget().AddText("Can't load any object, because your folder 'Static Geometry' is empty!");
            return;
        }

        xr_string path = Core.WorkingPath;
        if (Core.bRunningOutsideDirectory)
        {
            path.append("rawdata");
            path.append(sg_folder);
        }
        else
        {
            path.append("\\rawdata");
            path.append(sg_folder);
        }
        for (std::filesystem::directory_entry object :
            std::filesystem::directory_iterator(path.c_str())) // Подумать над тем, когда мы будем вызывать из под папки
        {
            std::string full_path = Helper::converter.to_bytes(object.path().c_str());
            xr_string object_name = full_path.c_str();
            if (std::filesystem::is_directory(full_path))
                continue;

            // Lord: потом добавить функционал для thm
            if (strstr(full_path.c_str(), ".thm"))
            {
                continue;
            }

            object_name.erase(object_name.rfind('.'));
            object_name = object_name.substr(object_name.rfind('\\') + 1);

            CEditableObject* obj_value = new CEditableObject(TEXT(""));
            if (!obj_value->Load(full_path.c_str()))
            {
                SDKUI_Log::Widget().SetColor(error);
                SDKUI_Log::Widget().AddText(TEXT("Can't load static geometry object - %s"), object_name.c_str());
                delete obj_value;
            }
            this->m_staticgeometry.insert(std::pair<xr_string, CEditableObject*>(object_name, obj_value));
        }
    }

	void SDK_Cache::MakeSpawnElementsCache(void)
	{
		std::pair<xr_string, xr_string> cache_element;
		for (CInifile::Root::const_iterator it = pSettings->sections().begin(); it != pSettings->sections().end(); ++it)
		{
			LPCSTR value;
			if ((*it)->line_exist("$spawn", &value))
			{
				shared_str sdk_info = pSettings->r_string_wb((*it)->Name.c_str(), "$spawn");

				if (sdk_info.size())
					cache_element.first = sdk_info.c_str();
				else
					continue;

				if ((*it)->Name.size())
					cache_element.second = (*it)->Name.c_str(); // @ Section name 
				else 
					continue;

				this->m_spawnelements.push_back(cache_element);
			}
		}
	}

    void SDK_Cache::UpdateStaticGeometryCache(void) 
	{
	
	}

	void SDK_Cache::UpdateSpawnElementsCache(void)
	{

	}

    CEditableObject* SDK_Cache::GetGeometry(LPCSTR name)
    {
        if (!name)
            return nullptr;

        xr_map<xr_string, CEditableObject*>::iterator it = this->m_staticgeometry.find(name);
        if (it != this->m_staticgeometry.end())
        {
            return it->second;
        }
        else
        {
            SDKUI_Log::Widget().SetColor(error);
            SDKUI_Log::Widget().AddText(TEXT("The static object is not loaded or found"));
        }

        return nullptr;
    }

    bool SDK_Cache::IsSameObjectContains(void) { return false; }

    void SDK_Cache::LoadDataForIconManager(void)
    {
        xr_string path = Core.WorkingPath;

        if (Core.bRunningOutsideDirectory)
        {
            path.append("rawdata\\resources");
        }
        else
        {
            path.append("\\rawdata\\resources");
        }

        if (FS.dirExists(path))
        {
            if (PathIsDirectoryEmptyA(path.c_str()))
            {
                return;
            }
            else
            {
                xr_string path1 = path;
                path.append("\\tools");
                if (FS.dirExists(path))
                {
                    if (PathIsDirectoryEmptyA(path.c_str()))
                    {
                        SDKUI_Log::Widget().SetColor(warning);
                        SDKUI_Log::Widget().AddText("ToolBar can't have any image-icons you must add that icons!");
                    }
                    else
                    {
                        for (std::filesystem::directory_entry file : std::filesystem::directory_iterator(path.c_str()))
                        {
                            xr_string file_str = Helper::converter.to_bytes(file.path().c_str()).c_str();

                            if (file_str.substr(file_str.find('.')) == ".png")
                            {
                                SDK_IconManager::GetInstance().LoadImageToolsIcons(file_str, 32, 32);
                            }
                        }
                    }
                }
                else
                {
                    SDKUI_Log::Widget().SetColor(warning);
                    SDKUI_Log::Widget().AddText("ToolBar can't have any image-icons you must add that icons!");
                }
            }
        }

        return;
    }
    }
}

