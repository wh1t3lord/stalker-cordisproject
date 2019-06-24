#include "stdafx.h"
#include "SDK_ObjectStaticGeometry.h"
#include "SDKUI_Helpers.h"
namespace Cordis
{
    namespace SDK
    {
    SDK_ObjectStaticGeometry::SDK_ObjectStaticGeometry(LPCSTR name) : inherited(name), m_data(nullptr)
    {
        this->m_id_objecttype = kSection_StaticGeometry;
        this->m_occbox.invalidate();
    }

    SDK_ObjectStaticGeometry::~SDK_ObjectStaticGeometry(void)
    {
        if (this->m_data)
            delete this->m_data;
    }

    void SDK_ObjectStaticGeometry::Render(const int& prior, const bool& strit)
    {
        if (!this->m_data)
        {
            SDKUI_Log::Widget().SetColor(warning);
            SDKUI_Log::Widget().AddText(TEXT("Can't draw data, because it was null!"));
            return;
        }

        if (this->m_is_updatetransform)
        {
            this->UpdateTransform();
            // this->Box.xform(this->GetTransform());
            this->m_occbox.set(this->m_data->GetBox());
            this->m_occbox.xform(this->getTransform());
        }

        // Fbox b = this->Box;
        // b.xform(this->GetTransform());
        //     if (RImplementation.occ_visible(this->OccBox))
        //     {
        this->m_data->Render(this->getTransform(), prior, strit);
        //         this->bRendering = true;
        //     }
        //     else
        //     {
        //         this->bRendering = false;
        //     }

        this->m_is_updatetransform = false;
    }

    void SDK_ObjectStaticGeometry::Load(LPCSTR model_name)
    {
        if (!model_name)
        {
            SDKUI_Log::Widget().SetColor(warning);
            SDKUI_Log::Widget().AddText(TEXT("Can't load because path is invalid!"));
            return;
        }

        this->m_data = new CEditableObject(TEXT("")); // Lord: нормально ли так TEXT("")

        if (!this->m_data->Load(model_name))
        {
            SDKUI_Log::Widget().SetColor(error);
            SDKUI_Log::Widget().AddText(TEXT("Can't load model error!"));
            return;
        }
    }

    void SDK_ObjectStaticGeometry::SetGeometry(CEditableObject* obj)
    {
        if (!obj)
        {
            SDKUI_Log::Widget().AddText("Can't SetGeometry because obj was null");
            return;
        }

        this->m_data = obj;
        this->m_box = this->m_data->GetBox();
    }

    bool SDK_ObjectStaticGeometry::RayPick(float& distance, const Fvector& start, const Fvector& direction)
    {
        if (!this->m_data)
        {
            return false;
        }

        Fmatrix _matrix = this->getTransform();
		_matrix.invert();
        if (RImplementation.occ_visible(this->m_occbox))
        {
            if (this->m_data->RayPick(distance, start, direction, _matrix))
            {
                return true;
            }
        }

        return false;
    }

    bool SDK_ObjectStaticGeometry::FrustumPick(const CFrustum& frustum)
    { 
        if (!this->m_data)
            return false;

        if (RImplementation.occ_visible(this->m_occbox))
        {
            return this->m_data->FrustumPick(frustum, this->getTransform());
        }

        return false;
    } 

    }
}
