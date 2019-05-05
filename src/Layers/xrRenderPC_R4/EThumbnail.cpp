#include "stdafx.h"
#pragma hdrstop

#include "EThumbnail.h"
//#include "editors/xrEProps/FolderLib.h" // Lord: Подумать над реализация FolderLib, ибо это уже ImGui должен делать
#include "xrEngine/xrImage_Resampler.h"
#include "ImageManager.h"
#include "xrServerEntities/ai_sounds.h"

// Это немного странно, но ладно, можно так и оставить
const xr_token anomaly_type_token[] = {{"undefined", int(sg_Undefined)},
    {"Item picking up", int(SOUND_TYPE_ITEM_PICKING_UP)}, {"Item dropping", int(SOUND_TYPE_ITEM_DROPPING)},
    {"Item taking", int(SOUND_TYPE_ITEM_TAKING)}, {"Item hiding", int(SOUND_TYPE_ITEM_HIDING)},
    {"Item using", int(SOUND_TYPE_ITEM_USING)}, {"Weapon shooting", int(SOUND_TYPE_WEAPON_SHOOTING)},
    {"Weapon empty clicking", int(SOUND_TYPE_WEAPON_EMPTY_CLICKING)},
    {"Weapon bullet hit", int(SOUND_TYPE_WEAPON_BULLET_HIT)}, {"Weapon recharging", int(SOUND_TYPE_WEAPON_RECHARGING)},
    {"NPC dying", int(SOUND_TYPE_MONSTER_DYING)}, {"NPC injuring", int(SOUND_TYPE_MONSTER_INJURING)},
    {"NPC step", int(SOUND_TYPE_MONSTER_STEP)}, {"NPC talking", int(SOUND_TYPE_MONSTER_TALKING)},
    {"NPC attacking", int(SOUND_TYPE_MONSTER_ATTACKING)}, {"NPC eating", int(SOUND_TYPE_MONSTER_EATING)},
    {"Anomaly idle", int(SOUND_TYPE_ANOMALY_IDLE)}, {"Object breaking", int(SOUND_TYPE_WORLD_OBJECT_BREAKING)},
    {"Object colliding", int(SOUND_TYPE_WORLD_OBJECT_COLLIDING)},
    {"Object exploding", int(SOUND_TYPE_WORLD_OBJECT_EXPLODING)}, {"World ambient", int(SOUND_TYPE_WORLD_AMBIENT)},
    {0, 0}};

//------------------------------------------------------------------------------
// Custom Thumbnail
//------------------------------------------------------------------------------
ECustomThumbnail::ECustomThumbnail(LPCSTR src_name, THMType type)
{
    m_Type = type;
    m_SrcName = src_name;
    m_Name = EFS.ChangeFileExt(xr_string(src_name), ".thm");
    m_Age = 0;
}

//------------------------------------------------------------------------------

