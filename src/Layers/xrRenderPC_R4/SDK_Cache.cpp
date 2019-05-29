#include "stdafx.h"
#include "SDK_Cache.h"
#include "SDKUI_Helpers.h"
#include "SDK_IconManager.h"
#include <filesystem>
#include <codecvt>
#include <shlwapi.h>
std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

void SDK_Cache::DeleteResources(void)
{
    for (xr_map<xr_string, CEditableObject*>::iterator it = this->LibStaticGeometry.begin();
         it != this->LibStaticGeometry.end(); ++it)
    {
        if (it->second)
        {
            delete it->second;
            it->second = nullptr;
        }

        this->LibStaticGeometry.erase(it->first);
    }
}

void SDK_Cache::MakeCache(void)
{
    this->MakeStaticGeometryCache();
    this->LoadDataForIconManager();
}

void SDK_Cache::UpdateCache(void) { this->UpdateStaticGeometryCache(); }

void SDK_Cache::MakeStaticGeometryCache(void)
{
    if (FS.IsRawDataEmpty() || SDK_FileSystemState::bStaticGeometryEmpty)
    {
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
        std::string full_path = converter.to_bytes(object.path().c_str());
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
        this->LibStaticGeometry.insert(std::pair<xr_string, CEditableObject*>(object_name, obj_value));
    }
}

void SDK_Cache::UpdateStaticGeometryCache(void) {}

CEditableObject* SDK_Cache::GetGeometry(LPCSTR name)
{
    if (!name)
        return nullptr;

    xr_map<xr_string, CEditableObject*>::iterator it = this->LibStaticGeometry.find(name);
    if (it != this->LibStaticGeometry.end())
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
                        xr_string file_str = converter.to_bytes(file.path().c_str()).c_str();

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
