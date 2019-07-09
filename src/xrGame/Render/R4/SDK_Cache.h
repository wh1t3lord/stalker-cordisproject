#pragma once

namespace Cordis
{
    namespace SDK
    {
    class SDK_Cache
    {
    private:
        SDK_Cache(void) noexcept : m_rawdata_path(Core.WorkingPath)
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
		inline const xr_map<xr_string, CEditableObject*>& getStaticGeometryCache(void) const noexcept { return this->m_staticgeometry; }
		inline const xr_vector<std::pair<xr_string, xr_string>>& getSpawnElementsCache(void) const noexcept { return this->m_spawnelements; }
        CEditableObject* GetGeometry(LPCSTR name);
        bool IsSameObjectContains(void); // Lord: подумать нужно ли данный метод
        // SDK_CustomObject* CreateObjectStaticGeometry(void);
        void DeleteResources(void);
        void LoadDataForIconManager(void);



    private:
        void MakeStaticGeometryCache(void);
        void UpdateStaticGeometryCache(void);
		void MakeSpawnElementsCache(void);
		void UpdateSpawnElementsCache(void);

    private:
        xr_string m_rawdata_path;
        xr_map<xr_string, CEditableObject*> m_staticgeometry;
		xr_vector<std::pair<xr_string, xr_string>> m_spawnelements;
    };
    }
}

