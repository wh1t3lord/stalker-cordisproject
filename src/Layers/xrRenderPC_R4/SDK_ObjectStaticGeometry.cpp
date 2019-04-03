#include "stdafx.h"
#include "SDK_ObjectStaticGeometry.h"
#include "SDKUI_Helpers.h"


SDK_ObjectStaticGeometry::SDK_ObjectStaticGeometry(LPVOID data, LPCSTR name) : inherited(data, name)
{
    this->Data = nullptr;
    this->ObjectType = OBJECT_CLASS_STATIC_GEOMETRY;
    this->SceneName = name;
}

SDK_ObjectStaticGeometry::~SDK_ObjectStaticGeometry(void)
{
    if (this->Data)
        delete this->Data;
}

void SDK_ObjectStaticGeometry::Render(const int& prior, const bool& strit)
{
    if (!this->Data)
    {
        SDKUI_Log::Widget().SetColor(warning);
        SDKUI_Log::Widget().AddText(TEXT("Can't draw data, because it was null!"));
        return;
    }

    if (this->bUpdateTransform)
    {
        this->UpdateTransform();
    }
    this->Data->Render(this->GetTransform(), prior, strit);

    this->bUpdateTransform = false;
}

void SDK_ObjectStaticGeometry::Load(LPCSTR model_name)
{
    if (!model_name)
    {
        SDKUI_Log::Widget().SetColor(warning);
        SDKUI_Log::Widget().AddText(TEXT("Can't load because path is invalid!"));
        return;
    }

    this->Data = new CEditableObject(TEXT("")); // Lord: нормально ли так TEXT("")

    if (!this->Data->Load(model_name))
    {
        SDKUI_Log::Widget().SetColor(error);
        SDKUI_Log::Widget().AddText(TEXT("Can't load model error!"));
        return;
    }
}

void SDK_ObjectStaticGeometry::DrawPreferences(void)
{ 
    SDKUI_StaticGeometryPref::Widget().Draw(this);
}