ECustomThumbnail::~ECustomThumbnail(void) {}
/*
void DrawThumbnail(TCanvas* pCanvas, TRect& r, U32Vec& data, bool bDrawWithAlpha, int _w = THUMB_WIDTH, int _h =
THUMB_HEIGHT)
{
    pCanvas->CopyMode		= cmSrcCopy;
    Graphics::TBitmap *pBitmap = new Graphics::TBitmap();

    pBitmap->PixelFormat 	= pf32bit;
    pBitmap->Height		 	= _h;
    pBitmap->Width		 	= _w;

    if (bDrawWithAlpha){
        Fcolor back;
        back.set		(bgr2rgb(pCanvas->Brush->Color));  back.mul_rgb(255.f);
        for (int y = 0; y < pBitmap->Height; y++)
        {
            u32* ptr 		= (u32*)pBitmap->ScanLine[y];
            for (int x = 0; x < pBitmap->Width; x++){
                u32 src 	= data[(_h-1-y)*_w+x];
                float a		= float(color_get_A(src))/255.f;
                float inv_a	= 1.f-a;;
                u32 r		= iFloor(float(color_get_R(src))*a+back.r*inv_a);
                u32 g		= iFloor(float(color_get_G(src))*a+back.g*inv_a);
                u32 b		= iFloor(float(color_get_B(src))*a+back.b*inv_a);
                ptr[x] 		= color_rgba(r,g,b,0);
            }
        }
    }else{
        for (int y = 0; y < pBitmap->Height; y++)
        {
            u32* ptr 		= (u32*)pBitmap->ScanLine[y];
            Memory.mem_copy	(ptr,&data[(_h-1-y)*_w],_w*4);
        }
    }
    pCanvas->StretchDraw(r,pBitmap);
    xr_delete(pBitmap);
}
*/
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Image Thumbnail
//------------------------------------------------------------------------------
EImageThumbnail::~EImageThumbnail(void)
{
    if (m_Pixels.size())
        m_Pixels.clear();

    if (this->image_data)
    {
        this->image_data->Release();
        this->image_data = nullptr;
    }
}

    void EImageThumbnail::VFlip(void)
    {
        R_ASSERT(!m_Pixels.empty());
        u32 line[THUMB_WIDTH];
        u32 sz_ln = sizeof(u32) * THUMB_WIDTH;
        u32 y2 = THUMB_WIDTH - 1;
        for (int y = 0; y < THUMB_HEIGHT / 2; y++, y2--)
        {
            CopyMemory(line, &*m_Pixels.begin() + y2 * THUMB_WIDTH, sz_ln);
            CopyMemory(&*m_Pixels.begin() + y2 * THUMB_WIDTH, &*m_Pixels.begin() + y * THUMB_WIDTH, sz_ln);
            CopyMemory(&*m_Pixels.begin() + y * THUMB_WIDTH, line, sz_ln);
        }
    }

    void EImageThumbnail::CreatePixels(u32 * p, u32 w, u32 h)
    {
        //	imf_filter	imf_box  imf_triangle  imf_bell  imf_b_spline  imf_lanczos3  imf_mitchell
        R_ASSERT(p && (w > 0) && (h > 0));
        m_Pixels.resize(THUMB_SIZE);
        imf_Process(&m_Pixels[0], THUMB_WIDTH, THUMB_HEIGHT, p, w, h, imf_box);
    }
    /* Lord: Interpret
    void EImageThumbnail::Draw(HDC hdc, const Irect& r)
    {
        if (Valid())
            FHelper.DrawThumbnail(hdc, r, Pixels(), THUMB_WIDTH, THUMB_HEIGHT);
    }

    */

    EImageThumbnail* CreateThumbnail(LPCSTR src_name, ECustomThumbnail::THMType type, bool bLoad)
    {
        switch (type)
        {
        case ECustomThumbnail::ETObject: return new EObjectThumbnail(src_name, bLoad);
        case ECustomThumbnail::ETTexture: return new ETextureThumbnail(src_name, bLoad);
        case ECustomThumbnail::ETGroup: return new EGroupThumbnail(src_name, bLoad);
        default: NODEFAULT;
        }
        return 0;
    }

    void EImageThumbnail::ConvertRawToTexture(void)
    {
        D3D11_TEXTURE2D_DESC t_desc = {0};
        t_desc.Width = t_desc.Height = 128;
        t_desc.MipLevels = t_desc.ArraySize = 1;
        t_desc.SampleDesc.Count = 1;
        t_desc.SampleDesc.Quality = 0;
        t_desc.Usage = D3D11_USAGE_DEFAULT;
        t_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        t_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        t_desc.CPUAccessFlags = NULL;
        t_desc.MiscFlags = NULL;

        D3D11_SUBRESOURCE_DATA subres_data = {0};
        subres_data.pSysMem = this->Pixels();
        subres_data.SysMemPitch = 128 * 4;
        subres_data.SysMemSlicePitch = 0;

        ID3D11Texture2D* data = nullptr;
        HRESULT hr = HW.pDevice->CreateTexture2D(&t_desc, &subres_data, &data);
        if (FAILED(hr))
        {
            ASSERT(TEXT("Can't create a texture2d. Abort!"));
        }

        hr = HW.pDevice->CreateShaderResourceView(data, NULL, &this->image_data);
        if (FAILED(hr))
        {
            ASSERT(TEXT("Can't convert image_data!"));
        }

        if (data)
        {
            data->Release();
            data = nullptr;
        }
    }

#pragma region EThumbnailTexture

