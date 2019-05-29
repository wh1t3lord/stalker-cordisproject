#include "stdafx.h"
#include "SDK_Settings.h"
#include <filesystem>
#include <Windows.h>
#include <ShlObj.h>
#include <fstream>

SDK_Settings::~SDK_Settings(void) {}

void SDK_Settings::Initialize(void)
{
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
    {
        xr_string cpath = szPath;
        cpath += "\\CordisProject_SDK";
        this->current_path = cpath;

        if (std::filesystem::exists(cpath))
        {
        }
        else
        {
            if (std::filesystem::create_directory(this->current_path))
            {
            }
            else
            {
                R_ASSERT(TEXT("Can't create a folder for CordisProject SDK!"));
            }
        }
    }
    else
    {
        R_ASSERT(TEXT("Can't obtain the folder! (appdata)"));
    }
}

void SDK_Settings::CreateDefaultSettingsFiles(void)
{
    if (!this->current_path.size())
    {
        R_ASSERT(TEXT("Can't create a default file for keyboard!") && 0);
        return;
    }
}

void SDK_Settings::CreateDefaultSettingsCamera(void)
{
    if (!this->current_path.size())
    {
        R_ASSERT(TEXT("Can't create a default file for keyboard!") && 0);
        return;
    }
}

void SDK_Settings::CreateDefaultSettingScene(void)
{
    if (!this->current_path.size())
    {
        R_ASSERT(TEXT("Can't create a default file for keyboard!") && 0);
        return;
    }
}

void SDK_Settings::CreateDefaultSettingsKeyboard(void)
{
    if (!this->current_path.size())
    {
        R_ASSERT(TEXT("Can't create a default file for keyboard!") && 0);
        return;
    }

    std::fstream file;
    xr_string _path = this->current_path;
    _path += "\\SettingsKeyboard.ini";
    file.open(_path.c_str(), std::ios::out);

    if (file.good())
    {
        file << "# Author: Lord" << std::endl;
        file << "[Keyboard]" << std::endl;
        file << "OpenFile = ";
        file << std::endl;
        file << "OpenProject = ";
        file << std::endl;
        file << "CloseProgram = ";
        file << std::endl;
        file << "ClearAllScene = ";
        file << std::endl;
        file << "DeleteObject = ";
        file << std::endl;
        file << "AddTool = ";
        file << std::endl;
        file << "MoveTool = ";
        file << std::endl;
        file << "RotateTool = ";
        file << std::endl;
        file << "ScaleTool = ";
        file << std::endl;
        file << "ZoomAtObject = ";
        file << std::endl;
        file << "SaveScene = ";
        file << std::endl;
        file << "CameraManagerMenu = ";
        file << std::endl;
        file << "Help = ";
        file << std::endl;
        file << "LogManagerMenu = ";
        file << std::endl;
        file << "OpenRecent = ";
        file << std::endl;
        file << "ObjectList = ";
        file << std::endl;
        file << "Preferences = ";
        file << std::endl;
    }
    else
    {
        R_ASSERT(TEXT("Can't create a file for SettingsKeyboard.ini"));
    }

    file.close();
}
