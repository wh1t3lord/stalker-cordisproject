#pragma once

class SDK_Settings
{
public:
    SDK_Settings(void) = default;
    inline static SDK_Settings& GetInstance(void) noexcept
    {
        static SDK_Settings instance;
        return instance;
    }
    ~SDK_Settings(void);

    void Initialize(void);

    void RestoreToDefaultAll(void);
    void RestoreToDefaultScene(void);
    void RestoreToDefaultKeyboard(void);
    void RestoreToDefaultCamera(void);

    void LoadSettings(void);
    void SaveSettings(void);

private:
    void CreateDefaultSettingsFiles(void);
    void CreateDefaultSettingsKeyboard(void);
    void CreateDefaultSettingScene(void);
    void CreateDefaultSettingsCamera(void);

public:
    bool IsFirstTime;

private:
    xr_string current_path;
};
