#pragma once
class blender_sdk : public IBlender
{
public:
    virtual LPCSTR getComment() { return "LEVEL: sdk-shader"; }
    virtual void Compile(CBlender_Compile& C);
    virtual BOOL canBeLMAPped() { return FALSE; }
    blender_sdk() = delete;
    blender_sdk(bool bLeaf, bool bCullBack = true, bool bIsFake = false)
    {
        this->bLeaf = bLeaf;
        this->bCullBack = bCullBack;
        this->bIsFake = bIsFake;
    }
    ~blender_sdk();

private:
    bool bLeaf;
    bool bIsFake;
    bool bCullBack;
};
