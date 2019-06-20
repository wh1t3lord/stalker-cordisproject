#include "stdafx.h"
#include "SDK_ObjectWayPoint.h"

 
namespace Cordis
{
    namespace SDK
    {

	SDK_ObjectWayPoint::SDK_ObjectWayPoint(LPCSTR name) : inherited(name)
	{
		this->m_id_objecttype = kSection_WayPoints;
		
	}

	SDK_ObjectWayPoint::~SDK_ObjectWayPoint(void)
	{
	}

	void SDK_ObjectWayPoint::Render(const int &, const bool &)
	{
	}

	bool SDK_ObjectWayPoint::RayPick(float & distance, const Fvector & position, const Fvector & direction)
	{
		return false;
	}

	bool SDK_ObjectWayPoint::FrustumPick(const CFrustum & frustum)
	{
		return false;
	}

    }
}
