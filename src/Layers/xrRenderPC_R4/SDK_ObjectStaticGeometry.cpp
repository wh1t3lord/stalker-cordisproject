#include "stdafx.h"
#include "SDK_ObjectStaticGeometry.h"
#include "SDKUI_Helpers.h"

SDK_ObjectStaticGeometry::SDK_ObjectStaticGeometry(LPCSTR name)
    : inherited(name), Data(nullptr)
{
    this->ObjectType = OBJECT_CLASS_STATIC_GEOMETRY;
    this->OccBox.invalidate();
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
        // this->Box.xform(this->GetTransform());
        this->OccBox.set(this->Data->GetBox());
        this->OccBox.xform(this->GetTransform());
    }

    // Fbox b = this->Box;
    // b.xform(this->GetTransform());
    //     if (RImplementation.occ_visible(this->OccBox))
    //     {
    this->Data->Render(this->GetTransform(), prior, strit);
    //         this->bRendering = true;
    //     }
    //     else
    //     {
    //         this->bRendering = false;
    //     }

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

void SDK_ObjectStaticGeometry::DrawPreferences(void) { SDKUI_StaticGeometryPref::Widget().Draw(this); }

void SDK_ObjectStaticGeometry::SetGeometry(CEditableObject* obj)
{
    if (!obj)
    {
        SDKUI_Log::Widget().AddText("Can't SetGeometry because obj was null");
        return;
    }

    this->Data = obj;
    this->Box = this->Data->GetBox();
}

bool SDK_ObjectStaticGeometry::RayPick(float& distance, const Fvector& start, const Fvector& direction) 
{
    if (!this->Data)
    {
        return false;
    }

    Fmatrix _m = this->GetTransform();
    _m.invert();
    if (RImplementation.occ_visible(this->OccBox))
    {
        if (this->Data->RayPick(distance, start, direction, _m))
        {
            return true;
        }
    }

    return false;
}
