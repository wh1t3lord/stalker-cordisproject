#include "stdafx.h"

#include "SDK_SceneManager.h"
#include "SDKUI_RightWindow.h"
#include "SDKUI_Helpers.h"
#include "xrEngine/SDK_Camera.h"

constexpr float GizmoDetection = 1.5;
SDK_SceneManager::~SDK_SceneManager(void) {}

void SDK_SceneManager::AddObject(const Fvector& p, const Fvector& n)
{
    // @ Типы секций...
    if (SDKUI_RightWindow::Widget().GetSection() == 0)
    {
        if (!SDKUI_RightWindow::Widget().GetCurrentSelectedStaticObject().empty())
        {
            SDK_CustomObject* object =
                this->_AddObjectStaticGeometry(SDKUI_RightWindow::Widget().GetCurrentSelectedStaticObject().c_str());
            object->MoveTo(p, n);
            this->m_objects_list.push_back(object);
        }
        else
        {
            // Lord: сюда modal window
        }
    }
}

void SDK_SceneManager::ZoomExtentSelected(const Fbox& bbox) {}

SDK_CustomObject* SDK_SceneManager::_AddObjectStaticGeometry(LPCSTR name)
{
    if (!name)
        return nullptr;

    // Lord: сюда добавить генерацию имени
    SDK_ObjectStaticGeometry* b = new SDK_ObjectStaticGeometry("lel");
    CEditableObject* o = SDK_Cache::GetInstance().GetGeometry(name);
    if (o == nullptr)
    {
        SDKUI_Log::Widget().AddText("Geometry was null!");
    }

    b->SetGeometry(o);

    return b;
}
// Lord: Этот метод вообще удалить
// void SDK_SceneManager::PickObject(const Fvector& start, const Fvector& direction)
// {
//     this->current_distance_to_object = SDK_Camera::GetInstance().fFar;
//     for (xr_list<SDK_CustomObject*>::value_type it : this->ObjectList)
//     {
//         it->bSelected = it->RayPick(this->current_distance_to_object, start, direction);
//     }
// }

SDK_CustomObject* SDK_SceneManager::Selection(const Fvector& start, const Fvector& direction, bool bSingleSelection)
{
    this->current_distance_to_object = SDK_Camera::GetInstance().fFar;

    if (bSingleSelection)
    {
        for (xr_list<SDK_CustomObject*>::value_type it : this->m_selectedobjects_list)
        {
            it->m_is_selected = false;
        }

        this->m_selectedobjects_list.clear();
        // @ Lord добавлять сюда
    }

    SDK_CustomObject* obj = nullptr;
    for (xr_list<SDK_CustomObject*>::value_type it : this->m_objects_list)
    {
        if (it->RayPick(this->current_distance_to_object, start, direction) && !it->m_is_selected)
        {
            obj = it;
        }
    }

    // @ Lord: подумать над дизайном, нормально ли это, скорее нет
    if (obj)
    {
        obj->m_is_selected = true;
        this->m_selectedobjects_list.push_back(obj);
    }

    return obj;
}

void SDK_SceneManager::DeSelection(const Fvector& start, const Fvector& direction)
{
    this->current_distance_to_object = SDK_Camera::GetInstance().fFar;

    SDK_CustomObject* obj = nullptr;
    for (xr_list<SDK_CustomObject*>::value_type it : this->m_objects_list)
    {
        if (it->RayPick(this->current_distance_to_object, start, direction))
        {
            obj = it;
        }
    }

    // @ Lord: подумать над дизайном, нормально ли это, скорее нет
    if (obj)
    {
        obj->m_is_selected = false;
        this->m_selectedobjects_list.remove(obj);
    }
}

void SDK_SceneManager::UnSelectAll(void)
{
    if (!this->m_selectedobjects_list.size())
        return;

    for (xr_list<SDK_CustomObject*>::iterator it = this->m_selectedobjects_list.begin();
        it != this->m_selectedobjects_list.end(); ++it)
    {
        (*it)->m_is_selected = false;
    }


    this->m_selectedobjects_list.clear();
}
AxisType SDK_SceneManager::SelectionAxisMove(void)
{
    float current_distance = 10000000;
    AxisType id_ = GIZMO_UNKNOWN;
    for (size_t i = 0; i < 3; i++)
    {
        // @ Have intersection
        float distance = GizmoMove[i].RayPick(SDKUI::UI().GetmPos(), SDKUI::UI().GetmDir());
        if (current_distance > distance)
        {
            current_distance = distance;
            id_ = GizmoMove[i].m_axis_id;
        }
        //   SDKUI_Log::Widget().AddText("%d %s", i,std::to_string(GizmoMove[i].RayPick(SDK_Camera::GetInstance().fFar,
        //   SDKUI::UI().GetmPos(), SDKUI::UI().GetmDir())).c_str());
    }
    //  SDKUI_Log::Widget().AddText("%f", current_distance);
    // SDKUI_Log::Widget().AddText("%d", id_);

    if (current_distance > GizmoDetection)
        id_ = GIZMO_UNKNOWN;

    for (unsigned int i = 0; i < 3; ++i)
    {
        if (GizmoMovePlanes[i].RayPick(SDKUI::UI().GetmPos(), SDKUI::UI().GetmDir()))
            id_ = GizmoMovePlanes[i].m_axis_id;
    }
    //   this->gizmo_distance = current_distance;
    return id_;
}

void SDK_SceneManager::Move(const Fvector& vec)
{
    if (!this->m_selectedobjects_list.size())
        return;

    for (xr_list<SDK_CustomObject*>::iterator it = this->m_selectedobjects_list.begin();
         it != this->m_selectedobjects_list.end(); ++it)
    {
        (*it)->Move(vec);
    }
}

void SDK_SceneManager::Rotate(const Fvector& vec)
{
    if (!this->m_selectedobjects_list.size())
        return;
    // @ Lord реализовать нормально
/*
    for (xr_list<SDK_CustomObject*>::iterator it = this->SelectedObjectsList.begin(); it != this->SelectedObjectsList.end(); ++it)
    {
        (*it)->RotateLocal(vec);
    }*/
}

void SDK_SceneManager::Scale(const Fvector& vec)
{
    if (!this->m_selectedobjects_list.size())
        return;

    for (xr_list<SDK_CustomObject*>::iterator it = this->m_selectedobjects_list.begin();
        it != this->m_selectedobjects_list.end(); ++it)
    {
        (*it)->Scale(vec);
    }
}
