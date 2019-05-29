#pragma once

#include "SDK_Cache.h"
#include "SDK_GizmoManager.h"

class SDK_SceneManager // Lord: реализовать отображение context window (когда background засветляется)
{
private:
    SDK_SceneManager(void)
        : total_count_objects(0), total_count_objects_glows(0), total_count_objects_groups(0),
          total_count_objects_sound_environments(0), total_count_objects_sound_sources(0),
          total_count_objects_spawn_elements(0), total_count_objects_static_particles(0),
          total_count_objects_wallmarks(0), total_count_objects_waypoints(0), total_count_objects_lights(0),
          total_count_objects_ai_nodes(0), total_current_selected_objects(0), total_deleted_objects(0),
          total_count_objects_static_geometry(0), total_count_objects_shapes(0), current_distance_to_object(0),
          CurrentObject(nullptr)
    {
        SDK_Cache::GetInstance().MakeCache();
    }

public:
    inline static SDK_SceneManager& GetInstance(void) noexcept
    {
        static SDK_SceneManager instance;
        return instance;
    }
    ~SDK_SceneManager(void);
    SDK_SceneManager(const SDK_SceneManager&) = delete;
    SDK_SceneManager& operator=(const SDK_SceneManager&) = delete;
    SDK_SceneManager(SDK_SceneManager&&) = delete;
    SDK_SceneManager& operator=(SDK_SceneManager&&) = delete;

    void AddObject(const Fvector&, const Fvector&);
    void ZoomExtentSelected(const Fbox& bbox);
    void DeleteResources(void);
    //  void PickObject(const Fvector& start, const Fvector& direction);
    SDK_CustomObject* Selection(const Fvector& start, const Fvector& direction, bool bSingleSelection = false);
    void DeSelection(const Fvector& start, const Fvector& direction);
    void UnSelectAll(void);
    AxisType SelectionAxisMove(void);
    void Move(const Fvector& vec);
    void Rotate(const Fvector& vec);
    void Scale(const Fvector& vec);

    inline const Fvector& GetPositionFromLastSelectedObject(void)
    {
        if (SDKUI::UI().IsUsedMoveTool() || SDKUI::UI().IsUsedRotateTool() || SDKUI::UI().IsUsedScaleTool())
        {
            if (!this->SelectedObjectsList.size())
                return Fvector({0, 0, 0});

            SDK_CustomObject* object = this->SelectedObjectsList.back();
            return object->GetPosition();
        }

        SDKUI_Log::Widget().SetColor(warning);
        SDKUI_Log::Widget().AddText("Warning getting position from unused state!");
        return Fvector({0, 0, 0});
    }

private:
    //  SDK_CustomObject* _CreateObject(void); Lord: Implement this
    SDK_CustomObject* _AddObjectStaticGeometry(LPCSTR name);
    void _AddObjectLight(void);

public:
#pragma region Statistic
    unsigned int total_count_objects;
    unsigned int total_count_objects_static_geometry;
    unsigned int total_count_objects_waypoints;
    unsigned int total_count_objects_lights;
    unsigned int total_count_objects_glows;
    unsigned int total_count_objects_spawn_elements;
    unsigned int total_count_objects_ai_nodes; // @ Lord: Interpret this to recastAnddetour
    unsigned int total_count_objects_wallmarks;
    unsigned int total_count_objects_sound_sources;
    unsigned int total_count_objects_sound_environments;
    unsigned int total_count_objects_static_particles;
    unsigned int total_count_objects_shapes;
    unsigned int total_count_objects_groups;
    unsigned int total_current_selected_objects;
    unsigned int total_deleted_objects;
#pragma endregion

private:
    float current_distance_to_object;

public:
    SDK_CustomObject* CurrentObject;
    xr_list<SDK_CustomObject*> SelectedObjectsList;
    xr_list<SDK_CustomObject*> ObjectList; // All kind of object in the scene
};
