#pragma once

#include "Layers/xrRender/ETextureParams.h"
//#include "PropertiesListTypes.h"
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
    virtual ~ECustomThumbnail(void);

    LPCSTR Name(void) { return m_Name.c_str(); }
    LPCSTR SrcName(void) { return m_SrcName.c_str(); }
    inline bool IsClass(THMType type) { return m_Type == type; }
    // thumbnail public routines
    virtual bool Load(LPCSTR src_name = 0, LPCSTR path = 0) = 0;
    virtual void Save(int age = 0, LPCSTR path = 0) = 0;
    virtual bool Valid(void) = 0;
    //	virtual void	FillProp		(PropItemVec& values)=0; // Lord: Interpret
//    virtual void FillInfo(PropItemVec& values) = 0;
};

//------------------------------------------------------------------------------

class EImageThumbnail : public ECustomThumbnail
{
    friend class CImageManager;

protected:
    U32Vec m_Pixels;

protected:
    void CreatePixels(u32* p, u32 w, u32 h);
    void VFlip(void);
    void ConvertRawToTexture(void);

public:
    EImageThumbnail(LPCSTR src_name, THMType type) : ECustomThumbnail(src_name, type){};
    virtual ~EImageThumbnail(void);

    //   virtual void Draw(HDC hdc, const Irect& r);
    //  virtual void Draw(TMxPanel* panel)
    //   {
    //       Irect r;
    //        r.set(1, 1, 1 + panel->Width, 1 + panel->Height);
    //        Draw(panel->Canvas->Handle, r);
    //    } // Lord: [Interpret]

    u32* Pixels(void) { return &*m_Pixels.begin(); }
    virtual int MemoryUsage(void) { return 0; };
    ID3D11ShaderResourceView* GetData(void) const { return this->image_data; }

private:
    ID3D11ShaderResourceView* image_data;
};

class ETextureThumbnail : public EImageThumbnail
{
    friend class CImageManager;
    typedef EImageThumbnail inherited;

private:
    STextureParams m_TexParams;
    bool m_bValid;

public:
    ETextureThumbnail(LPCSTR src_name, bool bLoad = true);
    virtual ~ETextureThumbnail(void);

    // Texture routines
    void CreateFromData(u32* p, u32 w, u32 h);
    inline u32 _Width(void) { return m_TexParams.width; }
    inline u32 _Height(void) { return m_TexParams.height; }
    inline u32 _Alpha(void) { return m_TexParams.HasAlphaChannel(); }
    // thumbnail public routines
    inline STextureParams& _Format() { return m_TexParams; }
    virtual bool Load(LPCSTR src_name = 0, LPCSTR path = 0);
    virtual void Save(int age = 0, LPCSTR path = 0);
    virtual bool Valid(void) { return m_bValid; /*return !m_Pixels.empty();*/ }
    void SetValid(void) { m_bValid = true; }
  //  virtual void FillProp(PropItemVec& values, PropValue::TOnChange on_type_change); // Lord: [InterPret]
 //   virtual void FillInfo(PropItemVec& values);

    //   virtual void Draw(HDC hdc, const Irect& r);
    ///   virtual void Draw(TMxPanel* panel) { inherited::Draw(panel); }
    virtual int MemoryUsage(void);
    LPCSTR FormatString(void);
    BOOL similar(ETextureThumbnail* thm1, xr_vector<xr_string>& sel_params);
};

//------------------------------------------------------------------------------

class EObjectThumbnail : public EImageThumbnail
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
    inline int _VertexCount(void) { return vertex_count; }
    inline int _FaceCount(void) { return face_count; }
    // thumbnail public routines
    virtual bool Load(LPCSTR src_name = 0, LPCSTR path = 0);
    virtual void Save(int age = 0, LPCSTR path = 0);
    virtual bool Valid(void) { return !m_Pixels.empty(); }
  //  virtual void FillProp(PropItemVec& values);
  //  virtual void FillInfo(PropItemVec& values);
};

//------------------------------------------------------------------------------

class EGroupThumbnail : public EImageThumbnail
{
    friend class CImageManager;
    typedef EImageThumbnail inherited;

private:
    SStringVec objects;

public:
    EGroupThumbnail(LPCSTR src_name, bool bLoad = true);
    virtual ~EGroupThumbnail(void);

    // Object routines
    void CreateFromData(u32* p, u32 w, u32 h, const SStringVec& lst);

    // thumbnail public routines
    virtual bool Load(LPCSTR src_name = 0, LPCSTR path = 0);
    virtual void Save(int age = 0, LPCSTR path = 0);
    virtual bool Valid() { return !m_Pixels.empty(); }
  //  virtual void FillProp(PropItemVec& values);
 //   virtual void FillInfo(PropItemVec& values);
};

//------------------------------------------------------------------------------

class ESoundThumbnail : public ECustomThumbnail
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
    virtual bool Load(LPCSTR src_name = nullptr, LPCSTR path = nullptr);
    virtual void Save(int age = 0, LPCSTR path = nullptr);
    virtual bool Valid(void) { return true; }
  //  virtual void FillProp(PropItemVec& values);
  //  virtual void FillInfo(PropItemVec& values);
    float MinDist(void) { return m_fMinDist; }
    float MaxDist(void) { return m_fMaxDist; };
    float BaseVolume(void) { return m_fBaseVolume; };
    void SetMinDist(float d) { m_fMinDist = d; }
    void SetMaxDist(float d) { m_fMaxDist = d; };
};
//------------------------------------------------------------------------------

EImageThumbnail* CreateThumbnail(LPCSTR src_name, ECustomThumbnail::THMType type, bool bLoad = true);
