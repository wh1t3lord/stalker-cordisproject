#pragma once

class CLAItem;

namespace Cordis
{
	namespace SDK
	{
		constexpr const char* LCONTROL_HEMI = "$hemi";
		constexpr const char* LCONTROL_SUN = "$sun";
		constexpr const char* LCONTROL_STATIC = "$static";

		enum LightFlags
		{
			kAffectStatic = (1 << 0),
			kAffectDynamic = (1 << 1),
			kProcedural = (1 << 2),
			kBreaking = (1 << 3),
			kPointFuzzy = (1 << 4),
			kCastShadow = (1 << 5),
		};

		enum LightType
		{
			kPoint = 1,
			kSpot,
			kDirect,
			kMaxCount,
			kMaxType = -1
		};


		struct FuzzyData
		{
			enum ShapeType
			{
				kSphere,
				kBox,
				kForce = u8(-1)
			};

			FuzzyData(void) : m_shapetype(kSphere), m_sphereradius(0.1f), m_boxdimension({0.1f, 0.1f, 0.1f}), m_pointcount(1)
			{
				this->m_positions.resize(this->m_pointcount, Fvector().set(0, 0, 0));
			}

			~FuzzyData(void) = default;

			void Generate(Fvector& point)
			{
				switch (this->m_shapetype)
				{
				case kSphere:
				{
					point.random_point(this->m_sphereradius);
					break;
				}
				case kBox:
				{
					point.random_point(this->m_boxdimension);
					break;
				}
				}


				void SaveStream(IWriter& writer);
				void LoadStream(IReader& reader);
				void SaveLTX(CInifile& ini, LPCSTR section_name);
				void LoadLTX(CInifile& ini, LPCSTR section_name);
			}

			std::int16_t m_pointcount;
			ShapeType m_shapetype;
			float m_sphereradius;
			Fvector m_boxdimension;
			xr_vector<Fvector> m_positions;
		};

		class SDK_ObjectLight : public SDK_CustomObject
		{
			using inherited = SDK_CustomObject;
		public:
			SDK_ObjectLight(void) = delete;
			SDK_ObjectLight(LPCSTR name);
			~SDK_ObjectLight(void);

			void Render(const int&, const bool&);
			bool RayPick(float& distance, const Fvector& position, const Fvector& direction);
			bool FrustumPick(const CFrustum& frustum);
			void Scale(const Fvector& v) {}
			void RotateLocal(const Fvector& axis, const float& angle) {}
			void RotateParent(const Fvector& axis, const float& angle) {}
			inline const Fbox& getBox(void) noexcept
			{
				this->m_box.set(this->getPosition(), this->getPosition());
				this->m_box.vMin.sub(this->m_range);
				this->m_box.vMax.add(this->m_range);

				return this->m_box;
			}

		private:
			bool m_is_render_to_scene;
			LightType m_lighttype;
			Flags32 m_flags;
			unsigned int m_lightcontrol;
			float m_brightness;
			float m_range;
			float m_attenuation0;
			float m_attenuation1;
			float m_attenuation2;
			float m_cone;
			float m_virtualsize;
			Fcolor m_color;
			xr_string m_fallofftex;
			FuzzyData* m_fuzzydata;
			CLAItem* m_animref;
		};

	}
}

