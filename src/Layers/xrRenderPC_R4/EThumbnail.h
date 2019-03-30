#pragma once

#include "Layers/xrRender/ETextureParams.h"
#include "PropertiesListTypes.h"
//#include "MXCtrls.hpp"
//------------------------------------------------------------------------------
// Custom class
//------------------------------------------------------------------------------
class ECustomThumbnail
{
public:
    enum THMType
    {
        ETUndef = -1,
        ETObject = 0,
        ETTexture = 1,
        ETSound = 2,
        ETGroup = 3,
        force_dword = (-1)
    };

protected:
    xr_string m_SrcName;
    xr_string m_Name;
    int m_Age;
    THMType m_Type;

public:
    ECustomThumbnail(LPCSTR src_name, THMType type);
    virtual ~ECustomThumbnail();

    LPCSTR Name() { return m_Name.c_str(); }
    LPCSTR SrcName() { return m_SrcName.c_str(); }
    inline bool IsClass(THMType type) { return m_Type == type; }
    // thumbnail public routines
    virtual bool Load(LPCSTR src_name = 0, LPCSTR path = 0) = 0;
    virtual void Save(int age = 0, LPCSTR path = 0) = 0;
    virtual bool Valid() = 0;
    //	virtual void	FillProp		(PropItemVec& values)=0; // Lord: Interpret
    virtual void FillInfo(PropItemVec& values) = 0;
};

//------------------------------------------------------------------------------

class EImageThumbnail :

    public ECustomThumbnail
{
    friend class CImageManager;

protected:
    U32Vec m_Pixels;

protected:
    void CreatePixels(u32* p, u32 w, u32 h);
    void VFlip();

public:
    EImageThumbnail(LPCSTR src_name, THMType type) : ECustomThumbnail(src_name, type){};
    virtual ~EImageThumbnail();

    //   virtual void Draw(HDC hdc, const Irect& r);
    //  virtual void Draw(TMxPanel* panel)
    //   {
    //       Irect r;
    //        r.set(1, 1, 1 + panel->Width, 1 + panel->Height);
    //        Draw(panel->Canvas->Handle, r);
    //    } // Lord: [Interpret]

    u32* Pixels() { return &*m_Pixels.begin(); }
    virtual int MemoryUsage() { return 0; };
};

class ETextureThumbnail :

    public EImageThumbnail
{
    friend class CImageManager;
    typedef EImageThumbnail inherited;

private:
    STextureParams m_TexParams;
    bool m_bValid;

public:
    ETextureThumbnail(LPCSTR src_name, bool bLoad = true);
    virtual ~ETextureThumbnail();

    // Texture routines
    void CreateFromData(u32* p, u32 w, u32 h);
    inline u32 _Width() { return m_TexParams.width; }
    inline u32 _Height() { return m_TexParams.height; }
    inline u32 _Alpha() { return m_TexParams.HasAlphaChannel(); }
    // thumbnail public routines
    inline STextureParams& _Format() { return m_TexParams; }
    virtual bool Load(LPCSTR src_name = 0, LPCSTR path = 0);
    virtual void Save(int age = 0, LPCSTR path = 0);
    virtual bool Valid() { return m_bValid; /*return !m_Pixels.empty();*/ }
    void SetValid() { m_bValid = true; }
    virtual void FillProp(PropItemVec& values, PropValue::TOnChange on_type_change); // Lord: [InterPret]
    virtual void FillInfo(PropItemVec& values);

    //   virtual void Draw(HDC hdc, const Irect& r);
    ///   virtual void Draw(TMxPanel* panel) { inherited::Draw(panel); }
    virtual int MemoryUsage();
    LPCSTR FormatString();
    BOOL similar(ETextureThumbnail* thm1, xr_vector<xr_string>& sel_params);
};

//------------------------------------------------------------------------------

class EObjectThumbnail :

    public EImageThumbnail
{
    friend class CImageManager;
    typedef EImageThumbnail inherited;

private:
    u32 face_count;
    u32 vertex_count;

public:
    EObjectThumbnail(LPCSTR src_name, bool bLoad = true);
    virtual ~EObjectThumbnail();

    // Object routines
    void CreateFromData(u32* p, u32 w, u32 h, int fc, int vc);
    inline int _VertexCount() { return vertex_count; }
    inline int _FaceCount() { return face_count; }
    // thumbnail public routines
    virtual bool Load(LPCSTR src_name = 0, LPCSTR path = 0);
    virtual void Save(int age = 0, LPCSTR path = 0);
    virtual bool Valid() { return !m_Pixels.empty(); }
    virtual void FillProp(PropItemVec& values);
    virtual void FillInfo(PropItemVec& values);
};

//------------------------------------------------------------------------------

class EGroupThumbnail :

    public EImageThumbnail
{
    friend class CImageManager;
    typedef EImageThumbnail inherited;

private:
    SStringVec objects;

public:
    EGroupThumbnail(LPCSTR src_name, bool bLoad = true);
    virtual ~EGroupThumbnail();

    // Object routines
    void CreateFromData(u32* p, u32 w, u32 h, const SStringVec& lst);

    // thumbnail public routines
    virtual bool Load(LPCSTR src_name = 0, LPCSTR path = 0);
    virtual void Save(int age = 0, LPCSTR path = 0);
    virtual bool Valid() { return !m_Pixels.empty(); }
    virtual void FillProp(PropItemVec& values);
    virtual void FillInfo(PropItemVec& values);
};

//------------------------------------------------------------------------------

class ESoundThumbnail :

    public ECustomThumbnail
{
    friend class CSoundManager;
    typedef ECustomThumbnail inherited;

private:
    float m_fQuality;
    float m_fMinDist;
    float m_fMaxDist;
    float m_fMaxAIDist;
    float m_fBaseVolume;
    u32 m_uGameType;
    bool OnMaxAIDistAfterEdit(PropValue* sender, float& edit_val);
    void OnMaxDistChange(PropValue* sender);

public:
    ESoundThumbnail(LPCSTR src_name, bool bLoad = true);
    virtual ~ESoundThumbnail();

    // thumbnail public routines
    virtual bool Load(LPCSTR src_name = 0, LPCSTR path = 0);
    virtual void Save(int age = 0, LPCSTR path = 0);
    virtual bool Valid() { return true; }
    virtual void FillProp(PropItemVec& values);
    virtual void FillInfo(PropItemVec& values);
    float MinDist() { return m_fMinDist; }
    float MaxDist() { return m_fMaxDist; };
    float BaseVolume() { return m_fBaseVolume; };
    void SetMinDist(float d) { m_fMinDist = d; }
    void SetMaxDist(float d) { m_fMaxDist = d; };
};
//------------------------------------------------------------------------------

EImageThumbnail* CreateThumbnail(LPCSTR src_name, ECustomThumbnail::THMType type, bool bLoad = true);