//------------------------------------------------------------------------------
#define THM_TEXTURE_VERSION 0x0012

    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    ETextureThumbnail::ETextureThumbnail(LPCSTR src_name, bool bLoad) : EImageThumbnail(src_name, ETTexture)
    {
        if (!strchr(src_name, '\\'))
        {
            xr_string _name = src_name;
            ImageLib.UpdateFileName(_name);
            m_Name = _name.c_str();
            m_Name = EFS.ChangeFileExt(m_Name, ".thm");
        }
        m_bValid = false;
        if (bLoad)
#ifdef XR_EPROPS_EXPORTS
            Load();
#else
            if (!Load())
            {
                ImageLib.CreateTextureThumbnail(this, src_name);
            }
#endif
    }

    //------------------------------------------------------------------------------

    ETextureThumbnail::~ETextureThumbnail() { m_Pixels.clear(); }
    //------------------------------------------------------------------------------

    int ETextureThumbnail::MemoryUsage()
    {
        int mem_usage = _Width() * _Height() * 4;
        switch (m_TexParams.fmt)
        {
        case STextureParams::tfDXT1:
        case STextureParams::tfADXT1: mem_usage /= 6; break;
        case STextureParams::tfDXT3:
        case STextureParams::tfDXT5: mem_usage /= 4; break;
        case STextureParams::tf4444:
        case STextureParams::tf1555:
        case STextureParams::tf565: mem_usage /= 2; break;
        case STextureParams::tfRGBA: break;
        }
        string_path fn;
        FS.update_path(fn, _game_textures_, EFS.ChangeFileExt(m_Name.c_str(), ".seq").c_str());
        if (FS.exist(fn))
        {
            string128 buffer;
            IReader* F = FS.r_open(0, fn);
            F->r_string(buffer, sizeof(buffer));
            int cnt = 0;
            while (!F->eof())
            {
                F->r_string(buffer, sizeof(buffer));
                cnt++;
            }
            FS.r_close(F);
            mem_usage *= cnt ? cnt : 1;
        }
        return mem_usage;
    }

    //------------------------------------------------------------------------------

    void ETextureThumbnail::CreateFromData(u32 * p, u32 w, u32 h)
    {
        EImageThumbnail::CreatePixels(p, w, h);
        m_TexParams.width = w;
        m_TexParams.height = h;
        m_TexParams.flags.set(STextureParams::flHasAlpha, FALSE);
    }

    //------------------------------------------------------------------------------

    bool Surface_Load(LPCSTR full_name, U32Vec & data, u32 & w, u32 & h, u32 & a);

    bool ETextureThumbnail::Load(LPCSTR src_name, LPCSTR path)
    {
        string_path fn;
        strcpy(fn, EFS.ChangeFileExt(src_name ? src_name : m_Name.c_str(), ".thm").c_str());
        if (path)
            FS.update_path(fn, path, fn);
        else
        {
            FS.update_path(fn, _game_textures_, fn);
        }

        if (!FS.exist(fn))
            return false;

        IReader* F = FS.r_open(fn);
        u16 version = 0;

        R_ASSERT(F->r_chunk(THM_CHUNK_VERSION, &version));
        if (version != THM_TEXTURE_VERSION)
        {
            Msg("!Thumbnail: Unsupported version.");
            return false;
        }

        /*
            IReader* D 		= F->open_chunk(THM_CHUNK_DATA); R_ASSERT(D);
            m_Pixels.resize	(THUMB_SIZE);
            D->r		 (m_Pixels.begin(),THUMB_SIZE*sizeof(u32));
            D->close		();
        */

        R_ASSERT(F->find_chunk(THM_CHUNK_TYPE));
        m_Type = THMType(F->r_u32());
        R_ASSERT(m_Type == ETTexture);

        m_TexParams.Load(*F);
        m_Age = FS.get_file_age(fn);

        FS.r_close(F);
        SetValid();
        return true;
    }

    //------------------------------------------------------------------------------

    void ETextureThumbnail::Save(int age, LPCSTR path)
    {
        if (!Valid())
            return;

        CMemoryWriter F;
        F.open_chunk(THM_CHUNK_VERSION);
        F.w_u16(THM_TEXTURE_VERSION);
        F.close_chunk();

        /*
            F.w_chunk		(THM_CHUNK_DATA | CFS_CompressMark,m_Pixels.begin(),m_Pixels.size()*sizeof(u32));
        */

        F.open_chunk(THM_CHUNK_TYPE);
        F.w_u32(m_Type);
        F.close_chunk();

        m_TexParams.Save(F);

        string_path fn;
        if (path)
            FS.update_path(fn, path, m_Name.c_str());
        else
            FS.update_path(fn, _game_textures_, m_Name.c_str());

        if (F.save_to(fn))
        {
            FS.set_file_age(fn, age ? age : m_Age);
        }
        else
        {
            Log("!Can't save thumbnail:", fn);
        }
    }

    //------------------------------------------------------------------------------

    void ETextureThumbnail::FillProp(PropItemVec & items, PropValue::TOnChange on_type_change)
    {
        // m_TexParams.FillProp(m_SrcName.c_str(), items, on_type_change);
    }

    //------------------------------------------------------------------------------

    void ETextureThumbnail::FillInfo(PropItemVec & items)
    {
        STextureParams& F = m_TexParams;
        //   PHelper().CreateCaption(items, "Format", get_token_name(tfmt_token, F.fmt));
        //  PHelper().CreateCaption(items, "Type", get_token_name(ttype_token, F.type));
        //   PHelper().CreateCaption(items, "Width", shared_str().printf("%d", _Width()));
        //   PHelper().CreateCaption(items, "Height", shared_str().printf("%d", _Height()));
        //    PHelper().CreateCaption(items, "Alpha", _Alpha() ? "on" : "off");
    }

    BOOL ETextureThumbnail::similar(ETextureThumbnail * thm1, xr_vector<xr_string> & sel_params)
    {
        BOOL res = m_TexParams.similar(thm1->m_TexParams, sel_params);
        /*
          if(res)
          {
              xr_vector<AnsiString>::iterator it = sel_params.begin();
              xr_vector<AnsiString>::iterator it_e = sel_params.end();

              for(;it!=it_e;++it)
              {
                 const AnsiString& par_name = *it;
                  if(par_name=="Format")
                  {
                      res = (m_TexParams.fmt == thm1->m_TexParams.fmt);
                  }else
                  if(par_name=="Type")
                  {
                      res = (m_TexParams.type == thm1->m_TexParams.type);
                  }else
                  if(par_name=="Width")
                  {
                      res = (_Width()==thm1->_Width());
                  }else
                  if(par_name=="Height")
                  {
                      res = (_Height()==thm1->_Height());
                  }else
                  if(par_name=="Alpha")
                  {
                      res = (_Alpha()==thm1->_Alpha());
                  }
                 if(!res)
                     break;
              }
          }
      */
        return res;
    }

    LPCSTR ETextureThumbnail::FormatString() { return m_TexParams.FormatString(); }
