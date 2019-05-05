#include "stdafx.h"
#include "SDK_SceneManager.h"
#include "SDKUI_RightWindow.h"
#include "SDKUI_Helpers.h"
#include "xrEngine/SDK_Camera.h"

SDK_SceneManager::~SDK_SceneManager(void) {}

void SDK_SceneManager::AddObject(const Fvector& p, const Fvector& n)
{
    // @ Типы секций...
    if (SDKUI_RightWindow::Widget().GetSection() == 0)
    {
        if (!SDKUI_RightWindow::Widget().GetCurrentSelectedStaticObject().empty())
        {
            SDK_CustomObject* object = this->_AddObjectStaticGeometry(SDKUI_RightWindow::Widget().GetCurrentSelectedStaticObject().c_str());
            object->MoveTo(p, n);
            this->ObjectList.push_back(object);
        }
        else
        {
            // Lord: сюда modal window
        }
    }
}

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

SDK_CustomObject* SDK_SceneManager::SingleSelection(const Fvector& start, const Fvector& direction)
{
    this->current_distance_to_object = SDK_Camera::GetInstance().fFar;

     
    for (xr_list<SDK_CustomObject*>::value_type it : this->ObjectList)
    {
        it->bSelected = false;
    }

    SDK_CustomObject* obj = nullptr;
    for (xr_list<SDK_CustomObject*>::value_type it : this->ObjectList)
    {
        if (it->RayPick(this->current_distance_to_object, start, direction))
        {
            obj = it;
        }
    }
    
    // @ Lord: подумать над дизайном, нормально ли это, скорее нет
    if (obj)
        obj->bSelected = true;

    if (obj)
        this->CurrentObject = obj;

    return obj;
}

AxisType SDK_SceneManager::SelectionAxisMove(void)
{
    float current_distance = 10000000;
    AxisType id_ = GIZMO_X;
    for (size_t i = 0; i < 3; i++)
    {
        // @ Have intersection
        float distance = GizmoMove[i].RayPick(SDK_Camera::GetInstance().fFar, SDKUI::UI().GetmPos(), SDKUI::UI().GetmDir());
        if (distance < current_distance)
        {
            current_distance = distance;
            id_ = GizmoMove[i].id;
        }

     //   SDKUI_Log::Widget().AddText("%d %s", i,std::to_string(GizmoMove[i].RayPick(SDK_Camera::GetInstance().fFar, SDKUI::UI().GetmPos(), SDKUI::UI().GetmDir())).c_str());
    }

    if (current_distance > 0.2)
        id_ = GIZMO_UNKNOWN;
    
    for (unsigned int i = 0; i < 3; ++i)
    {
        if (GizmoMovePlanes[i].RayPick(SDKUI::UI().GetmPos(), SDKUI::UI().GetmDir()))
            id_ = GizmoMovePlanes[i].id;
    }

    return id_;
}
