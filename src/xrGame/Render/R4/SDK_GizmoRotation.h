#pragma once

namespace Cordis
{
	namespace SDK
	{

		enum AxisRotationType
		{
			GIZMO_ROTATION_X,
			GIZMO_ROTATION_Y,
			GIZMO_ROTATION_Z,
			GIZMO_ROTATION_SPHERE,
			GIZMO_ROTATION_CIRCLE,
			GIZMO_ROTATION_UNKNOWN = -1
		};

		class SDK_GizmoRotation 
		{
		public:
			SDK_GizmoRotation(void) = default;
			SDK_GizmoRotation(AxisRotationType _id) : m_axis_id(_id) {}
			~SDK_GizmoRotation(void);

			bool getPoint(const Fvector& position, const Fvector& direction, Fvector& point);
			void RayPick(const Fvector& position, const Fvector& direction);
			// Lord: реализовать данный метод, подумать либо заполнять либо просто передавать как ссылку
			void InitializePoints(const Fvector* cache);
		private:
			AxisRotationType m_axis_id;
			unsigned int m_color;
			Fvector* m_points;
		};

 
 
	}
}

