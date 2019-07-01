#pragma once

#include "SDK_GizmoManager.h"

 

namespace Cordis
{
    namespace SDK
    {
    class SDK_SceneManager // Lord: реализовать отображение context window (когда background засветляется)
    {
        friend class SDK_ObjectShape;

    private:
        SDK_SceneManager(void)
            : m_total_count_objects(0), /*m_total_count_objects_glows(0),*/ m_total_count_objects_groups(0),
              m_total_count_objects_sound_environments(0), m_total_count_objects_sound_sources(0),
              m_total_count_objects_spawn_elements(0), m_total_count_objects_static_particles(0),
              m_total_count_objects_wallmarks(0), m_total_count_objects_waypoints(0), m_total_count_objects_lights(0),
              m_total_count_objects_ai_nodes(0), m_total_current_selected_objects(0), m_total_deleted_objects(0),
              m_total_count_objects_static_geometry(0), m_total_count_objects_shapes(0), current_distance_to_object(0),
              m_currentobject(nullptr)
        {
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


		inline const xr_list<SDK_CustomObject*>& getObjectsList(void) noexcept { return this->m_objects_list; }
		inline const xr_list<SDK_CustomObject*>& getObjectsListSelected(void) noexcept { return this->m_selectedobjects_list; }

		void AddObject(const Fvector&, const Fvector&);
        void ZoomExtentSelected(const Fbox& bbox);
        void DeleteResources(void);
        //  void PickObject(const Fvector& start, const Fvector& direction);
        SDK_CustomObject* Selection(const Fvector& start, const Fvector& direction, bool bSingleSelection = false);
        void Selection(const CFrustum& frustum, bool single_selection = false);
        void DeSelection(const Fvector& start, const Fvector& direction);
        void DeSelection(const CFrustum& frustum);
        void UnSelectAll(void);

        AxisType SelectionAxisMove(void);
        AxisScaleType SelectionAxisScale(void);
        void Move(const Fvector& value);
        void Rotate(const Fvector& value);
        void Scale(const Fvector& value);

        inline const Fvector& getPositionFromLastSelectedObject(void)
        {
            if (SDKUI::UI().IsUsedMoveTool() || SDKUI::UI().IsUsedRotateTool() || SDKUI::UI().IsUsedScaleTool())
            {
                if (!this->m_selectedobjects_list.size())
                    return Fvector({0, 0, 0});

                SDK_CustomObject* object = this->m_selectedobjects_list.back();
                return object->getPosition();
            }

            SDKUI_Log::Widget().SetColor(warning);
            SDKUI_Log::Widget().AddText("Warning getting position from unused state!");
            return Fvector({0, 0, 0});
        }

    private:
        SDK_CustomObject* _AddObjectStaticGeometry(LPCSTR name);
        SDK_CustomObject* _AddObjectShape(const xr_string& name);
		SDK_CustomObject* _AddObjectLight(const xr_string& name);
		SDK_CustomObject* _AddObjectSpawnElement(const xr_string& name);
        void _AddObjectGeometryOfShape(ShapeType type);
        

    public:
#pragma region Statistic
        unsigned int m_total_count_objects;
        unsigned int m_total_count_objects_static_geometry;
        unsigned int m_total_count_objects_waypoints;
        unsigned int m_total_count_objects_lights;
  //      unsigned int m_total_count_objects_glows;
        unsigned int m_total_count_objects_spawn_elements;
        unsigned int m_total_count_objects_ai_nodes; // @ Lord: Interpret this to recastAnddetour
        unsigned int m_total_count_objects_wallmarks;
        unsigned int m_total_count_objects_sound_sources;
        unsigned int m_total_count_objects_sound_environments;
        unsigned int m_total_count_objects_static_particles;
        unsigned int m_total_count_objects_shapes;
        unsigned int m_total_count_objects_groups;
        unsigned int m_total_current_selected_objects;
        unsigned int m_total_deleted_objects;
#pragma endregion

    private:
        float current_distance_to_object;
        SDK_CustomObject* m_currentobject;
        xr_list<SDK_CustomObject*> m_selectedobjects_list;
        xr_list<SDK_CustomObject*> m_objects_list; // All kind of object in the scene

    };

    }
}