//------------------------------------------------------------------------------
/*

void ETextureThumbnail::Draw(HDC hdc, const Irect& R) Lord: Interpret
{
    if (0 == m_Pixels.size())
    {
        u32 image_w, image_h, image_a;
        xr_string fn_img = EFS.ChangeFileExt(m_Name.c_str(), ".tga");
        string_path fn;
        FS.update_path(fn, _textures_, fn_img.c_str());

        if (!FS.exist(fn))
        {
            fn_img = EFS.ChangeFileExt(m_Name.c_str(), ".dds");
            FS.update_path(fn, _game_textures_, fn_img.c_str());

            if (!FS.exist(fn))
            {
            //    ELog.Msg(mtError, "Can't make preview for texture '%s'.", m_Name.c_str());
                return;
            }
        }

        U32Vec data;
        u32 w, h, a;
        if (!Surface_Load(fn, data, image_w, image_h, image_a))
        {
         //   ELog.Msg(mtError, "Can't make preview for texture '%s'.", m_Name.c_str());
            return;
        }
        ImageLib.MakeThumbnailImage(this, data.begin(), image_w, image_h, image_a);
    }

    if (Valid())
    {
        Irect r;
        r.x1 = R.x1 + 1;
        r.y1 = R.y1 + 1;
        r.x2 = R.x2 - 1;
        r.y2 = R.y2 - 1;
        if (_Width() != _Height())
            FHelper.FillRect(hdc, r, 0x00000000);
        if (_Width() > _Height())
        {
            r.y2 -= r.height() - iFloor(r.height() * float(_Height()) / float(_Width()));
        }
        else
        {
            r.x2 -= r.width() - iFloor(r.width() * float(_Width()) / float(_Height()));
        }
        inherited::Draw(hdc, r);
    }
}
*/
#pragma endregion

