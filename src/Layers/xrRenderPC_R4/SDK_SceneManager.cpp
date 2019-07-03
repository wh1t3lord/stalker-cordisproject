#include "stdafx.h"

#include "SDK_SceneManager.h"
#include "SDKUI_RightWindow.h"
#include "SDKUI_Helpers.h"
#include "xrEngine/SDK_Camera.h"
#include "SDK_ObjectShape.h"
#include "SDK_ObjectLight.h"
#include "SDK_ObjectSpawnElement.h"

namespace Cordis
{
namespace SDK
{
constexpr float kGizmoDetectionMove = 1.5;
constexpr float kGizmoDetectionScale = 0.25f;
SDK_SceneManager::~SDK_SceneManager(void) {}

void SDK_SceneManager::AddObject(const Fvector& position, const Fvector& normal)
{
    /* Памятка
        kSection_StaticGeometry,
        kSection_Lights,
        kSection_SoundSource,
        kSection_SoundEnvironment,
        kSection_Glows,
        kSection_Shapes,
        kSection_SpawnElements,
        kSection_WayPoints,
        kSection_Sectors,
        kSection_Portals,
        kSection_Groups,
        kSection_StaticParticles,
        kSection_DetailObjects,
        kSection_AIMap,
        kSection_Wallmarks,
        kSection_FogVolumes
    */
    // @ Типы секций...
    switch (SDKUI_RightWindow::Widget().GetSection())
    {
    case kSection_StaticGeometry:
    {
        if (!SDKUI_RightWindow::Widget().GetCurrentSelectedStaticObject().empty())
        {
            SDK_CustomObject* object = this->_AddObjectStaticGeometry(this->GenerateName(SDKUI_RightWindow::Widget().GetCurrentSelectedStaticObject()));
            object->MoveTo(position, normal);
            this->m_objects_list.push_back(object);
        }
        else
        {
            // Lord: сюда modal window
        }
        break;
    }
    case kSection_Lights:
    {
        // Lord: нужно реализовать генерацию имени!
        SDK_CustomObject* object = this->_AddObjectLight(this->GenerateName("light"));
        object->MoveTo(position, normal);
        this->m_objects_list.push_back(object);
        break;
    }
    case kSection_SoundSource: { break;
    }
    case kSection_SoundEnvironment: { break;
    }
    case kSection_Shapes:
    {
        SDK_CustomObject* object = this->_AddObjectShape(this->GenerateName("shape"));
        object->MoveTo(position, normal);
        this->m_objects_list.push_back(object);
        break;
    }
    case kSection_SpawnElements:
    {
        if (!SDKUI_RightWindow::Widget().getCurrentSelectedSpawnElement().size())
        {
            SDKUI_Log::Widget().SetColor(warning);
            SDKUI_Log::Widget().AddText("Can't add object, because nothing selected");
            break;
        }

        // Lord: реализовать генерацию имени!
        SDK_CustomObject* object = this->_AddObjectSpawnElement(this->GenerateName(SDKUI_RightWindow::Widget().getCurrentSelectedSpawnElement()));
        object->MoveTo(position, normal);
        this->m_objects_list.push_back(object);
        break;
    }
    case kSection_WayPoints: { break;
    }
    case kSection_Sectors: { break;
    }
    case kSection_Portals: { break;
    }
    case kSection_Groups: { break;
    }
    case kSection_StaticParticles: { break;
    }
    case kSection_DetailObjects: { break;
    }
    case kSection_AIMap: { break;
    }
    case kSection_Wallmarks: { break;
    }
    case kSection_FogVolumes: { break;
    }
        // @
    }
}

void SDK_SceneManager::ZoomExtentSelected(const Fbox& bbox) {}

#pragma region AddingObjects
SDK_CustomObject* SDK_SceneManager::_AddObjectStaticGeometry(const xr_string& name)
{
    if (!name.size())
        return nullptr;

    // Lord: сюда добавить генерацию имени
    SDK_ObjectStaticGeometry* object = new SDK_ObjectStaticGeometry(name.c_str());
    CEditableObject* model = SDK_Cache::GetInstance().GetGeometry(SDKUI_RightWindow::Widget().GetCurrentSelectedStaticObject().c_str());
    if (object == nullptr)
    {
        SDKUI_Log::Widget().SetColor(error);
        SDKUI_Log::Widget().AddText("Geometry was null!");
    }

    object->SetGeometry(model);
    object->setReferenceName(name);
    return object;
}

SDK_CustomObject* SDK_SceneManager::_AddObjectShape(const xr_string& name)
{
    if (!name.size())
        return nullptr;

    SDK_ObjectShape* object = new SDK_ObjectShape(name.c_str());
    if (!object)
    {
        R_ASSERT2(false, "Something goes wrong!");
    }
    object->SetGeometryType(SDKUI_RightWindow::Widget().GetCurrentShapeType());
    object->m_box_data = Fidentity;
    Fsphere sphere;
    sphere.identity();
    object->m_sphere_data = sphere;
    return object;
}

SDK_CustomObject* SDK_SceneManager::_AddObjectLight(const xr_string& name)
{
    if (!name.size())
        return nullptr;

    SDK_ObjectLight* object = new SDK_ObjectLight(name.c_str());
    return object;
}

SDK_CustomObject* SDK_SceneManager::_AddObjectSpawnElement(const xr_string& name)
{
    if (!name.size())
        return nullptr;
    SDK_ObjectSpawnElement* object = new SDK_ObjectSpawnElement(name.c_str(), SDKUI_RightWindow::Widget().getCurrentSelectedSpawnElement().c_str());
    return object;
}

void SDK_SceneManager::_AddObjectGeometryOfShape(ShapeType type)
{
    // @ Lord удалить данный метод
}
#pragma endregion

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
    this->m_current_distance_to_object = SDK_Camera::GetInstance().fFar;

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
        if (it->RayPick(this->m_current_distance_to_object, start, direction) && !it->m_is_selected)
        {
            obj = it;
        }
    }

