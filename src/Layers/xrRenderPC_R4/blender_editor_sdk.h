#pragma once
class blender_editor_sdk : public IBlender
{
public:
    blender_editor_sdk(void) = delete;
    blender_editor_sdk(bool bSelection)
    { 
        this->bSelection = bSelection;
    }
    ~blender_editor_sdk(void);
    virtual LPCSTR getComment(void) { return "LEVEL: sdk-shader"; }
    virtual void Compile(CBlender_Compile& C);
    virtual BOOL canBeLMAPped(void) { return FALSE; }

private:
    bool bSelection;
};