#pragma region EThumbnailObject

//------------------------------------------------------------------------------
#define THM_OBJECT_VERSION 0x0012
//------------------------------------------------------------------------------
#define THM_CHUNK_OBJECTPARAM 0x0816

    //------------------------------------------------------------------------------
    EObjectThumbnail::EObjectThumbnail(LPCSTR src_name, bool bLoad) : EImageThumbnail(src_name, ETObject)
    {
        if (bLoad)
        {
            this->Load();
            
        }

    }

    //------------------------------------------------------------------------------

    EObjectThumbnail::~EObjectThumbnail() { m_Pixels.clear(); }
    //------------------------------------------------------------------------------

    void EObjectThumbnail::CreateFromData(u32 * p, u32 w, u32 h, int fc, int vc)
    {
        EImageThumbnail::CreatePixels(p, w, h);
        face_count = fc;
        vertex_count = vc;
    }

    //------------------------------------------------------------------------------

    bool EObjectThumbnail::Load(LPCSTR src_name, LPCSTR path)
    {
        //     string_path fn;
        //     strcpy(fn, EFS.ChangeFileExt(src_name ? src_name : m_Name.c_str(), ".thm").c_str());
        //     if (path)
        //         FS.update_path(fn, path, fn);
        //     else
        //         FS.update_path(fn, _objects_, fn);
        //     if (!FS.exist(fn))
        //         return false;
        xr_string bbb = path;
        bbb += src_name;
        IReader* F = FS.r_open(bbb.c_str());
        u16 version = 0;

        R_ASSERT(F->r_chunk(THM_CHUNK_VERSION, &version));
        if (version != THM_OBJECT_VERSION)
        {
            Msg("!Thumbnail: Unsupported version.");
            return false;
        }

        IReader* D = F->open_chunk(THM_CHUNK_DATA);
        R_ASSERT(D);
        m_Pixels.resize(THUMB_SIZE);
        D->r(&*m_Pixels.begin(), THUMB_SIZE * sizeof(u32));
        D->close();

        R_ASSERT(F->find_chunk(THM_CHUNK_TYPE));
        m_Type = THMType(F->r_u32());
        R_ASSERT(m_Type == ETObject);

        R_ASSERT(F->find_chunk(THM_CHUNK_OBJECTPARAM));
        face_count = F->r_u32();
        vertex_count = F->r_u32();

        m_Age = FS.get_file_age((xr_string(src_name) + xr_string(path)).c_str());

        FS.r_close(F);

        this->ConvertRawToTexture();
        return true;
    }

    //------------------------------------------------------------------------------

    void EObjectThumbnail::Save(int age, LPCSTR path)
    {
        if (!Valid())
            return;

        CMemoryWriter F;
        F.open_chunk(THM_CHUNK_VERSION);
        F.w_u16(THM_OBJECT_VERSION);
        F.close_chunk();

        F.w_chunk(THM_CHUNK_DATA | CFS_CompressMark, &*m_Pixels.begin(), m_Pixels.size() * sizeof(u32));

        F.open_chunk(THM_CHUNK_TYPE);
        F.w_u32(m_Type);
        F.close_chunk();

        F.open_chunk(THM_CHUNK_OBJECTPARAM);
        F.w_u32(face_count);
        F.w_u32(vertex_count);
        F.close_chunk();

        string_path fn;
        //.    if (path) 		FS.update_path(fn,path,m_Name.c_str());
        //.    else			FS.update_path(fn,_objects_,m_Name.c_str());

        strcpy(fn, m_Name.c_str());
        if (F.save_to(fn))
        {
            FS.set_file_age(fn, age ? age : m_Age);
        }
        else
        {
            Log("!Can't save thumbnail:", fn);
        }
    }

    //------------------------------------------------------------------------------

    void EObjectThumbnail::FillProp(PropItemVec & items)
    { // Lord: подумать
      //    PHelper().CreateCaption(items, "Face Count", xr_string(face_count).c_str());
      //  PHelper().CreateCaption(items, "Vertex Count", xr_string(vertex_count).c_str());
    }

    //------------------------------------------------------------------------------

    void EObjectThumbnail::FillInfo(PropItemVec & items)
    { // Lord: подумать
      //   PHelper().CreateCaption(items, "Face Count", xr_string(face_count).c_str());
      //   PHelper().CreateCaption(items, "Vertex Count", xr_string(vertex_count).c_str());
    }

    //------------------------------------------------------------------------------

