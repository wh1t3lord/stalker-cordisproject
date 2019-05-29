#pragma once
 
#include "SDK_CustomObject.h"
#include "SDKUI_ObjectPrefs.h"

 
class SDK_ObjectStaticGeometry : public SDK_CustomObject
{
    friend class SDKUI_StaticGeometryPref;

private:
    using inherited = SDK_CustomObject;

public:
    SDK_ObjectStaticGeometry(LPCSTR name);
    SDK_ObjectStaticGeometry(void) = delete;
    ~SDK_ObjectStaticGeometry(void);

    void Render(const int&, const bool&);
    void Load(LPCSTR model_name);
    void SetGeometry(CEditableObject*);
    inline CEditableObject* GetData(void) const { return this->Data; }
    inline EditMeshVec* Meshes(void) const { return this->Data ? &this->Data->Meshes() : nullptr; }
    inline LPCSTR GetDataName(void) const { return this->Data ? this->Data->GetName() : ""; }
    inline void HidePreferences(void) { SDKUI_StaticGeometryPref::Widget().Hide(); }
    inline void ShowPreferences(void) { SDKUI_StaticGeometryPref::Widget().Show(); }
    void DrawPreferences(void);
    bool RayPick(float& distance, const Fvector& start, const Fvector& direction);
 

private:
    Fbox OccBox;
    CEditableObject* Data;
};
