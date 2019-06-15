#pragma once
#include "SDK_CustomObject.h"

namespace Cordis
{
namespace SDK
{
struct ShapesColors
{
    static constexpr unsigned int kTransparentColor = 0x3C808080;
    static constexpr unsigned int kEdgeColor = 0xFF202020;
};

enum ShapeType
{
    kShapeType_Sphere,
    kShapeType_Box,
    kShapeType_NonSelected
};

class SDK_ObjectShape : public SDK_CustomObject
{
    friend class SDK_SceneManager;
    using inherited = SDK_CustomObject;

public:
    SDK_ObjectShape(void) = delete;
    SDK_ObjectShape(const xr_string& name);
    ~SDK_ObjectShape(void);

    inline unsigned char GetShapeType(void) noexcept { return this->m_shape_type; }
    inline void SetGeometryType(const ShapeType& value) noexcept { this->m_shape_geometry_type = value; }

    void Attach(SDK_ObjectShape*);
    void Detach(void);

    void Render(const int&, const bool&);
    bool RayPick(float& distance, const Fvector& start, const Fvector& direction);
    bool FrustumPick(const CFrustum& frustum);
    void Scale(const Fvector& value);

private:
    void ComputeBounds(void);

private:
    unsigned char m_shape_type;
    unsigned char m_shape_geometry_type;
    Fsphere m_sphere_data;
    Fmatrix m_box_data;
    //  Fsphere m_sphere;
};
} // namespace SDK
} // namespace Cordis
