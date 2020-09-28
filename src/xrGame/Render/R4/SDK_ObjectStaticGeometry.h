#pragma once
 
#include "SDK_CustomObject.h"


namespace Cordis
{
    namespace SDK
    {
    class SDK_ObjectStaticGeometry : public SDK_CustomObject
    {
    private:
        using inherited = SDK_CustomObject;

    public:
        SDK_ObjectStaticGeometry(LPCSTR name);
        SDK_ObjectStaticGeometry(void) = delete;
        ~SDK_ObjectStaticGeometry(void);

        void Render(const int&, const bool&);
        void Load(LPCSTR model_name);
        void SetGeometry(CEditableObject*);
        bool RayPick(float& distance, const Fvector& start, const Fvector& direction);
        bool FrustumPick(const CFrustum& frustum);
        inline CEditableObject* GetData(void) const { return this->m_data; }
        inline EditMeshVec* Meshes(void) const { return this->m_data ? &this->m_data->Meshes() : nullptr; }
        inline LPCSTR GetDataName(void) const { return this->m_data ? this->m_data->GetName() : ""; }

    private:
        Fbox m_occbox;
        CEditableObject* m_data;
    };
    }
}

