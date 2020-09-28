#pragma once
class Blender_2D_UI : public IBlender
{
public:
    Blender_2D_UI(void) = default;
    ~Blender_2D_UI(void);
    virtual LPCSTR getComment(void) { return "LEVEL: ui-shader"; }
    virtual void Compile(CBlender_Compile& C);
    virtual BOOL canBeLMAPped(void) { return FALSE; }
};