    // @ Lord: подумать над дизайном, нормально ли это, скорее нет
    if (obj)
    {
        if (this->m_selectedobjects_list.size())
        {
            if (this->m_selectedobjects_list.back()->m_id_objecttype != obj->m_id_objecttype)
            {
            }
            else
            {
                obj->m_is_selected = true;
                this->m_selectedobjects_list.push_back(obj);
            }
        }
        else
        {
            if (SDKUI_RightWindow::Widget().GetSection() != obj->m_id_objecttype)
                SDKUI_RightWindow::Widget().ChangeSectionByObject_CLASS(obj->m_id_objecttype);

            obj->m_is_selected = true;
            this->m_selectedobjects_list.push_back(obj);
        }
    }

    return obj;
}

void SDK_SceneManager::Selection(const CFrustum& frustum, bool single_selection)
{
    if (this->m_selectedobjects_list.size() && single_selection)
    {
        for (xr_list<SDK_CustomObject*>::value_type it : this->m_selectedobjects_list)
        {
            it->m_is_selected = false;
        }

        this->m_selectedobjects_list.clear();
    }

    for (xr_list<SDK_CustomObject*>::value_type it : this->m_objects_list)
    {
        if (it->FrustumPick(frustum) && it->m_id_objecttype == SDKUI_RightWindow::Widget().GetSection())
        {
            it->m_is_selected = true;
            this->m_selectedobjects_list.push_back(it);
        }
    }
}

