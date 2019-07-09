#include "StdAfx.h"
#include "xrEngine/LightAnimLibrary.h"
#include "SDK_ObjectLight.h"

namespace Cordis
{
	namespace SDK
	{
		constexpr unsigned int LIGHT_VERSION = 0x0011;
		enum
		{
			LIGHT_CHUNK_VERSION = 0xB411,
			LIGHT_CHUNK_FLAG = 0xB413,
			LIGHT_CHUNK_BRIGHTNESS = 0xB425,
			LIGHT_CHUNK_D3D_PARAMS = 0xB435,
			LIGHT_CHUNK_USE_IN_D3D = 0xB436,
			LIGHT_CHUNK_ROTATE = 0xB437,
			LIGHT_CHUNK_ANIMREF = 0xB438,
			LIGHT_CHUNK_SPOT_TEXTURE = 0xB439,
			LIGHT_CHUNK_FUZZY_DATA = 0xB440,
			LIGHT_CHUNK_LCONTROL = 0xB441,
			LIGHT_CHUNK_PARAMS = 0xB442,
		};

		constexpr float VIS_RADIUS = 0.25f;
		constexpr unsigned int SEL_COLOR = 0x00FFFFFF;
		constexpr unsigned int NORM_COLOR = 0x00FFFF00;
		constexpr unsigned int NORM_DYN_COLOR = 0x0000FF00;
		constexpr unsigned int LOCK_COLOR = 0x00FF0000;



		SDK_ObjectLight::SDK_ObjectLight(LPCSTR name) : inherited(name), m_is_render_to_scene(false), m_fuzzydata(nullptr), 
			m_lighttype(kPoint), m_color({1.f,1.f,1.f,0.0f}), m_brightness(1.f), m_attenuation0(1.f), m_attenuation1(1.f), m_attenuation2(1.f),
			m_range(8.f), m_cone(PI_DIV_8), m_virtualsize(0.0f), m_animref(nullptr), m_lightcontrol(0)
		{
			this->m_id_objecttype = kSection_Lights;
			this->m_flags.assign(LightFlags::kAffectStatic);
		}

		SDK_ObjectLight::~SDK_ObjectLight(void)
		{
			if (this->m_fuzzydata)
				xr_delete(this->m_fuzzydata);
		}

		void SDK_ObjectLight::Render(const int &, const bool &)
		{
			RCache.set_Shader(RImplementation.m_WireShader);
			RCache.set_xform_world(Fidentity);
			unsigned int color = SEL_COLOR;
			switch (this->m_lighttype)
			{
			case LightType::kPoint:
			{
				if (this->IsSelected())
				{
					DUImpl.DrawLineSphere(this->getPosition(), this->m_range, SEL_COLOR, true);
				}

				DUImpl.DrawPointLight(this->getPosition(), VIS_RADIUS, NORM_COLOR);
				if (this->m_flags.is(LightFlags::kPointFuzzy))
				{
					if (!this->m_fuzzydata)
					{
						SDKUI_Log::Widget().SetColor(warning);
						SDKUI_Log::Widget().AddText("Something is wrong with m_fuzzydata go to -> Lord!");
					}

					Fvector temporary;
					for (const Fvector& it : this->m_fuzzydata->m_positions)
					{
						this->getTransform().transform_tiny(temporary, it);
						DUImpl.DrawPointLight(temporary, VIS_RADIUS / 6.0f, color);
					}
				}

				break;
			}
			case LightType::kSpot:
			{
				break;
			}
			}
		}

		bool SDK_ObjectLight::RayPick(float & distance, const Fvector & position, const Fvector & direction)
		{
			Fvector ray;
			ray.sub(this->getPosition(),  position);
			float result = ray.dotproduct(direction);

			if (result > 0)
			{
				float result2 = ray.magnitude();
				if (((result2 * result2 - result * result) < (VIS_RADIUS * VIS_RADIUS)) && (result > VIS_RADIUS))
				{
					if (result < distance)
					{
						distance = result;
						return true;
					}
				}
			}

			return false;
		}

		bool SDK_ObjectLight::FrustumPick(const CFrustum & frustum)
		{
			Fvector value = this->getPosition();
			if (frustum.testSphere_dirty(value, VIS_RADIUS))
				return true;

			return false;
		}
	}
}