#pragma endregion

#pragma region EThumbnailSound

//------------------------------------------------------------------------------
#define THM_SOUND_VERSION 0x0014
//------------------------------------------------------------------------------
#define THM_CHUNK_SOUNDPARAM 0x1000
#define THM_CHUNK_SOUNDPARAM2 0x1001
#define THM_CHUNK_SOUND_AI_DIST 0x1002

    //------------------------------------------------------------------------------
    ESoundThumbnail::ESoundThumbnail(LPCSTR src_name, bool bLoad) : ECustomThumbnail(src_name, ETSound)
    {
        m_fQuality = 0.f;
        m_fMinDist = 1.f;
        m_fMaxDist = 300.f;
        m_fMaxAIDist = 300.f;
        m_fBaseVolume = 1.f;
        m_uGameType = 0;
        if (bLoad)
            Load();
    }

    //------------------------------------------------------------------------------

    ESoundThumbnail::~ESoundThumbnail() {}
    //------------------------------------------------------------------------------

    bool ESoundThumbnail::Load(LPCSTR src_name, LPCSTR path)
    {
        string_path fn;
        strcpy(fn, EFS.ChangeFileExt(src_name ? src_name : m_Name.c_str(), ".thm").c_str());
        if (path)
            FS.update_path(fn, path, fn);
        else
            FS.update_path(fn, _sounds_, fn);
        if (!FS.exist(fn))
            return false;

        IReader* F = FS.r_open(fn);
        u16 version = 0;

        R_ASSERT(F->r_chunk(THM_CHUNK_VERSION, &version));
        if (version != THM_SOUND_VERSION)
        {
            Msg("!Thumbnail: Unsupported version.");
            return false;
        }

        R_ASSERT(F->find_chunk(THM_CHUNK_TYPE));
        m_Type = THMType(F->r_u32());
        R_ASSERT(m_Type == ETSound);

        R_ASSERT(F->find_chunk(THM_CHUNK_SOUNDPARAM));
        m_fQuality = F->r_float();
        m_fMinDist = F->r_float();
        m_fMaxDist = F->r_float();
        m_uGameType = F->r_u32();

        if (F->find_chunk(THM_CHUNK_SOUNDPARAM2))
            m_fBaseVolume = F->r_float();

        if (F->find_chunk(THM_CHUNK_SOUND_AI_DIST))
            m_fMaxAIDist = F->r_float();
        else
            m_fMaxAIDist = m_fMaxDist;

        m_Age = FS.get_file_age(fn);

        FS.r_close(F);

        return true;
    }

    //------------------------------------------------------------------------------

    void ESoundThumbnail::Save(int age, LPCSTR path)
    {
        if (!Valid())
            return;

        CMemoryWriter F;
        F.open_chunk(THM_CHUNK_VERSION);
        F.w_u16(THM_SOUND_VERSION);
        F.close_chunk();

        F.open_chunk(THM_CHUNK_TYPE);
        F.w_u32(m_Type);
        F.close_chunk();

        F.open_chunk(THM_CHUNK_SOUNDPARAM);
        F.w_float(m_fQuality);
        F.w_float(m_fMinDist);
        F.w_float(m_fMaxDist);
        F.w_u32(m_uGameType);
        F.close_chunk();

        F.open_chunk(THM_CHUNK_SOUNDPARAM2);
        F.w_float(m_fBaseVolume);
        F.close_chunk();

        F.open_chunk(THM_CHUNK_SOUND_AI_DIST);
        F.w_float(m_fMaxAIDist);
        F.close_chunk();

        string_path fn;
        if (path)
            FS.update_path(fn, path, m_Name.c_str());
        else
            FS.update_path(fn, _sounds_, m_Name.c_str());

        if (F.save_to(fn))
        {
            FS.set_file_age(fn, age ? age : m_Age);
        }
        else
        {
            Log("!Can't save thumbnail:", fn);
        }
    }

    //------------------------------------------------------------------------------

    // Lord: Подумать над TProperties ибо это уже ImGui

    bool ESoundThumbnail::OnMaxAIDistAfterEdit(PropValue * sender, float& edit_val)
    {
        /*
        



        TProperties* P = sender->Owner()->Owner();
        VERIFY(P);
        PropItem* S = P->FindItem("Max Dist");
        VERIFY(S);
        FloatValue* V = dynamic_cast<FloatValue*>(S->GetFrontValue());
        VERIFY(V);
        float max_val = V->GetValue();
        return edit_val < max_val;
        */
        return false;
    }

    void ESoundThumbnail::OnMaxDistChange(PropValue * sender)
    {
        /*
        



        



        FloatValue* SV = dynamic_cast<FloatValue*>(sender);
        VERIFY(SV);
        TProperties* P = sender->Owner()->Owner();
        VERIFY(P);
        PropItem* S = P->FindItem("Max AI Dist");
        VERIFY(S);
        bool bChanged = false;
        for (PropItem::PropValueVec::iterator it = S->Values().begin(); S->Values().end() != it; ++it)
        {
            FloatValue* CV = dynamic_cast<FloatValue*>(*it);
            VERIFY(CV);
            CV->lim_mx = *SV->value;
            if (*CV->value > CV->lim_mx)
            {
              //  ELog.DlgMsg(mtInformation, "'Max AI Dist' <= 'Max Dist'. 'Max AI Dist' will be clamped.");
                bChanged = true;
                *CV->value = CV->lim_mx;
            }
            if (!CV->Equal(S->Values().front()))
                S->m_Flags.set(PropItem::flMixed, TRUE);
        }
        if (bChanged)
        {
            P->Modified();
            P->RefreshForm();
        }
        */
    }

    void ESoundThumbnail::FillProp(PropItemVec & items)
    {
        FloatValue* V = 0;
        // PHelper().CreateFloat(items, "Quality", &m_fQuality);
        //  PHelper().CreateFloat(items, "Min Dist", &m_fMinDist, 0.01f, 1000.f);
        //  V = PHelper().CreateFloat(items, "Max Dist", &m_fMaxDist, 0.1f, 1000.f);
        //  V->OnChangeEvent.bind(this, &ESoundThumbnail::OnMaxDistChange);
        //   V = PHelper().CreateFloat(items, "Max AI Dist", &m_fMaxAIDist, 0.1f, 1000.f);
        //   V->OnAfterEditEvent.bind(this, &ESoundThumbnail::OnMaxAIDistAfterEdit);
        //    PHelper().CreateFloat(items, "Base Volume", &m_fBaseVolume, 0.f, 2.f);
        //    PHelper().CreateToken32(items, "Game Type", &m_uGameType, anomaly_type_token);
    }

    //------------------------------------------------------------------------------

    void ESoundThumbnail::FillInfo(PropItemVec & items)
    {
        // Lord: подумать надо как это переписать
        //  PHelper().CreateCaption(items, "Quality", AnsiString().sprintf("%3.2f", m_fQuality).c_str());
        //   PHelper().CreateCaption(items, "Min Dist", AnsiString().sprintf("%3.2f", m_fMinDist).c_str());
        ///  PHelper().CreateCaption(items, "Max Dist", AnsiString().sprintf("%3.2f", m_fMaxDist).c_str());
        //  PHelper().CreateCaption(items, "Max AI Dist", AnsiString().sprintf("%3.2f", m_fMaxAIDist).c_str());
        // PHelper().CreateCaption(items, "Base Volume", AnsiString().sprintf("%3.2f", m_fBaseVolume).c_str());
        //    PHelper().CreateCaption(items, "Game Type", get_token_name(anomaly_type_token, m_uGameType));
    }

    //------------------------------------------------------------------------------

