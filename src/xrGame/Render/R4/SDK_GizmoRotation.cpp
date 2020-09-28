#include "StdAfx.h"
#include "SDK_GizmoRotation.h"

namespace Cordis
{
	namespace SDK
	{
		SDK_GizmoRotation::~SDK_GizmoRotation(void)
		{
			
		}


		bool SDK_GizmoRotation::getPoint(const Fvector& position, const Fvector& direction, Fvector& point)
		{
			if (this->m_axis_id != GIZMO_ROTATION_SPHERE)
			{

			}

			return false;
		}

		void SDK_GizmoRotation::RayPick(const Fvector& position, const Fvector& direction)
		{
			if (this->m_axis_id != GIZMO_ROTATION_SPHERE)
			{

			}
			

		}

		void SDK_GizmoRotation::InitializePoints(const Fvector* cache)
		{
			if (!cache)
				return;

		}
	}
}
