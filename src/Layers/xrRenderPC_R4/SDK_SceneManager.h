#pragma once

#include "SDK_Cache.h"
#include "SDK_GizmoManager.h"

class SDK_SceneManager // Lord: реализовать отображение context window (когда background засветляется)
{
private:
    SDK_SceneManager(void)
    {
        SDK_Cache::GetInstance().MakeCache();

#pragma region Statistics
        this->total_count_objects = 0;
        this->total_count_objects_glows = 0;
        this->total_count_objects_ai_nodes = 0;
        this->total_count_objects_lights = 0;
        this->total_count_objects_shapes = 0;
        this->total_count_objects_sound_sources = 0;
        this->total_count_objects_spawn_elements = 0;
        this->total_count_objects_wallmarks = 0;
        this->total_count_objects_waypoints = 0;
        this->total_count_objects_static_geometry = 0;
        this->total_count_objects_static_particles = 0;
        this->total_count_objects_groups = 0;
        this->total_current_selected_objects = 0;
        this->total_deleted_objects = 0;
#pragma endregion

        this->current_distance_to_object = 0;
        this->CurrentObject = nullptr;
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
    void DeleteResources(void);
    //  void PickObject(const Fvector& start, const Fvector& direction);
    SDK_CustomObject* SingleSelection(const Fvector& start, const Fvector& direction);
    AxisType SelectionAxisMove(void);

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
    xr_list<SDK_CustomObject*> ObjectList; // All kind of object in the scene
};
