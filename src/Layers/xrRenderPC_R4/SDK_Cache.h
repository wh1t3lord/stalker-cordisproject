#pragma once

class SDK_Cache
{
private:
    SDK_Cache(void) : RawDataPath(Core.WorkingPath) 
    {
      //  RawDataPath.append();
    }

public:
    inline static SDK_Cache& GetInstance(void) noexcept
    {
        static SDK_Cache instance;
        return instance;
    }

    ~SDK_Cache(void) noexcept = default;
    SDK_Cache(const SDK_Cache&) = delete;
    SDK_Cache& operator=(const SDK_Cache&) = delete;
    SDK_Cache(SDK_Cache&&) = delete;
    SDK_Cache& operator=(SDK_Cache&&) = delete;

    void MakeCache(void);
    void UpdateCache(void);

    inline CEditableObject* GetGeometry(xr_string str) { this->GetGeometry(str.c_str()); }
    CEditableObject* GetGeometry(LPCSTR name);
    bool IsSameObjectContains(void); // Lord: подумать нужно ли данный метод
    // SDK_CustomObject* CreateObjectStaticGeometry(void);
    void DeleteResources(void);
    void LoadDataForIconManager(void);
private:
    void MakeStaticGeometryCache(void);
    void UpdateStaticGeometryCache(void);

public:
    xr_string RawDataPath;
    xr_map<xr_string, CEditableObject*> LibStaticGeometry;
};
