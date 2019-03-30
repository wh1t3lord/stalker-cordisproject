#pragma once

#include "xrEngine/GameMtlLib.h"
#include "ImageManager.h"


struct SSimpleImage;

class CSurface
{
    u32 m_GameMtlID;
    ref_shader m_Shader;
    enum ERTFlags
    {
        rtValidShader = (1 << 0),
    };

public:
    enum EFlags
    {
        sf2Sided = (1 << 0),
    };
    shared_str m_Name;
    shared_str m_Texture; //
    shared_str m_VMap; //
    shared_str m_ShaderName;
    shared_str m_ShaderXRLCName;
    shared_str m_GameMtlName;
    Flags32 m_Flags;
    u32 m_dwFVF;
#ifdef _MAX_EXPORT
    u32 mid;
    Mtl* mtl;
#endif
    Flags32 m_RTFlags;
    u32 tag;
    SSimpleImage* m_ImageData;

public:
    CSurface()
    {
        m_GameMtlName = "default";
        m_ImageData = 0;
        m_Shader = 0;
        m_RTFlags.zero();
        m_Flags.zero();
        m_dwFVF = 0;
#ifdef _MAX_EXPORT
        mtl = 0;
        mid = 0;
#endif
        tag = 0;
    }
    inline bool Validate() { return (0 != xr_strlen(m_Texture)) && (0 != xr_strlen(m_ShaderName)); }

    ~CSurface()
    {
        R_ASSERT(!m_Shader);
        xr_delete(m_ImageData);
    }
    inline void CopyFrom(CSurface* surf)
    {
        *this = *surf;
        m_Shader = 0;
    }
    inline int _Priority() { return _Shader() ? _Shader()->E[0]->flags.iPriority : 1; }
    inline bool _StrictB2F() { return _Shader() ? _Shader()->E[0]->flags.bStrictB2F : false; }
    inline ref_shader _Shader()
    {
        if (!m_RTFlags.is(rtValidShader))
            OnDeviceCreate();
        return m_Shader;
    }

    inline LPCSTR _Name() const { return *m_Name; }
    inline LPCSTR _ShaderName() const { return *m_ShaderName; }
    inline LPCSTR _GameMtlName() const { return *m_GameMtlName; }
    inline LPCSTR _ShaderXRLCName() const { return *m_ShaderXRLCName; }
    inline LPCSTR _Texture() const { return *m_Texture; }
    inline LPCSTR _VMap() const { return *m_VMap; }
    inline u32 _FVF() const { return m_dwFVF; }
    inline void SetName(LPCSTR name) { m_Name = name; }

    inline void SetShader(LPCSTR name)
    {
        R_ASSERT2(name && name[0], "Empty shader name.");
        m_ShaderName = name;
        OnDeviceDestroy(); // Lord: Подумать что здесь должно быть
    }

    inline void SetShaderXRLC(LPCSTR name) { m_ShaderXRLCName = name; }
    inline void SetGameMtl(LPCSTR name) { m_GameMtlName = name; }
    inline void SetFVF(u32 fvf) { m_dwFVF = fvf; }
    inline void SetTexture(LPCSTR name)
    {
        string512 buf;
        xr_strcpy(buf, sizeof(buf), name);
        if (strext(buf))
            *strext(buf) = 0;
        m_Texture = buf;
    }

    inline void SetVMap(LPCSTR name) { m_VMap = name; }


    inline u32 _GameMtl() const { return GMLib.GetMaterialID(*m_GameMtlName); } // Lord: есть ли разница однако использовать PGMLib или GMLib, хех
    inline void OnDeviceCreate()
    {
        R_ASSERT(!m_RTFlags.is(rtValidShader));
        if (m_ShaderName.size() && m_Texture.size())
            m_Shader.create("editor\\wire"/*, *m_Texture*/);
        else
            m_Shader.create("editor\\wire");
        m_RTFlags.set(rtValidShader, TRUE);
    }
    inline void OnDeviceDestroy()
    {
        m_Shader.destroy();
        m_RTFlags.set(rtValidShader, FALSE);
    }
    void CreateImageData();
    void RemoveImageData();

};

using SurfaceVec = xr_vector<CSurface*>;
using SurfaceIt = SurfaceVec::iterator;