void SDK_SceneManager::DeSelection(const Fvector& start, const Fvector& direction)
{
    this->m_current_distance_to_object = SDK_Camera::GetInstance().fFar;

    SDK_CustomObject* obj = nullptr;
    for (xr_list<SDK_CustomObject*>::value_type it : this->m_objects_list)
    {
        if (it->RayPick(this->m_current_distance_to_object, start, direction))
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

void SDK_SceneManager::DeSelection(const CFrustum& frustum)
{
    if (!this->m_selectedobjects_list.size())
        return;

    for (xr_list<SDK_CustomObject*>::value_type it : this->m_selectedobjects_list)
    {
        if (it->FrustumPick(frustum) && it->m_is_selected)
        {
            it->m_is_selected = false;
            this->m_selectedobjects_list.remove(it);
        }
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
    if (!SDKUI::UI().IsUsedMoveTool())
        return GIZMO_UNKNOWN;

    float current_distance = 10000000;
    AxisType id_ = GIZMO_UNKNOWN;
    for (unsigned int i = 0; i < 3; i++)
    {
        // @ Have intersection
        float distance = GizmoMove[i].RayPick(SDKUI::UI().getMousePosition(), SDKUI::UI().getMouseDirection());
        if (current_distance > distance)
        {
            current_distance = distance;
            id_ = GizmoMove[i].m_axis_id;
        }
        //   SDKUI_Log::Widget().AddText("%d %s",
        //   i,std::to_string(GizmoMove[i].RayPick(SDK_Camera::GetInstance().fFar, SDKUI::UI().GetmPos(),
        //   SDKUI::UI().GetmDir())).c_str());
    }
    //  SDKUI_Log::Widget().AddText("%f", current_distance);
    // SDKUI_Log::Widget().AddText("%d", id_);

    if (current_distance > kGizmoDetectionMove)
        id_ = GIZMO_UNKNOWN;

    for (unsigned int i = 0; i < 3; ++i)
    {
        if (GizmoMovePlanes[i].RayPick(SDKUI::UI().getMousePosition(), SDKUI::UI().getMouseDirection()))
            id_ = GizmoMovePlanes[i].m_axis_id;
    }
    //   this->gizmo_distance = current_distance;
    return id_;
}

AxisScaleType SDK_SceneManager::SelectionAxisScale(void)
{
    if (!SDKUI::UI().IsUsedScaleTool())
        return GIZMO_SCALE_UNKNOWN;

    float current_distance = 10000000;
    AxisScaleType _id = GIZMO_SCALE_UNKNOWN;
    float distance = -1.0f;
    for (unsigned int i = 0; i < 3; ++i)
    {
        distance = g_gizmo_lines_scale[i].RayPick(SDKUI::UI().getMousePosition(), SDKUI::UI().getMouseDirection());
        if (current_distance > distance)
        {
            current_distance = distance;
            _id = g_gizmo_lines_scale[i].m_axis_id;
        }
    }

    if (current_distance > kGizmoDetectionScale)
        _id = GIZMO_SCALE_UNKNOWN;

    for (unsigned int i = 0; i < 9; ++i)
    {
        if (g_gizmo_planes_scale[i].RayPick(SDKUI::UI().getMousePosition(), SDKUI::UI().getMouseDirection()))
            _id = g_gizmo_planes_scale[i].m_axis_id;
    }

    return _id;
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
        for (xr_list<SDK_CustomObject*>::iterator it = this->SelectedObjectsList.begin(); it !=
       this->SelectedObjectsList.end(); ++it)
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

xr_string SDK_SceneManager::GenerateName(const xr_string& object_name_from_list)
{
    if (!SDKUI_SceneOptions::Widget().getLevelPrefix())
        return xr_string("generated_level_prefix");

   xr_string generated_name = SDKUI_SceneOptions::Widget().getLevelPrefix();
   generated_name.append("_");
   generated_name.append(object_name_from_list);
   
   // @ Ya know it's a lazy implementation каюсь)
   static int generated_id = 0;
   generated_name.append("_");
   generated_name.append(std::to_string(generated_id).c_str());
   ++generated_id;

   return generated_name;
}

} // namespace SDK
} // namespace Cordis
