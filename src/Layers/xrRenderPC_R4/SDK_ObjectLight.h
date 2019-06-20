#pragma once
namespace Cordis
{
	namespace SDK
	{
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
		};

	}
}

