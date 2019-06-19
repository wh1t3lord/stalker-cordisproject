#pragma once
namespace Cordis
{
    namespace SDK
    {
    class SDK_ObjectWayPoint : public SDK_CustomObject
    {
		using inherited = SDK_CustomObject;
    public:
        SDK_ObjectWayPoint(void) = delete;
		SDK_ObjectWayPoint(LPCSTR name);
        ~SDK_ObjectWayPoint(void);

		void Render(const int&, const bool&);
		bool RayPick(float& distance, const Fvector& position, const Fvector& direction);
		bool FrustumPick(const CFrustum& frustum);
    };
    }
}