#pragma endregion

#pragma region EThumbnailGroup

//------------------------------------------------------------------------------
#define THM_GROUP_VERSION 0x0001
//------------------------------------------------------------------------------
#define THM_CHUNK_GROUPPARAM 0x0001

    //------------------------------------------------------------------------------
    EGroupThumbnail::EGroupThumbnail(LPCSTR src_name, bool bLoad) : EImageThumbnail(src_name, ETObject)
    {
        if (bLoad)
            Load();
    }

    //------------------------------------------------------------------------------

    EGroupThumbnail::~EGroupThumbnail() { m_Pixels.clear(); }
    //------------------------------------------------------------------------------

    void EGroupThumbnail::CreateFromData(u32 * p, u32 w, u32 h, const SStringVec& lst)
    {
        EImageThumbnail::CreatePixels(p, w, h);
        objects = lst;
    }

    //------------------------------------------------------------------------------

    bool EGroupThumbnail::Load(LPCSTR src_name, LPCSTR path)
    {
        string_path fn;
        strcpy(fn, EFS.ChangeFileExt(src_name ? src_name : m_Name.c_str(), ".thm").c_str());
        if (path)
            FS.update_path(fn, path, fn);
        else
            FS.update_path(fn, _objects_, fn);
        if (!FS.exist(fn))
            return false;

        IReader* F = FS.r_open(fn);
        u16 version = 0;

        R_ASSERT(F->r_chunk(THM_CHUNK_VERSION, &version));
        if (version != THM_GROUP_VERSION)
        {
            Msg("!Thumbnail: Unsupported version.");
            return false;
        }

        IReader* D = F->open_chunk(THM_CHUNK_DATA);
        R_ASSERT(D);
        m_Pixels.resize(THUMB_SIZE);
        D->r(&*m_Pixels.begin(), THUMB_SIZE * sizeof(u32));
        D->close();

        R_ASSERT(F->find_chunk(THM_CHUNK_TYPE));
        m_Type = THMType(F->r_u32());
        R_ASSERT(m_Type == ETObject);

        R_ASSERT(F->find_chunk(THM_CHUNK_GROUPPARAM));
        objects.resize(F->r_u32());
        for (SStringVecIt it = objects.begin(); it != objects.end(); it++)
            F->r_stringZ(*it);

        m_Age = FS.get_file_age(fn);

        FS.r_close(F);

        return true;
    }

    //------------------------------------------------------------------------------

    void EGroupThumbnail::Save(int age, LPCSTR path)
    {
        if (!Valid())
            return;

        CMemoryWriter F;
        F.open_chunk(THM_CHUNK_VERSION);
        F.w_u16(THM_GROUP_VERSION);
        F.close_chunk();

        F.w_chunk(THM_CHUNK_DATA | CFS_CompressMark, &*m_Pixels.begin(), m_Pixels.size() * sizeof(u32));

        F.open_chunk(THM_CHUNK_TYPE);
        F.w_u32(m_Type);
        F.close_chunk();

        F.open_chunk(THM_CHUNK_GROUPPARAM);
        F.w_u32(objects.size());
        for (SStringVecIt it = objects.begin(); it != objects.end(); it++)
            F.w_stringZ(*it);
        F.close_chunk();

        string_path fn;
        if (path)
            FS.update_path(fn, path, m_Name.c_str());
        else
            FS.update_path(fn, _objects_, m_Name.c_str());
        if (F.save_to(fn))
        {
            FS.set_file_age(fn, age ? age : m_Age);
        }
        else
        {
            Log("!Can't save thumbnail:", fn);
        }
    }

    //------------------------------------------------------------------------------

    void EGroupThumbnail::FillProp(PropItemVec & items)
    { // Lord: подумать
      // PHelper().CreateCaption(items, "Objects\\Count", xr_string(objects.size()).c_str());
      //  for (SStringVecIt it = objects.begin(); it != objects.end(); it++)
      // //       PHelper().CreateCaption(items, xr_string().sprintf("Objects\\#%d", it - objects.begin()).c_str(),
      // it->c_str());
    }

    //------------------------------------------------------------------------------

    void EGroupThumbnail::FillInfo(PropItemVec & items) { FillProp(items); }
    //------------------------------------------------------------------------------

#pragma endregion
