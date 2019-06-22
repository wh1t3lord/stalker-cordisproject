#include "stdafx.h"
#include "SDK_ObjectLight.h"

namespace Cordis
{
	namespace SDK
	{
		SDK_ObjectLight::SDK_ObjectLight(LPCSTR name) : inherited(name)
		{
			this->m_id_objecttype = kSection_Lights;
		}

		SDK_ObjectLight::~SDK_ObjectLight(void)
		{
		}

		void SDK_ObjectLight::Render(const int &, const bool &)
		{







		}

		bool SDK_ObjectLight::RayPick(float & distance, const Fvector & position, const Fvector & direction)
		{



			return false;
		}

		bool SDK_ObjectLight::FrustumPick(const CFrustum & frustum)
		{


			return false;
		}
	}
}

