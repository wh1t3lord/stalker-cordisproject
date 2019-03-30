//----------------------------------------------------
// file: EditObject.cpp
//----------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

#include "xrCore/FMesh.hpp"

#include "EditObject.h"
#include "EditMesh.h"

#include "xrCore/Animation/Motion.hpp"
#include "xrCore/Animation/Bone.hpp"
#include "ImageManager.h"

#pragma region EditObject
// mimimal bounding box size
float g_MinBoxSize = 0.05f;

CEditableObject::CEditableObject(LPCSTR name) : m_physics_shell(0), m_object_xform(0)
{
    m_LibName = name;

    m_objectFlags.zero();
    m_ObjectVersion = 0;

    vs_SkeletonGeom = 0;
    m_BBox.invalidate();

    m_LoadState.zero();

    m_ActiveSMotion = 0;

    t_vPosition.set(0.f, 0.f, 0.f);
    t_vScale.set(1.f, 1.f, 1.f);
    t_vRotate.set(0.f, 0.f, 0.f);

    a_vPosition.set(0.f, 0.f, 0.f);
    a_vRotate.set(0.f, 0.f, 0.f);

    bOnModified = false;

    m_RefCount = 0;

    m_LODShader = 0;

    m_CreateName = "unknown";
    m_CreateTime = 0;
    m_ModifName = "unknown";
    m_ModifTime = 0;
}

CEditableObject::~CEditableObject() { ClearGeometry(); }
//----------------------------------------------------

void CEditableObject::VerifyMeshNames()
{
    int idx = 0;
    string1024 nm, pref;
    for (EditMeshIt m_def = m_Meshes.begin(); m_def != m_Meshes.end(); m_def++)
    {
        strcpy(pref, (*m_def)->m_Name.size() ? (*m_def)->m_Name.c_str() : "mesh");
        _Trim(pref);
        strcpy(nm, pref);
        while (FindMeshByName(nm, *m_def))
            sprintf(nm, "%s%2d", pref, idx++);
        (*m_def)->SetName(nm);
    }
}

bool CEditableObject::ContainsMesh(const CEditableMesh* m)
{
    VERIFY(m);
    for (EditMeshIt m_def = m_Meshes.begin(); m_def != m_Meshes.end(); m_def++)
        if (m == (*m_def))
            return true;
    return false;
}

CEditableMesh* CEditableObject::FindMeshByName(const char* name, CEditableMesh* Ignore)
{
    for (EditMeshIt m = m_Meshes.begin(); m != m_Meshes.end(); m++)
        if ((Ignore != (*m)) && (xr_stricmp((*m)->Name().c_str(), name) == 0))
            return (*m);
    return 0;
}

void CEditableObject::ClearGeometry()
{
    OnDeviceDestroy();  

    if (!m_Meshes.empty())
        for (EditMeshIt m = m_Meshes.begin(); m != m_Meshes.end(); m++)
            xr_delete(*m);
    if (!m_Surfaces.empty())
        for (SurfaceIt s_it = m_Surfaces.begin(); s_it != m_Surfaces.end(); s_it++)
            xr_delete(*s_it);
    m_Meshes.clear();
    m_Surfaces.clear();

    // bones
    for (BoneVec::iterator b_it = m_Bones.begin(); b_it != m_Bones.end(); b_it++)
        xr_delete(*b_it);

    m_Bones.clear();
    // skeletal motions
    for (SMotionIt s_it = m_SMotions.begin(); s_it != m_SMotions.end(); s_it++)
        xr_delete(*s_it);
    m_SMotions.clear();

    m_ActiveSMotion = 0;
}

int CEditableObject::GetFaceCount(bool bMatch2Sided, bool bIgnoreOCC)
{
    int cnt = 0;
    for (EditMeshIt m = m_Meshes.begin(); m != m_Meshes.end(); m++)
        cnt += (*m)->GetFaceCount(bMatch2Sided, bIgnoreOCC);
    return cnt;
}

int CEditableObject::GetSurfFaceCount(const char* surf_name)
{
    int cnt = 0;
    CSurface* surf = FindSurfaceByName(surf_name);
    for (EditMeshIt m = m_Meshes.begin(); m != m_Meshes.end(); m++)
        cnt += (*m)->GetSurfFaceCount(surf);
    return cnt;
}

int CEditableObject::GetVertexCount()
{
    int cnt = 0;
    for (EditMeshIt m = m_Meshes.begin(); m != m_Meshes.end(); m++)
        cnt += (*m)->GetVertexCount();
    return cnt;
}

void CEditableObject::UpdateBox()
{
    VERIFY(!m_Meshes.empty());
    EditMeshIt m = m_Meshes.begin();
    m_BBox.invalidate();

    for (; m != m_Meshes.end(); ++m)
    {
        Fbox meshbox;
        (*m)->GetBox(meshbox);
        for (int i = 0; i < 8; ++i)
        {
            Fvector pt;
            meshbox.getpoint(i, pt);
            m_BBox.modify(pt);
        }
    }
}

//----------------------------------------------------
void CEditableObject::RemoveMesh(CEditableMesh* mesh)
{
    EditMeshIt m_it = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
    VERIFY(m_it != m_Meshes.end());
    m_Meshes.erase(m_it);
    xr_delete(mesh);
}

void CEditableObject::TranslateToWorld(const Fmatrix& parent)
{
    EditMeshIt m = m_Meshes.begin();
    for (; m != m_Meshes.end(); m++)
        (*m)->Transform(parent);

    // OnDeviceDestroy(); Lord: подумать

    UpdateBox();
}

CSurface* CEditableObject::FindSurfaceByName(const char* surf_name, int* s_id)
{
    for (SurfaceIt s_it = m_Surfaces.begin(); s_it != m_Surfaces.end(); s_it++)
        if (xr_stricmp((*s_it)->_Name(), surf_name) == 0)
        {
            if (s_id)
                *s_id = s_it - m_Surfaces.begin();
            return *s_it;
        }
    return 0;
}

LPCSTR CEditableObject::GenerateSurfaceName(const char* base_name)
{
    static string1024 nm;
    strcpy(nm, base_name);
    if (FindSurfaceByName(nm))
    {
        DWORD idx = 0;
        do
        {
            sprintf(nm, "%s_%d", base_name, idx);
            idx++;
        } while (FindSurfaceByName(nm));
    }
    return nm;
}

bool CEditableObject::VerifyBoneParts()
{
    std::vector<u8> b_use(BoneCount(), 0);
    for (BPIt bp_it = m_BoneParts.begin(); bp_it != m_BoneParts.end(); bp_it++)
        for (int i = 0; i < int(bp_it->bones.size()); i++)
        {
            int idx = FindBoneByNameIdx(bp_it->bones[i].c_str());
            if (idx == -1)
            {
                bp_it->bones.erase(bp_it->bones.begin() + i);
                i--;
            }
            else
            {
                b_use[idx]++;
            }
        }

    for (U8It u_it = b_use.begin(); u_it != b_use.end(); u_it++)
        if (*u_it != 1)
            return false;
    return true;
}

void CEditableObject::PrepareOGFDesc(ogf_desc& desc)
{
    string512 tmp;
    desc.source_file = m_LibName.c_str();
    desc.create_name = m_CreateName.c_str();
    desc.create_time = m_CreateTime;
    desc.modif_name = m_ModifName.c_str();
    desc.modif_time = m_ModifTime;
    desc.build_name = strconcat(sizeof(tmp), tmp, "\\\\", Core.CompName, "\\", Core.UserName);
    ctime(&desc.build_time);
}

void CEditableObject::SetVersionToCurrent(BOOL bCreate, BOOL bModif)
{
    string512 tmp;
    if (bCreate)
    {
        m_CreateName = strconcat(sizeof(tmp), tmp, "\\\\", Core.CompName, "\\", Core.UserName);
        m_CreateTime = time(NULL);
    }
    if (bModif)
    {
        m_ModifName = strconcat(sizeof(tmp), tmp, "\\\\", Core.CompName, "\\", Core.UserName);
        m_ModifTime = time(NULL);
    }
}

void CEditableObject::GetFaceWorld(const Fmatrix& parent, CEditableMesh* M, int idx, Fvector* verts)
{
    const Fvector* PT[3];
    M->GetFacePT(idx, PT);
    parent.transform_tiny(verts[0], *PT[0]);
    parent.transform_tiny(verts[1], *PT[1]);
    parent.transform_tiny(verts[2], *PT[2]);
}

void CEditableObject::Optimize()
{
    for (EditMeshIt m_def = m_Meshes.begin(); m_def != m_Meshes.end(); m_def++)
    {
        (*m_def)->OptimizeMesh(false);
        (*m_def)->RebuildVMaps();
    }
}

bool CEditableObject::Validate()
{
    bool bRes = true;
    for (SurfaceIt s_it = m_Surfaces.begin(); s_it != m_Surfaces.end(); s_it++)
        if (false == (*s_it)->Validate())
        {
            Msg("!Invalid surface found: Object [%s], Surface [%s].", GetName(), (*s_it)->_Name());
            bRes = false;
        }
    for (EditMeshIt m_def = m_Meshes.begin(); m_def != m_Meshes.end(); m_def++)
        if (false == (*m_def)->Validate())
        {
            Msg("!Invalid mesh found: Object [%s], Mesh [%s].", m_LibName.c_str(), (*m_def)->Name().c_str());
            bRes = false;
        }
    return bRes;
}

//----------------------------------------------------------------------------
//#ifdef DEBUG

LPCSTR CEditableObject::LL_BoneName_dbg(u16 ID) { return GetBone(ID)->Name().c_str(); }
//#endif
CBoneInstance& CEditableObject::LL_GetBoneInstance(u16 bone_id) { return *GetBone(bone_id); }
CBoneData& CEditableObject::LL_GetData(u16 bone_id)
{
    VERIFY(false);
    static CBoneData dummy_bone_data(0);
    return dummy_bone_data;
}

Fmatrix& CEditableObject::LL_GetTransform_R(u16 bone_id)
{
    //	VERIFY(false);
    // static Fmatrix dummy_matrix;
    return GetBone(bone_id)->_RenderTransform();
}

Fobb& CEditableObject::LL_GetBox(u16 bone_id)
{
    VERIFY(false);
    static Fobb dummy_box;
    return dummy_box;
}
#pragma endregion

#pragma region EditObjectImport

#pragma region LW_ShaderDef
#pragma pack(push, 1)
typedef struct st_XRShader
{
    char en_name[64];
    int en_idx;
    char lc_name[64];
    int lc_idx;
    char gm_name[64];
    int gm_idx;
    char* desc;
} XRShader;
#pragma pack(pop)

#define SH_PLUGIN_NAME "!XRayShader"
#pragma endregion

extern "C" {
#include "utils/LWO/lwo2.h"
};

#include "Layers/xrRender/ResourceManager.h"

#include "Common/Platform.hpp"
extern "C" XR_IMPORT lwObject* LWOImportObject(char* filename);
extern "C" XR_IMPORT void LWOCloseFile(lwObject* object);

bool CEditableObject::ImportLWO(const char* fn, bool optimize)
{
    string512 fname;
    strcpy(fname, fn);

    //#ifdef _EDITOR
    lwObject* lwObj = LWOImportObject(fname);
    //#else
    /// u3//2 errId;
    // int errPos;
    //  lwObject* lwObj = lwGetObject(fname, &errId, &errPos);
    //#endif

    if (!lwObj)
    {
        //  ELog.DlgMsg(mtError, "Can't import LWO object file!"); Lord: Interpret
        return false;
    }
    //  ELog.Msg(mtInformation, "CEditableObject: import lwo %s...", fname); Lord: interpret
    bool result = false; // assume fail
    // parse lwo object
    m_Meshes.reserve(lwObj->nlayers);
    m_Surfaces.reserve(lwObj->nsurfs);
    int surfaceId = 0;
    for (lwSurface* lwSurf = lwObj->surf; lwSurf; lwSurf = lwSurf->next)
    {
        lwSurf->alpha_mode = surfaceId; // save surface id for internal use
        auto surf = new CSurface();
        m_Surfaces.push_back(surf);
        surf->SetName(lwSurf->name && lwSurf->name[0] ? lwSurf->name : "Default");
        surf->m_Flags.set(CSurface::sf2Sided, lwSurf->sideflags == 3);
        xr_string enName = "default", lcName = "default", gmName = "default";
        if (lwSurf->nshaders && !xr_stricmp(lwSurf->shader->name, SH_PLUGIN_NAME))
        {
            auto shader = (XRShader*)lwSurf->shader->data;
            enName = shader->en_name;
            lcName = shader->lc_name;
            gmName = shader->gm_name;
        }
        else
            //  ELog.Msg(mtError, "CEditableObject: Shader not found on surface '%s'.", surf->_Name());

            if (!RImplementation.Resources->_FindBlender(enName.c_str()))
        {
            // ELog.Msg(mtError,
            //   "CEditableObject: Render shader '%s' - can't find in library.\n"
            //   "Using 'default' shader on surface '%s'.",
            //   enName.c_str(), surf->_Name());
            enName = "default";
        }
        if (!RImplementation.ShaderXRLC.Get(lcName.c_str()))
        {
            //  ELog.Msg(mtError,
            //      "CEditableObject: Compiler shader '%s' - can't find in library.\n"
            //      "Using 'default' shader on surface '%s'.",
            //      lcName.c_str(), surf->_Name());
            lcName = "default";
        }
        if (!GMLib.GetMaterial(gmName.c_str()))
        {
            // ELog.Msg(mtError,
            //     "CEditableObject: Game material '%s' - can't find in library.\n"
            //     "Using 'default' material on surface '%s'.",
            //     lcName.c_str(), surf->_Name());
            gmName = "default";
        }

        // fill texture layers
        u32 textureCount = 0;
        for (st_lwTexture* Itx = lwSurf->color.tex; Itx; Itx = Itx->next)
        {
            string1024 tname = "";
            textureCount++;
            int cidx = -1;
            if (Itx->type == ID_IMAP)
                cidx = Itx->param.imap.cindex;
            else
            {
                // ELog.DlgMsg(mtError, "Import LWO (Surface '%s'): 'Texture' is not Image Map!", surf->_Name()); Lord:
                // Interpret
                goto importFailed;
            }
            if (cidx != -1)
            {
                // get textures
                for (st_lwClip* lwClip = lwObj->clip; lwClip; lwClip = lwClip->next)
                {
                    if (cidx == lwClip->index && lwClip->type == ID_STIL)
                    {
                        strcpy(tname, lwClip->source.still.name);
                        break;
                    }
                }
                if (tname[0] == 0)
                {
                    //  ELog.DlgMsg(mtError,
                    //      "Import LWO (Surface '%s'): "
                    //     "'Texture' name is empty or non 'STIL' type!",
                    //     surf->_Name()); Lord: Interpret
                    goto importFailed;
                }
                string256 textureName;
                _splitpath(tname, nullptr, nullptr, textureName, nullptr);
                surf->SetTexture(EFS.AppendFolderToName(textureName, 256, 1, true));
                // get vmap refs
                surf->SetVMap(Itx->param.imap.vmap_name);
            }
        }
        if (!surf->_VMap() || *surf->_VMap() == 0)
        {
            //  ELog.DlgMsg(mtError, "Invalid surface '%s'. Empty VMap.", surf->_Name());
            goto importFailed;
        }
        if (!surf->_Texture() || *surf->_Texture() == 0)
        {
            //  ELog.DlgMsg(mtError, "Can't create shader. Invalid surface '%s'. Textures empty.", surf->_Name());
            goto importFailed;
        }
        if (enName.c_str() == 0)
        {
            //   ELog.DlgMsg(mtError, "Can't create shader. Invalid surface '%s'. Shader empty.", surf->_Name());
            goto importFailed;
        }
        surf->SetShader(enName.c_str());
        surf->SetShaderXRLC(lcName.c_str());
        surf->SetGameMtl(gmName.c_str());
        surf->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | textureCount << D3DFVF_TEXCOUNT_SHIFT);
        surfaceId++;
    }
    // process mesh layers
    for (st_lwLayer* lwLayer = lwObj->layer; lwLayer; lwLayer = lwLayer->next)
    {
        auto mesh = new CEditableMesh(this);
        m_Meshes.push_back(mesh);
        mesh->SetName(lwLayer->name ? lwLayer->name : "");
        auto bbox = lwLayer->bbox;
        mesh->m_Box.set(bbox[0], bbox[1], bbox[2], bbox[3], bbox[4], bbox[5]);
        // parse mesh(lwo-layer) data
        if (lwLayer->nvmaps == 0)
        {
            //   ELog.DlgMsg(mtError, "Import LWO: Mesh layer must contain UV map!");
            goto importFailed;
        }
        // XXX nitrocaster: incompatible with multithreaded import
        static xr_map<void*, int> vmIndices;
        vmIndices.clear();
        int vmIndex = 0;
        for (st_lwVMap* lwVmap = lwLayer->vmap; lwVmap; lwVmap = lwVmap->next)
        {
            switch (lwVmap->type)
            {
            case ID_TXUV:
            {
                if (lwVmap->dim != 2)
                {
                    //    ELog.DlgMsg(mtError, "Import LWO: 'UV Map' must be equal to 2!");
                    goto importFailed;
                }
                mesh->m_VMaps.push_back(new st_VMap(lwVmap->name, vmtUV, !!lwVmap->perpoly));
                st_VMap* vmap = mesh->m_VMaps.back();
                vmap->copyfrom(*lwVmap->val, lwVmap->nverts);
                // flip uv
                for (int uvIndex = 0; uvIndex < vmap->size(); uvIndex++)
                {
                    Fvector2& uv = vmap->getUV(uvIndex);
                    uv.y = 1.f - uv.y;
                }
                vmIndices[lwVmap] = vmIndex++;
                break;
            }
            case ID_WGHT:
            {
                if (lwVmap->dim != 1)
                {
                    //   ELog.DlgMsg(mtError, "Import LWO: 'Weight' must be equal to 1!"); Lord: Interpret
                    goto importFailed;
                }
                mesh->m_VMaps.push_back(new st_VMap(lwVmap->name, vmtWeight, false));
                st_VMap* vmap = mesh->m_VMaps.back();
                vmap->copyfrom(*lwVmap->val, lwVmap->nverts);
                vmIndices[lwVmap] = vmIndex++;
                break;
            }
            case ID_PICK: goto importFailed; // ELog.Msg(mtError, "Found 'PICK' VMAP. Import failed.");
            case ID_MNVW:
                goto importFailed; // ELog.Msg(mtError, "Found 'MNVW' VMAP. Import failed."); goto importFailed;
            case ID_MORF:
                goto importFailed; // ELog.Msg(mtError, "Found 'MORF' VMAP. Import failed."); goto importFailed;
            case ID_SPOT:
                goto importFailed; // ELog.Msg(mtError, "Found 'SPOT' VMAP. Import failed."); goto importFailed;
            case ID_RGB: goto importFailed; // ELog.Msg(mtError, "Found 'RGB' VMAP. Import failed."); goto importFailed;
            case ID_RGBA:
                goto importFailed; // ELog.Msg(mtError, "Found 'RGBA' VMAP. Import failed."); goto importFailed;
            }
        }
        // points
        mesh->m_VertCount = lwLayer->point.count;
        mesh->m_Vertices = xr_alloc<Fvector>(mesh->m_VertCount);
        for (int i = 0; i < lwLayer->point.count; i++)
        {
            st_lwPoint& lwPoint = lwLayer->point.pt[i];
            Fvector& vertex = mesh->m_Vertices[i];
            vertex.set(lwPoint.pos);
        }
        // polygons
        mesh->m_FaceCount = lwLayer->polygon.count;
        mesh->m_Faces = xr_alloc<st_Face>(mesh->m_FaceCount);
        mesh->m_SmoothGroups = xr_alloc<u32>(mesh->m_FaceCount);
        memset(mesh->m_SmoothGroups, u32(-1), mesh->m_FaceCount);
        mesh->m_VMRefs.reserve(lwLayer->polygon.count * 3);
        xr_vector<int> surfIds(lwLayer->polygon.count);
        for (int i = 0; i < lwLayer->polygon.count; i++)
        {
            st_Face& face = mesh->m_Faces[i];
            st_lwPolygon& lwPoly = lwLayer->polygon.pol[i];
            if (lwPoly.nverts != 3)
            {
                // ELog.DlgMsg(mtError, "Import LWO: Face must contain only 3 vertices!"); Lord: Interpret
                goto importFailed;
            }
            for (int fvIndex = 0; fvIndex < 3; fvIndex++)
            {
                st_lwPolVert& lwFaceVert = lwPoly.v[fvIndex];
                st_FaceVert& faceVert = face.pv[fvIndex];
                faceVert.pindex = lwFaceVert.index;
                mesh->m_VMRefs.push_back(st_VMapPtLst());
                st_VMapPtLst& vmPointList = mesh->m_VMRefs.back();
                faceVert.vmref = mesh->m_VMRefs.size() - 1;
                // parse uv-map
                st_lwPoint& lwPoint = lwLayer->point.pt[faceVert.pindex];
                if (lwFaceVert.nvmaps == 0 && lwPoint.nvmaps == 0)
                {
                    //       ELog.DlgMsg(mtError, "Found mesh without UV's!"); Lord: Interpret
                    goto importFailed;
                }
                xr_vector<st_VMapPt> vmPoints;
                AStringVec names;
                // process polys
                for (int j = 0; j < lwFaceVert.nvmaps; j++)
                {
                    if (lwFaceVert.vm[j].vmap->type != ID_TXUV)
                        continue;
                    vmPoints.push_back(st_VMapPt());
                    st_VMapPt& pt = vmPoints.back();
                    pt.vmap_index = vmIndices[lwFaceVert.vm[j].vmap]; // VMap id
                    names.push_back(lwFaceVert.vm[j].vmap->name);
                    pt.index = lwFaceVert.vm[j].index;
                }
                // process points
                for (int j = 0; j < lwPoint.nvmaps; j++)
                {
                    if (lwPoint.vm[j].vmap->type != ID_TXUV)
                        continue;
                    if (std::find(names.begin(), names.end(), lwPoint.vm[j].vmap->name) != names.end())
                        continue;
                    vmPoints.push_back(st_VMapPt());
                    st_VMapPt& pt = vmPoints.back();
                    pt.vmap_index = vmIndices[lwPoint.vm[j].vmap]; // VMap id
                    pt.index = lwPoint.vm[j].index;
                }
                auto cmpFunc = [](const st_VMapPt& a, const st_VMapPt& b) { return a.vmap_index < b.vmap_index; };
                std::sort(vmPoints.begin(), vmPoints.end(), cmpFunc);
                // parse weight-map
                for (int j = 0; j < lwPoint.nvmaps; j++)
                {
                    if (lwPoint.vm[j].vmap->type != ID_WGHT)
                        continue;
                    vmPoints.push_back(st_VMapPt());
                    st_VMapPt& pt = vmPoints.back();
                    pt.vmap_index = vmIndices[lwPoint.vm[j].vmap]; // VMap id
                    pt.index = lwPoint.vm[j].index;
                }
                vmPointList.count = vmPoints.size();
                vmPointList.pts = xr_alloc<st_VMapPt>(vmPointList.count);
                memcpy(vmPointList.pts, &*vmPoints.begin(), vmPointList.count * sizeof(st_VMapPt));
            }
            // lwPoly.surf->alpha_mode stores reviously saved surface id
            surfIds[i] = lwPoly.surf->alpha_mode;
        }
        for (u32 polyId = 0; polyId < mesh->GetFCount(); polyId++)
            mesh->m_SurfFaces[m_Surfaces[surfIds[polyId]]].push_back(polyId);
        if (optimize)
            mesh->OptimizeMesh(false);
        mesh->RebuildVMaps();
    }
    result = true;
importFailed:
    LWOCloseFile(lwObj);

    if (result)
    {
        VerifyMeshNames();
        char* ext = strext(fname);
        m_LoadName = ext ? strcpy(ext, ".object") : strcat(fname, ".object");
    }
    else
    {
        //  ELog.DlgMsg(mtError, "Can't parse LWO object."); Lord: Interpret
    }

    return result;
}
#pragma endregion

#pragma region EditObjectMotions

#include "Layers/xrRender/SkeletonAnimated.h"
#include "Layers/xrRender/AnimationKeyCalculate.h"

// bool check_scale(Fmatrix F) { return true; }

//----------------------------------------------------
class fBoneNameEQ
{
    shared_str name;

public:
    fBoneNameEQ(shared_str N) : name(N){};

    IC

        bool
        operator()(CBone* B)
    {
        return (xr_strcmp(B->Name(), name) == 0);
    }
};

class fBoneWMNameEQ
{
    shared_str wm_name;

public:
    fBoneWMNameEQ(shared_str N) : wm_name(N){};

    IC

        bool
        operator()(CBone* B)
    {
        return (xr_strcmp(B->WMap(), wm_name) == 0);
    }
};

//----------------------------------------------------
#ifdef _EDITOR
extern CBone* bone_to_delete;
extern u32 bone_to_delete_frame;

void CEditableObject::OnFrame()
{
    if (IsSkeleton())
    {
        BoneVec& lst = m_Bones;
        if (IsSMotionActive())
        {
            Fvector R, T;
            int i = 0;
            for (BoneIt b_it = lst.begin(); b_it != lst.end(); b_it++, i++)
            {
                m_ActiveSMotion->_Evaluate(i, m_SMParam.Frame(), T, R);
                (*b_it)->_Update(T, R);
            }
            m_SMParam.Update(EDevice.fTimeDelta, m_ActiveSMotion->fSpeed, !m_ActiveSMotion->m_Flags.is(esmStopAtEnd));
        }
        else
        {
            // for (BoneIt b_it=lst.begin(); b_it!=lst.end(); b_it++) (*b_it)->Reset();
        }
        CalculateAnimation(m_ActiveSMotion);
    }
    if (bone_to_delete)
    {
        if (EDevice.dwFrame > bone_to_delete_frame + 3)
            xr_delete(bone_to_delete);
    }
}
#endif

void CEditableObject::OnBindTransformChange()
{
    for (auto mesh_it = FirstMesh(); mesh_it != LastMesh(); ++mesh_it)
    {
        CEditableMesh* MESH = *mesh_it;
        MESH->UnloadSVertices(true);
    }
    GotoBindPose();
}

void CEditableObject::GotoBindPose()
{
    BoneVec& lst = m_Bones;
    for (auto& it : lst)
        it->Reset();
    CalculateAnimation(nullptr);
#ifdef _EDITOR
    UI->RedrawScene();
#endif
}

CSMotion* CEditableObject::ResetSAnimation(bool bGotoBindPose)
{
    CSMotion* M = m_ActiveSMotion;
    SetActiveSMotion(0);
    if (bGotoBindPose)
        GotoBindPose();
    return M;
}

//----------------------------------------------------
// Skeletal motion
//----------------------------------------------------
static void CalculateAnimBone(CBone* bone, CSMotion* motion, Fmatrix& parent)
{
    Flags8 flags;
    flags.zero();
    if (motion)
        flags = motion->GetMotionFlags(bone->SelfID);

    Fmatrix& M = bone->_MTransform();
    Fmatrix& L = bone->_LTransform();

    const Fvector& r = bone->_Rotate();
    if (!bone->callback_overwrite())
    {
        if (flags.is(st_BoneMotion::flWorldOrient))
        {
            M.setXYZi(r.x, r.y, r.z);
            M.c.set(bone->_Offset());
            L.mul(parent, M);
            L.i.set(M.i);
            L.j.set(M.j);
            L.k.set(M.k);

            Fmatrix LI;
            LI.invert(parent);
            M.mulA_43(LI);
        }
        else
        {
            M.setXYZi(r.x, r.y, r.z);
            M.c.set(bone->_Offset());
            L.mul(parent, M);
        }
    }
    if (bone->callback())
    {
        bone->callback()(bone);
        M.mul_43(Fmatrix().invert(parent), L);
        // bone->_Offset().set( M.c );
    }

    bone->_RenderTransform().mul_43(bone->_LTransform(), bone->_RITransform());
}

static void CalculateAnim(CBone* bone, CSMotion* motion, Fmatrix& parent)
{
    CalculateAnimBone(bone, motion, parent);
    // Calculate children
    for (auto& it : bone->children)
        CalculateAnim(it, motion, bone->_LTransform());
}

static void CalculateRest(CBone* bone, Fmatrix& parent)
{
    Fmatrix& R = bone->_RTransform();
    R.setXYZi(bone->_RestRotate());
    R.c.set(bone->_RestOffset());
    bone->_LRTransform() = R;
    R.mulA_43(parent);
    bone->_RITransform().invert(bone->_RTransform());

    // Calculate children
    for (auto& it : bone->children)
        CalculateRest(it, bone->_RTransform());
}

void CEditableObject::CalculateAnimation(CSMotion* motion)
{
    if (!m_Bones.empty())
        CalculateAnim(m_Bones.front(), motion, Fidentity);
}

float CEditableObject::GetBonesBottom()
{
    float bottom = FLT_MAX;
    VERIFY(!m_Bones.empty());
    for (auto b_it = m_Bones.begin() + 1; b_it != m_Bones.end(); b_it++)
        if (!(*b_it)->IsRoot() && (*b_it)->_LTransform().c.y < bottom)
            bottom = (*b_it)->_LTransform().c.y;
    return bottom;
}

static void SetBoneTransform(CBone& bone, const Fmatrix& T, const Fmatrix& parent)
{
    bone._LTransform() = T;
    Fmatrix LI;
    LI.invert(parent);
    bone._MTransform().mul_43(LI, T);
    bone._RenderTransform().mul_43(bone._LTransform(), bone._RITransform());
}

bool CEditableObject::AnimateRootObject(CSMotion* motion)
{
    VERIFY(motion);
    if (!motion->m_Flags.test(esmRootMover))
        return false;
    CBone& root_bone = *m_Bones[GetRootBoneID()];
    if (root_bone.children.size() != 1)
        return false;
    return true;
}

void CEditableObject::GetAnchorForRootObjectAnimation(Fmatrix& anchor)
{
    CBone& root_bone = *m_Bones[GetRootBoneID()];
    VERIFY(root_bone.children.size() == 1);
    CBone& anchor_bone = *root_bone.children[0];
    anchor = anchor_bone._LTransform();
    anchor.invert();
}

static void AlineYtoGlobalFrame(Fmatrix& in_out_m)
{
    Fmatrix& m = in_out_m;

    m.i.y = 0;
    m.k.y = 0;

    m.j.x = 0;
    m.j.y = 1;
    m.j.z = 0;

    float smi = m.i.x * m.i.x + m.i.z * m.i.z;
    float smk = m.k.x * m.k.x + m.k.z * m.k.z;
    bool bi = smi > EPS_S;
    bool bk = smk > EPS_S;
    if (smk > smi && bk)
    {
        m.k.mul(1.f / _sqrt(smk));
        m.i.crossproduct(m.j, m.k);
    }
    else if (bi)
    {
        m.i.mul(1.f / _sqrt(smi));
        m.k.crossproduct(m.i, m.j);
    }
    else // if( !bi && !bk )
    {
        // unreal indeed
        m = Fidentity;
    }

    //    if		 ( bi && bk )
    //   {
    //		m.i.mul( 1.f/_sqrt( smi ) );
    //       m.k.mul( 1.f/_sqrt( smk ) );
    //   }

    if (!check_scale(in_out_m))
        VERIFY(check_scale(in_out_m));
}

void CEditableObject::CalculateRootObjectAnimation(const Fmatrix& anchor)
{
    float bottom = GetBonesBottom();
    CBone& root_bone = *m_Bones[GetRootBoneID()];
    VERIFY(root_bone.children.size() == 1);
    CBone& anchor_bone = *root_bone.children[0];
    const Fmatrix gl_anchor = Fmatrix().mul_43(anchor_bone._LTransform(), anchor);

    Fmatrix root_transform = gl_anchor; // gl_anchor;
    AlineYtoGlobalFrame(root_transform);

    root_transform.c = gl_anchor.c;
    root_transform.c.y = bottom;
    SetBoneTransform(root_bone, root_transform, Fidentity);

    SetBoneTransform(anchor_bone, anchor_bone._LTransform(), root_transform);

    //  for (BoneIt b_it=root_bone .children.begin(); b_it!=root_bone .children.end(); b_it++)
    //  	SetBoneTransform(   *(*b_it), (*(*b_it))._LTransform(), root_transform );
}

void CEditableObject::CalculateBindPose()
{
    if (!m_Bones.empty())
        CalculateRest(m_Bones.front(), Fidentity);
}

void CEditableObject::SetActiveSMotion(CSMotion* mot)
{
    m_ActiveSMotion = mot;
    if (m_ActiveSMotion)
        m_SMParam.Set(m_ActiveSMotion);
}

bool CEditableObject::RemoveSMotion(const char* name)
{
    SMotionVec& lst = m_SMotions;
    for (SMotionIt m = lst.begin(); m != lst.end(); ++m)
        if ((xr_stricmp((*m)->Name(), name) == 0))
        {
            if (m_ActiveSMotion == *m)
                SetActiveSMotion(0);
            xr_delete(*m);
            lst.erase(m);
            return true;
        }
    return false;
}

//---------------------------------------------------------------------------
/*
bool CEditableObject::LoadSMotions(const char* fname)
{
    IReader* F	= FS.r_open(fname);
    ClearSMotions();
    // object motions
    m_SMotions.resize(F->r_u32());
    SetActiveSMotion(0);
    for (SMotionIt m_it=m_SMotions.begin(); m_it!=m_SMotions.end(); m_it++){
        *m_it = new CSMotion();
        if (!(*m_it)->Load(*F)){
            ELog.DlgMsg(mtError,"Motions has different version. Load failed.");
            xr_delete(*m_it);
            m_SMotions.clear();
            FS.r_close(F);
            return false;
        }
          if (!CheckBoneCompliance(*m_it)){
            ClearSMotions();
            ELog.DlgMsg(mtError,"Load failed.",fname);
            xr_delete(&*m_it);
            FS.r_close(F);
            return false;
        }
    }
    FS.r_close(F);
    return true;
}
*/
bool CEditableObject::AppendSMotion(LPCSTR fname, SMotionVec* inserted)
{
    VERIFY(IsSkeleton());

    bool bRes = true;

    LPCSTR ext = strext(fname);
    if (0 == xr_stricmp(ext, ".skl"))
    {
        CSMotion* M = new CSMotion();
        if (!M->LoadMotion(fname))
        {
            //  ELog.Msg(mtError, "Motion '%s' can't load. Append failed.", fname); Lord: Interpret
            xr_delete(M);
            bRes = false;
        }
        else
        {
            string256 name;
            _splitpath(fname, 0, 0, name, 0);
            if (CheckBoneCompliance(M))
            {
                M->SortBonesBySkeleton(m_Bones);
                string256 m_name;
                GenerateSMotionName(m_name, name, M);
                M->SetName(m_name);
                m_SMotions.push_back(M);
                if (inserted)
                    inserted->push_back(M);
                // optimize
                M->Optimize();
            }
            else
            {
                //   ELog.Msg(mtError, "Append failed.", fname); Lord: Interpret
                xr_delete(M);
                bRes = false;
            }
        }
    }
    else if (0 == xr_stricmp(ext, ".skls"))
    {
        IReader* F = FS.r_open(fname);
        if (!F)
        {
            //   ELog.Msg(mtError, "Can't open file '%s'.", fname); Lord: Interpret
            bRes = false;
        }
        if (bRes)
        {
            // object motions
            int cnt = F->r_u32();
            for (int k = 0; k < cnt; k++)
            {
                CSMotion* M = new CSMotion();
                if (!M->Load(*F))
                {
                    //      ELog.Msg(mtError, "Motion '%s' has different version. Load failed.", M->Name()); Lord:
                    //      Interpret
                    xr_delete(M);
                    bRes = false;
                    break;
                }
                if (!CheckBoneCompliance(M))
                {
                    xr_delete(M);
                    bRes = false;
                    break;
                }
                if (bRes)
                {
                    M->SortBonesBySkeleton(m_Bones);
                    string256 m_name;
                    GenerateSMotionName(m_name, M->Name(), M);
                    M->SetName(m_name);
                    m_SMotions.push_back(M);
                    if (inserted)
                        inserted->push_back(M);
                    // optimize
                    M->Optimize();
                }
            }
        }
        FS.r_close(F);
    }
    return bRes;
}

void CEditableObject::ClearSMotions()
{
    SetActiveSMotion(0);
    for (SMotionIt m_it = m_SMotions.begin(); m_it != m_SMotions.end(); ++m_it)
        xr_delete(*m_it);
    m_SMotions.clear();
}

bool CEditableObject::SaveSMotions(const char* fname)
{
    CMemoryWriter F;
    F.w_u32(m_SMotions.size());
    for (SMotionIt m_it = m_SMotions.begin(); m_it != m_SMotions.end(); ++m_it)
        (*m_it)->Save(F);
    return F.save_to(fname);
}

bool CEditableObject::RenameSMotion(const char* old_name, const char* new_name)
{
    if (xr_stricmp(old_name, new_name) == 0)
        return true;
    if (FindSMotionByName(new_name))
        return false;
    CSMotion* M = FindSMotionByName(old_name);
    VERIFY(M);
    M->SetName(new_name);
    return true;
}

CSMotion* CEditableObject::FindSMotionByName(const char* name, const CSMotion* Ignore)
{
    if (name && name[0])
    {
        SMotionVec& lst = m_SMotions;
        for (SMotionIt m = lst.begin(); m != lst.end(); ++m)
            if ((Ignore != (*m)) && (xr_stricmp((*m)->Name(), name) == 0))
                return (*m);
    }
    return 0;
}

void CEditableObject::GenerateSMotionName(char* buffer, const char* start_name, const CSMotion* M)
{
    strcpy(buffer, start_name);
    int idx = 0;
    while (FindSMotionByName(buffer, M))
    {
        sprintf(buffer, "%s_%2d", start_name, idx);
        idx++;
    }
    xr_strlwr(buffer);
}

ICF

    bool
    pred_sort_B(CBone* A, CBone* B)
{
    return (xr_strcmp(A->Name().c_str(), B->Name().c_str()) < 0);
}

ICF

    void
    fill_bones_by_parent(BoneVec& bones, CBone* start)
{
    bones.push_back(start);
    for (auto b_it = start->children.begin(); b_it != start->children.end(); b_it++)
        fill_bones_by_parent(bones, *b_it);
}

void CEditableObject::PrepareBones()
{
    if (m_Bones.empty())
        return;
    CBone* PARENT = nullptr;
    // clear empty parent
    for (auto b_it = m_Bones.begin(); b_it != m_Bones.end(); b_it++)
    {
        (*b_it)->children.clear();
        (*b_it)->parent = NULL;
        const auto parent = std::find_if(m_Bones.begin(), m_Bones.end(), fBoneNameEQ((*b_it)->ParentName()));
        if (parent == m_Bones.end())
        {
            (*b_it)->SetParentName("");
            VERIFY2(nullptr == PARENT, "Invalid object. Have more than 1 parent.");
            PARENT = *b_it;
        }
        else
        {
            auto parent = std::find_if(m_Bones.begin(), m_Bones.end(), fBoneNameEQ((*b_it)->ParentName()));
            CBone* tmp = (parent == m_Bones.end()) ? 0 : *parent;
            (*b_it)->parent = tmp;
        }
    }
    // sort by name
    std::sort(m_Bones.begin(), m_Bones.end(), pred_sort_B);
    // fill children
    for (auto b_it = m_Bones.begin(); b_it != m_Bones.end(); b_it++)
    {
        auto parent = std::find_if(m_Bones.begin(), m_Bones.end(), fBoneNameEQ((*b_it)->ParentName()));
        if (parent != m_Bones.end())
            (*parent)->children.push_back(*b_it);
    }
    // manual sort
    u32 b_cnt = m_Bones.size();
    m_Bones.clear();
    fill_bones_by_parent(m_Bones, PARENT);

    u32 cnt_new = m_Bones.size();
    VERIFY(b_cnt == cnt_new);
    // update SelfID
    for (auto b_it = m_Bones.begin(); b_it != m_Bones.end(); b_it++)
        (*b_it)->SelfID = b_it - m_Bones.begin();
    VERIFY(0 == m_Bones.front()->parent);
    /*
        for (b_it=m_Bones.begin(); b_it!=m_Bones.end(); b_it++)
            Msg("%20s - %20s",(*b_it)->Name().c_str(),(*b_it)->ParentName().c_str());
    */
    CalculateBindPose();
}

auto CEditableObject::FindBoneByNameIt(const char* name)
{
    return std::find_if(m_Bones.begin(), m_Bones.end(), fBoneNameEQ(name));
}

int CEditableObject::FindBoneByNameIdx(LPCSTR name)
{
    auto b_it = FindBoneByNameIt(name);
    return (b_it == m_Bones.end()) ? -1 : b_it - m_Bones.begin();
}

CBone* CEditableObject::FindBoneByName(const char* name)
{
    auto b_it = FindBoneByNameIt(name);
    return (b_it == m_Bones.end()) ? 0 : *b_it;
}

int CEditableObject::GetRootBoneID()
{
    for (auto b_it = m_Bones.begin(); b_it != m_Bones.end(); ++b_it)
        if ((*b_it)->IsRoot())
            return b_it - m_Bones.begin();
    // THROW;
    ASSERT(TEXT(""));
    return -1;
}

int CEditableObject::PartIDByName(LPCSTR name)
{
    for (auto it = m_BoneParts.begin(); it != m_BoneParts.end(); ++it)
        if (it->alias == name)
            return it - m_BoneParts.begin();
    return -1;
}

shared_str CEditableObject::BoneNameByID(int id)
{
    VERIFY((id >= 0) && (id < (int)m_Bones.size()));
    return m_Bones[id]->Name();
}

u16 CEditableObject::GetBoneIndexByWMap(const char* wm_name)
{
    auto bone = std::find_if(m_Bones.begin(), m_Bones.end(), fBoneWMNameEQ(wm_name));
    return (bone == m_Bones.end()) ? BI_NONE : (u16)(bone - m_Bones.begin());
}

void CEditableObject::GetBoneWorldTransform(u32 bone_idx, float t, CSMotion* motion, Fmatrix& matrix)
{
    VERIFY(bone_idx < m_Bones.size());
    int idx = bone_idx;
    matrix.identity();
    IntVec lst;
    do
    {
        lst.push_back(idx);
    } while ((idx = m_Bones[idx]->Parent() ? m_Bones[idx]->Parent()->SelfID : -1) > -1);
    for (int i = lst.size() - 1; i >= 0; i--)
    {
        idx = lst[i];
        Flags8 flags = motion->GetMotionFlags(idx);
        Fvector T, R;
        Fmatrix rot, mat;
        motion->_Evaluate(idx, t, T, R);
        if (flags.is(st_BoneMotion::flWorldOrient))
        {
            rot.setXYZi(R.x, R.y, R.z);
            mat.identity();
            mat.c.set(T);
            mat.mulA_43(matrix);
            mat.i.set(rot.i);
            mat.j.set(rot.j);
            mat.k.set(rot.k);
        }
        else
        {
            mat.setXYZi(R.x, R.y, R.z);
            mat.c.set(T);
            mat.mulA_43(matrix);
        }
        matrix.set(mat);
    }
}

bool CEditableObject::CheckBoneCompliance(CSMotion* M)
{
    VERIFY(M);
    /*
        BoneMotionVec& lst = M->BoneMotions();
        if (m_Bones.size()!=lst.size()){
            Log		("!Different bone count.\nObject has '%d' bones. Motion has '%d' bones.",m_Bones.size(),lst.size());
            return false;
        }
        for(BoneMotionIt bm_it=lst.begin(); bm_it!=lst.end(); bm_it++)
            if (!FindBoneByName(*bm_it->name)){
                Msg		("!Can't find bone '%s' in object.",bm_it->name);
                return false;
            }
    */
    for (auto b_it = m_Bones.begin(); b_it != m_Bones.end(); ++b_it)
        if (!M->FindBoneMotion((*b_it)->Name()))
        {
            //        	Msg		("!Can't find bone '%s' in motion.",*(*b_it)->Name());
            //        	return false;
            M->add_empty_motion((*b_it)->Name());
            continue;
        }
    return true;
}

void CEditableObject::OptimizeSMotions()
{
    // Lord: Interpret
    // SPBItem* pb = UI->ProgressStart(m_SMotions.size(), "Motions optimizing...");

    for (auto s_it = m_SMotions.begin(); s_it != m_SMotions.end(); ++s_it)
    {
        (*s_it)->Optimize();

        //   pb->Inc();
    }

    //  UI->ProgressEnd(pb);
}

#pragma endregion

#pragma region EditObjectProps
// Lord: Interpret функционал подсматривать
#pragma endregion

#pragma region EditObjectSkin

const u32 color_bone_sel_color = 0xFFFFFFFF;
const u32 color_bone_norm_color = 0xFFFFFF00;
const u32 color_bone_link_color = 0xFFA0A000;
const u32 color_bone_sel_cm = 0xFFFF0000;
const u32 color_bone_norm_cm = 0xFF700000;
const float joint_size = 0.025f;

void CEditableObject::ResetBones()
{
    for (auto& it : m_Bones)
        it->ResetData();
}
/*

class fBoneNameEQ
{
    shared_str name;

public:
    fBoneNameEQ(shared_str N) : name(N){};

    bool operator()(CBone* B) { return xr_strcmp(B->Name(), name) == 0; }
}; */

bool CEditableObject::LoadBoneData(IReader& F)
{
    BoneVec load_bones;
    int count = 0;
    IReader* R;
    while (nullptr != (R = F.open_chunk(count++)))
    {
        CBone* nBone = new CBone();
        load_bones.push_back(nBone);
        nBone->LoadData(*R);
        Msg("loaded bone [%s]", nBone->Name().c_str());
    }
    bool bRes = true;
    // load bones
    if (!load_bones.empty())
    {
        for (auto& bone : m_Bones)
        {
            const auto n_it = std::find_if(load_bones.begin(), load_bones.end(), fBoneNameEQ(bone->Name()));
            if (n_it != load_bones.end())
            {
                bone->CopyData(*n_it);
            }
            else
            {
                // ELog.Msg(mtError, "Can't find bone: '%s'.", bone->Name().c_str()); Lord: Interpret
                bRes = false;
                //                break;
            }
        }

        for (auto& it : load_bones)
            xr_delete(it);

        load_bones.clear();
    }
    else
    {
        //  ELog.Msg(mtError, "Empty bone list."); Lord: Interpret
        bRes = false;
    }
    // load bone part
    if (F.find_chunk(EOBJ_CHUNK_BONEPARTS2))
    {
        shared_str buf;
        m_BoneParts.resize(F.r_u32());
        for (auto& bp_it : m_BoneParts)
        {
            F.r_stringZ(buf);
            bp_it.alias = buf;
            bp_it.bones.resize(F.r_u32());
            for (auto& s_it : bp_it.bones)
                F.r_stringZ(s_it);
        }
        if (!m_BoneParts.empty() && !VerifyBoneParts())
        {
            //    ELog.Msg(mtError, "Invalid bone parts. Found missing or duplicate bone."); Lord: Interpret
        }
    }
    else
    {
        // ELog.Msg(mtError, "Can't load bone parts. Invalid version."); Lord: Interpret
    }
    return bRes;
}

void CEditableObject::SaveBoneData(IWriter& F)
{
    for (auto b_it = m_Bones.begin(); b_it != m_Bones.end(); ++b_it)
    {
        F.open_chunk(b_it - m_Bones.begin());
        (*b_it)->SaveData(F);
        F.close_chunk();
    }
    // save bone part
    F.open_chunk(EOBJ_CHUNK_BONEPARTS2);
    F.w_u32(m_BoneParts.size());
    for (auto& bp_it : m_BoneParts)
    {
        F.w_stringZ(bp_it.alias.c_str());
        F.w_u32(bp_it.bones.size());
        for (auto& s_it : bp_it.bones)
            F.w_stringZ(s_it);
    }
    F.close_chunk();
}

void CEditableObject::RenderSkeletonSingle(const Fmatrix& parent)
{
    RenderSingle(parent);
    RenderBones(parent);
}

void CEditableObject::RenderBones(const Fmatrix& parent)
{
    if (IsSkeleton())
    {
        // render
        for (auto& it : m_Bones)
        {
            RCache.set_Shader(RImplementation.m_WireShader);
            RCache.set_xform_world(parent);
            Fmatrix& M = it->_LTransform();
            Fvector p1 = M.c;
            u32 c_joint = it->flags.is(CBone::flSelected) ? color_bone_sel_color : color_bone_norm_color;
            //  if (EPrefs->object_flags.is(epoDrawJoints)) Lord: Interpret добавить EPrefs сюда
            DUImpl.DrawJoint(p1, joint_size, c_joint);
            // center of mass
            if (it->shape.type != SBoneShape::stNone)
            {
                Fvector cm;
                M.transform_tiny(cm, it->center_of_mass);
                if (it->flags.is(CBone::flSelected))
                {
                    float sz = joint_size * 2.f;
                    DUImpl.DrawCross(cm, sz, sz, sz, sz, sz, sz, 0xFFFFFFFF, false);
                    DUImpl.DrawRomboid(cm, joint_size * 0.7f, color_bone_sel_cm);
                }
                else
                {
                    DUImpl.DrawRomboid(cm, joint_size * 0.7f, color_bone_norm_cm);
                }
            }
            /*
                        if (0){
                            M.transform_dir	(d);
                            p2.mad			(p1,d,it->_Length());
                            DU.DrawLine		(p1,p2,c_joint);
                        }
            */
            if (it->Parent())
            {
                RCache.set_Shader(RImplementation.m_SelectionShader);
                Fvector& p2 = it->Parent()->_LTransform().c;
                DUImpl.DrawLine(p1, p2, color_bone_link_color);
            }
            //     if (EPrefs->object_flags.is(epoDrawBoneAxis)) Lord: Interpret
            {
                Fmatrix mat;
                mat.mul(parent, M);
                DUImpl.DrawObjectAxis(mat, 0.03f, it->flags.is(CBone::flSelected));
            }
            //   if (EPrefs->object_flags.is(epoDrawBoneNames)) Lord: Interpret
            {
                parent.transform_tiny(p1);
                u32 c = it->flags.is(CBone::flSelected) ? 0xFFFFFFFF : 0xFF000000;
                u32 s = it->flags.is(CBone::flSelected) ? 0xFF000000 : 0xFF909090;
                DUImpl.OutText(p1, it->Name().c_str(), c, s);
            }
            //  if (EPrefs->object_flags.is(epoDrawBoneShapes)) Lord: Interpret
            {
                RCache.set_Shader(RImplementation.m_SelectionShader);
                Fmatrix mat = M;
                mat.mulA_43(parent);
                u32 c = it->flags.is(CBone::flSelected) ? 0x80ffffff : 0x300000ff;
                if (it->shape.Valid())
                {
                    switch (it->shape.type)
                    {
                    case SBoneShape::stBox: DUImpl.DrawOBB(mat, it->shape.box, c, c); break;
                    case SBoneShape::stSphere: DUImpl.DrawSphere(mat, it->shape.sphere, c, c, TRUE, TRUE); break;
                    case SBoneShape::stCylinder:
                        DUImpl.DrawCylinder(mat, it->shape.cylinder.m_center, it->shape.cylinder.m_direction,
                            it->shape.cylinder.m_height, it->shape.cylinder.m_radius, c, c, TRUE, TRUE);
                        break;
                    }
                }
            }
        }
    }
}

CBone* CEditableObject::PickBone(const Fvector& S, const Fvector& D, const Fmatrix& parent)
{
    float dist = 10000.f;
    CBone* sel = 0;
    for (auto& it : m_Bones)
        if (it->Pick(dist, S, D, parent))
            sel = it;
    return sel;
}

void CEditableObject::SelectBones(bool bVal)
{
    for (auto& it : m_Bones)
        it->Select(bVal);
}

void CEditableObject::SelectBone(CBone* b, bool bVal)
{
    if (b)
        b->Select(bVal);
}

int CEditableObject::GetSelectedBones(BoneVec& sel_bones)
{
    for (auto& it : m_Bones)
        if (it->flags.is(CBone::flSelected))
            sel_bones.push_back(it);
    return sel_bones.size();
}

//----------------------------------------------------
#include "FreeMagic/Source/Containment3D/MgcCont3DMinSphere.h"
#include "FreeMagic/Source/Core3D/MgcSphere.h"
#include "ExportSkeleton.h"

BOOL f_valid(float f) { return _finite(f) && !_isnan(f); }
BOOL SphereValid(FvectorVec& geom, Fsphere& test)
{
    if (!f_valid(test.P.x) || !f_valid(test.R))
        Msg("*** Attention ***: invalid sphere: %f,%f,%f - %f", test.P.x, test.P.y, test.P.z, test.R);

    Fsphere S = test;
    S.R += EPS_L;
    for (auto& it : geom)
        if (!S.contains(it))
            return FALSE;
    return TRUE;
}

void ComputeSphere(Fsphere& B, FvectorVec& V)
{
    if (V.size() < 3)
    {
        B.P.set(0, 0, 0);
        B.R = 0.f;
        return;
    }

    // 1: calc first variation
    Fsphere S1;
    Fsphere_compute(S1, &*V.cbegin(), V.size());
    BOOL B1 = SphereValid(V, S1);

    // 2: calc ordinary algorithm (2nd)
    Fsphere S2;
    Fbox bbox;
    bbox.invalidate();
    for (auto I = V.begin(); I != V.end(); ++I)
        bbox.modify(*I);
    bbox.grow(EPS_L);
    bbox.getsphere(S2.P, S2.R);
    S2.R = -1;
    for (auto& it : V)
    {
        const float d = S2.P.distance_to_sqr(it);
        if (d > S2.R)
            S2.R = d;
    }
    S2.R = _sqrt(_abs(S2.R));
    BOOL B2 = SphereValid(V, S2);

    // 3: calc magic-fm
    Mgc::Sphere _S3 = Mgc::MinSphere(V.size(), (const Mgc::Vector3*) &*V.begin());
    Fsphere S3;
    S3.P.set(_S3.Center().x, _S3.Center().y, _S3.Center().z);
    S3.R = _S3.Radius();
    BOOL B3 = SphereValid(V, S3);

    // select best one
    if (B1 && (S1.R < S2.R))
    { // miniball or FM
        if (B3 && (S3.R < S1.R))
        { // FM wins
            B.set(S3);
        }
        else
        { // MiniBall wins
            B.set(S1);
        }
    }
    else
    { // base or FM
        if (B3 && (S3.R < S2.R))
        { // FM wins
            B.set(S3);
        }
        else
        { // Base wins :)
            R_ASSERT(B2);
            B.set(S2);
        }
    }
}

//----------------------------------------------------
#include "FreeMagic/Source/Containment3D/MgcCont3DCylinder.h"

void ComputeCylinder(Fcylinder& C, Fobb& B, FvectorVec& V)
{
    if (V.size() < 3)
    {
        C.invalidate();
        return;
    }
    // pow(area,(3/2))/volume
    // 2*Pi*R*H+2*Pi*R*R

    //	Fvector axis;
    float max_hI = flt_min;
    float min_hI = flt_max;
    float max_rI = flt_min;
    float max_hJ = flt_min;
    float min_hJ = flt_max;
    float max_rJ = flt_min;
    float max_hK = flt_min;
    float min_hK = flt_max;
    float max_rK = flt_min;
    Fvector axisJ = B.m_rotate.j;
    Fvector axisI = B.m_rotate.i;
    Fvector axisK = B.m_rotate.k;
    Fvector& c = B.m_translate;
    for (FvectorIt I = V.begin(); I != V.end(); ++I)
    {
        Fvector tmp;
        Fvector pt = *I;
        Fvector pt_c;
        pt_c.sub(pt, c);

        float pI = axisI.dotproduct(pt);
        min_hI = _min(min_hI, pI);
        max_hI = _max(max_hI, pI);
        tmp.mad(c, axisI, axisI.dotproduct(pt_c));
        max_rI = _max(max_rI, tmp.distance_to(pt));

        float pJ = axisJ.dotproduct(pt);
        min_hJ = _min(min_hJ, pJ);
        max_hJ = _max(max_hJ, pJ);
        tmp.mad(c, axisJ, axisJ.dotproduct(pt_c));
        max_rJ = _max(max_rJ, tmp.distance_to(pt));

        float pK = axisK.dotproduct(pt);
        min_hK = _min(min_hK, pK);
        max_hK = _max(max_hK, pK);
        tmp.mad(c, axisK, axisK.dotproduct(pt_c));
        max_rK = _max(max_rK, tmp.distance_to(pt));
    }

    float hI = (max_hI - min_hI);
    float hJ = (max_hJ - min_hJ);
    float hK = (max_hK - min_hK);
    float vI = hI * M_PI * _sqr(max_rI);
    float vJ = hJ * M_PI * _sqr(max_rJ);
    float vK = hK * M_PI * _sqr(max_rK);
    //    vI					= pow(2*M_PI*max_rI*hI+2*M_PI*_sqr(max_rI),3/2)/vI;
    //    vJ					= pow(2*M_PI*max_rJ*hJ+2*M_PI*_sqr(max_rJ),3/2)/vJ;
    //    vK					= pow(2*M_PI*max_rK*hK+2*M_PI*_sqr(max_rK),3/2)/vK;
    // pow(area,(3/2))/volume
    // 2*Pi*R*H+2*Pi*R*R

    if (vI < vJ)
    {
        if (vI < vK)
        {
            // vI;
            C.m_direction.set(axisI);
            C.m_height = hI;
            C.m_radius = max_rI;
        }
        else
        {
            // vK
            C.m_direction.set(axisK);
            C.m_height = hK;
            C.m_radius = max_rK;
        }
    }
    else
    {
        // vJ < vI
        if (vJ < vK)
        {
            // vJ
            C.m_direction.set(axisJ);
            C.m_height = hJ;
            C.m_radius = max_rJ;
        }
        else
        {
            // vK
            C.m_direction.set(axisK);
            C.m_height = hK;
            C.m_radius = max_rK;
        }
    }

    C.m_center.set(B.m_translate);
    /*
        if (V.size()<3) { B.invalidate(); return; }
        Mgc::Cylinder CYL	= Mgc::ContCylinder(V.size(), (const Mgc::Vector3*) V.begin());
        B.m_center.set		(CYL.Center());
        B.m_direction.set	(CYL.Direction());
        B.m_height			= CYL.Height();
        B.m_radius			= CYL.Radius();
    */
}

bool CEditableObject::GenerateBoneShape(bool bSelOnly)
{
    R_ASSERT(IsSkeleton());
    xr_vector<FvectorVec> bone_points;
    bone_points.resize(m_Bones.size());
    for (EditMeshIt mesh_it = FirstMesh(); mesh_it != LastMesh(); ++mesh_it)
    {
        CEditableMesh* MESH = *mesh_it;
        // generate vertex offset
        MESH->GenerateSVertices(1);
        for (u32 f_id = 0; f_id != MESH->GetFCount(); f_id++)
        {
            for (int k = 0; k < 3; k++)
            {
                st_SVert& sv = MESH->m_SVertices[f_id * 3 + k];
                VERIFY(sv.bones.size() == 1);
                u16 b_id =
                    sv.bones[0]
                        .id; //(sv.bones.size()>1)?(sv.bones[0].w>sv.bones[1].w?sv.bones[0].id:sv.bones[1].id):sv.bones[0].id;
                FvectorVec& P = bone_points[b_id];
                bool bFound = false;
                Fvector p;
                m_Bones[b_id]->_RITransform().transform_tiny(p, sv.offs);
                for (FvectorIt p_it = P.begin(); p_it != P.end(); ++p_it)
                    if (p_it->similar(p))
                    {
                        bFound = true;
                        break;
                    }
                if (!bFound)
                    P.push_back(p);
                //		        if (sv.bone1!=BI_NONE) bone_points[sv.bone1].push_back(sv.offs1);
            }
        }
        MESH->UnloadSVertices();
    }

    BoneVec& lst = m_Bones;
    for (auto b_it = lst.begin(); b_it != lst.end(); ++b_it)
    {
        if (bSelOnly && !(*b_it)->flags.is(CBone::flSelected))
            continue;
        FvectorVec& positions = bone_points[b_it - lst.begin()];
        ComputeOBB_WML((*b_it)->shape.box, positions);
        ComputeSphere((*b_it)->shape.sphere, positions);
        ComputeCylinder((*b_it)->shape.cylinder, (*b_it)->shape.box, positions);
        (*b_it)->center_of_mass.set((*b_it)->shape.sphere.P);
    }
    return true;
}

void CEditableObject::ClampByLimits(bool bSelOnly)
{
    for (auto& it : m_Bones)
        if (!bSelOnly || bSelOnly && it->Selected())
            it->ClampByLimits();
}

#pragma endregion

#pragma region EditObjectIO
#include "exportskeleton.h"
#include "exportobjectOGF.h"
#include "Layers/xrRender/Shader.h"

bool CEditableObject::Load(const char* fname)
{
  //  if (FS.exist(fname))
  //  {
        int age = FS.get_file_age(fname);
        VERIFY3(age > 0, "Invalid file age:", fname);
        IReader* F = FS.r_open(fname);
        R_ASSERT(F);
        IReader* OBJ = F->open_chunk(EOBJ_CHUNK_OBJECT_BODY);
        R_ASSERT2(OBJ, "Corrupted file.");
        bool bRes = Load(*OBJ);
        OBJ->close();
        FS.r_close(F);
        if (bRes)
        {
            m_LoadName = fname;
            m_ObjectVersion = age;
        }
        return bRes;
  //  }
    return false;
}

bool CEditableObject::Save(const char* fname)
{
    if (IsModified())
    {
        // update transform matrix
        Fmatrix mTransform, mScale, mTranslate, mRotate;
        if (!fsimilar(t_vRotate.magnitude(), 0) || !fsimilar(t_vScale.magnitude(), 1.73205f) ||
            !fsimilar(t_vPosition.magnitude(), 0))
        {
            mRotate.setHPB(t_vRotate.y, t_vRotate.x, t_vRotate.z);
            mScale.scale(t_vScale);
            mTranslate.translate(t_vPosition);
            mTransform.mul(mTranslate, mRotate);
            mTransform.mulB_43(mScale);
            TranslateToWorld(mTransform);
            t_vRotate.set(0, 0, 0);
            t_vPosition.set(0, 0, 0);
            t_vScale.set(1, 1, 1);
        }
    }

    // save object
    IWriter* F = FS.w_open(fname);
    if (F)
    {
        F->open_chunk(EOBJ_CHUNK_OBJECT_BODY);
        Save(*F);
        F->close_chunk();

        FS.w_close(F);

        m_LoadName = fname;
        m_ObjectVersion = (int)FS.get_file_age(fname);
        VERIFY3(m_ObjectVersion > 0, "Invalid file age:", fname);
        return true;
    }
    else
    {
        return false;
    }
}

void CEditableObject::Save(IWriter& F)
{
    F.open_chunk(EOBJ_CHUNK_VERSION);
    F.w_u16(EOBJ_CURRENT_VERSION);
    F.close_chunk();

    F.open_chunk(EOBJ_CHUNK_CLASSSCRIPT);
    F.w_stringZ(m_ClassScript);
    F.close_chunk();

    F.open_chunk(EOBJ_CHUNK_LODS);
    F.w_stringZ(m_LODs);
    F.close_chunk();

    F.w_chunk(EOBJ_CHUNK_FLAGS, &m_objectFlags.flags, sizeof(m_objectFlags.flags));

    // meshes
    F.open_chunk(EOBJ_CHUNK_EDITMESHES);
    int count = 0;

    for (EditMeshIt m = m_Meshes.begin(); m != m_Meshes.end(); ++m)
    {
        F.open_chunk(count);
        ++count;
        (*m)->SaveMesh(F);
        F.close_chunk();
    }
    F.close_chunk();

    // surfaces
    F.open_chunk(EOBJ_CHUNK_SURFACES3);
    F.w_u32(m_Surfaces.size());

    for (auto& it : m_Surfaces)
    {
        F.w_stringZ(it->_Name());
        F.w_stringZ(it->_ShaderName());
        F.w_stringZ(it->_ShaderXRLCName());
        F.w_stringZ(it->_GameMtlName());
        F.w_stringZ(it->_Texture());
        F.w_stringZ(it->_VMap());
        F.w_u32(it->m_Flags.get());
        F.w_u32(it->_FVF());
        F.w_u32(1);
    }
    F.close_chunk();

    // bones
    if (!m_Bones.empty())
    {
        F.open_chunk(EOBJ_CHUNK_BONES2);
        for (auto b_it = m_Bones.begin(); b_it != m_Bones.end(); ++b_it)
        {
            F.open_chunk(b_it - m_Bones.begin());
            (*b_it)->Save(F);
            F.close_chunk();
        }
        F.close_chunk();
    }

    // skeleton motions
    if (!m_SMotions.empty())
    {
        F.open_chunk(EOBJ_CHUNK_SMOTIONS);
        F.w_u32(m_SMotions.size());

        for (SMotionIt s_it = m_SMotions.begin(); s_it != m_SMotions.end(); ++s_it)
            (*s_it)->Save(F);

        F.close_chunk();
    }
    // skeleton motions refs
    if (m_SMotionRefs.size())
    {
        F.open_chunk(EOBJ_CHUNK_SMOTIONS3);
        F.w_u32(m_SMotionRefs.size());

        for (u32 i = 0; i < m_SMotionRefs.size(); ++i)
            F.w_stringZ(m_SMotionRefs[i].c_str());

        F.close_chunk();
    }

    // bone parts
    if (!m_BoneParts.empty())
    {
        F.open_chunk(EOBJ_CHUNK_BONEPARTS2);
        F.w_u32(m_BoneParts.size());
        for (BPIt bp_it = m_BoneParts.begin(); bp_it != m_BoneParts.end(); ++bp_it)
        {
            F.w_stringZ(bp_it->alias);
            F.w_u32(bp_it->bones.size());

            for (auto& s_it : bp_it->bones)
                F.w_stringZ(s_it.c_str());
        }
        F.close_chunk();
    }

    if (IsDynamic())
    {
        F.open_chunk(EOBJ_CHUNK_ACTORTRANSFORM);
        F.w_fvector3(a_vPosition);
        F.w_fvector3(a_vRotate);
        F.close_chunk();
    }

    F.open_chunk(EOBJ_CHUNK_DESC);
    F.w_stringZ(m_CreateName.c_str());
    F.w(&m_CreateTime, sizeof(m_CreateTime));
    F.w_stringZ(m_ModifName.c_str());
    F.w(&m_ModifTime, sizeof(m_ModifTime));
    F.close_chunk();

    // set modif desc
    SetVersionToCurrent(FALSE, TRUE);

    bOnModified = false;
}

bool CEditableObject::Load(IReader& F)
{
    bool bRes = true;
    do
    {
        u32 version = 0;
        shared_str buf;
        shared_str sh_name;
        R_ASSERT(F.r_chunk(EOBJ_CHUNK_VERSION, &version));
        if (version != EOBJ_CURRENT_VERSION)
        {
            //  ELog.DlgMsg(mtError, "CEditableObject: unsupported file version. Object can't load."); Lord: Interpret
            bRes = false;
            break;
        }

        R_ASSERT(F.r_chunk(EOBJ_CHUNK_FLAGS, &m_objectFlags.flags));

        if (F.find_chunk(EOBJ_CHUNK_CLASSSCRIPT))
            F.r_stringZ(m_ClassScript);

        if (F.find_chunk(EOBJ_CHUNK_LODS))
            F.r_stringZ(m_LODs);

        // surfaces
        if (F.find_chunk(EOBJ_CHUNK_SURFACES3))
        {
            u32 cnt = F.r_u32();
            m_Surfaces.resize(cnt);
            for (SurfaceIt s_it = m_Surfaces.begin(); s_it != m_Surfaces.end(); ++s_it)
            {
                *s_it = new CSurface();
                F.r_stringZ(buf);
                (*s_it)->SetName(buf.c_str());
                F.r_stringZ(buf);
                (*s_it)->SetShader(buf.c_str());
                F.r_stringZ(buf);
                (*s_it)->SetShaderXRLC(buf.c_str());
                F.r_stringZ(buf);
                (*s_it)->SetGameMtl(buf.c_str());
                F.r_stringZ(buf);
                (*s_it)->SetTexture(buf.c_str());
                F.r_stringZ(buf);
                (*s_it)->SetVMap(buf.c_str());
                (*s_it)->m_Flags.assign(F.r_u32());
                (*s_it)->SetFVF(F.r_u32());
                cnt = F.r_u32();
                if (cnt > 1)
                {
                    // ELog.DlgMsg(mtError, "Object surface '%s' has more than one TC's.", buf.c_str()); Lord: Interpret
                }

                R_ASSERT(1 <= cnt);
            }
        }
        else if (F.find_chunk(EOBJ_CHUNK_SURFACES2))
        {
            u32 cnt = F.r_u32();
            m_Surfaces.resize(cnt);
            for (SurfaceIt s_it = m_Surfaces.begin(); s_it != m_Surfaces.end(); ++s_it)
            {
                *s_it = new CSurface();
                F.r_stringZ(buf);
                (*s_it)->SetName(buf.c_str());
                F.r_stringZ(buf);
                (*s_it)->SetShader(buf.c_str());
                F.r_stringZ(buf);
                (*s_it)->SetShaderXRLC(buf.c_str());
                F.r_stringZ(buf);
                (*s_it)->SetTexture(buf.c_str());
                F.r_stringZ(buf);
                (*s_it)->SetVMap(buf.c_str());
                (*s_it)->m_Flags.assign(F.r_u32());
                (*s_it)->SetFVF(F.r_u32());
                cnt = F.r_u32();
                if (cnt > 1)
                {
                    // ELog.DlgMsg(mtError, "Object surface '%s' has more than one TC's.", buf.c_str()); Lord: Interpret
                }

                R_ASSERT(1 <= cnt);
            }
        }
        else
        {
            R_ASSERT(F.find_chunk(EOBJ_CHUNK_SURFACES));
            u32 cnt = F.r_u32();
            m_Surfaces.resize(cnt);
            for (SurfaceIt s_it = m_Surfaces.begin(); s_it != m_Surfaces.end(); ++s_it)
            {
                *s_it = new CSurface();
                F.r_stringZ(buf);
                (*s_it)->SetName(buf.c_str());
                F.r_stringZ(sh_name);
                (*s_it)->m_Flags.set(CSurface::sf2Sided, !!F.r_u8());
                (*s_it)->SetFVF(F.r_u32());
                cnt = F.r_u32();
                if (cnt > 1)
                {
                    // ELog.DlgMsg(mtError, "Object surface '%s' has more than one TC's.", buf.c_str()); Lord: Interpret
                }

                R_ASSERT(1 <= cnt);
                F.r_stringZ(buf);
                (*s_it)->SetTexture(buf.c_str());
                F.r_stringZ(buf);
                (*s_it)->SetVMap(buf.c_str());
                (*s_it)->SetShader(sh_name.c_str());
                (*s_it)->SetShaderXRLC("default");
            }

            // surfaces xrlc part
            if (F.find_chunk(EOBJ_CHUNK_SURFACES_XRLC))
                for (SurfaceIt s_it = m_Surfaces.begin(); s_it != m_Surfaces.end(); ++s_it)
                {
                    F.r_stringZ(buf);
                    (*s_it)->SetShaderXRLC(buf.c_str());
                }
        }

        // Load meshes
        IReader* OBJ = F.open_chunk(EOBJ_CHUNK_EDITMESHES);
        if (OBJ)
        {
            IReader* M = OBJ->open_chunk(0);
            for (int count = 1; M; count++)
            {
                CEditableMesh* mesh = new CEditableMesh(this);
                if (mesh->LoadMesh(*M))
                    m_Meshes.push_back(mesh);
                else
                {
                    // ELog.DlgMsg(mtError, "CEditableObject: Can't load mesh '%s'!", *mesh->m_Name); Lord: Interpret
                    xr_delete(mesh);
                    bRes = false;
                }
                M->close();
                if (!bRes)
                    break;
                M = OBJ->open_chunk(count);
            }
            OBJ->close();
        }

        // bones
        if (bRes)
        {
            IReader* B_CHUNK = F.open_chunk(EOBJ_CHUNK_BONES2);
            if (B_CHUNK)
            {
                int chunk = 0;
                IReader* O;
                while (0 != (O = B_CHUNK->open_chunk(chunk++)))
                {
                    m_Bones.push_back(new CBone());
                    m_Bones.back()->Load_1(*O);
                    O->close();
                }
                B_CHUNK->close();
                PrepareBones();
            }
            else if (F.find_chunk(EOBJ_CHUNK_BONES))
            {
                m_Bones.resize(F.r_u32());
                for (auto b_it = m_Bones.begin(); b_it != m_Bones.end(); ++b_it)
                {
                    *b_it = new CBone();
                    (*b_it)->Load_0(F);
                }
                PrepareBones();
            }

            // skeleton motions
            if (F.find_chunk(EOBJ_CHUNK_SMOTIONS))
            {
                m_SMotions.resize(F.r_u32());
                for (SMotionIt s_it = m_SMotions.begin(); s_it != m_SMotions.end(); ++s_it)
                {
                    *s_it = new CSMotion();
                    if (!(*s_it)->Load(F))
                    {
                        Log("!Motions has different version. Load failed.");
                        xr_delete(*s_it);
                        m_SMotions.clear();
                        break;
                    }
                    // resort bone_mots
                    (*s_it)->SortBonesBySkeleton(m_Bones);
                }
            }
            if (F.find_chunk(EOBJ_CHUNK_SMOTIONS2))
            {
                shared_str tmp;
                F.r_stringZ(tmp);
                u32 set_cnt = _GetItemCount(tmp.c_str());

                string_path nm;
                for (u32 k = 0; k < set_cnt; ++k)
                {
                    _GetItem(tmp.c_str(), k, nm);
                    m_SMotionRefs.push_back(nm);
                }
            }
            else if (F.find_chunk(EOBJ_CHUNK_SMOTIONS3))
            {
                u32 set_cnt = F.r_u32();

                shared_str nm;
                for (u32 k = 0; k < set_cnt; ++k)
                {
                    F.r_stringZ(nm);
                    m_SMotionRefs.push_back(nm);
                }
            }
        }

        // bone parts
        if (bRes)
        {
            if (F.find_chunk(EOBJ_CHUNK_BONEPARTS))
            {
                m_BoneParts.resize(F.r_u32());
                bool bBPok = true;
                for (auto& bp_it : m_BoneParts)
                {
                    F.r_stringZ(buf);
                    bp_it.alias = buf;
                    bp_it.bones.resize(F.r_u32());
                    for (auto& s_it : bp_it.bones)
                    {
                        int idx = F.r_u32();
                        if ((idx >= 0) && (idx < (int)m_Bones.size()))
                        {
                            s_it = m_Bones[idx]->Name();
                        }
                        else
                        {
                            Log("!Invalid bone parts.", GetName());
                            bBPok = false;
                            break;
                        }
                    }
                    if (!bBPok)
                        break;
                }
                if (!bBPok)
                    m_BoneParts.clear();
                if (!m_BoneParts.empty() && !VerifyBoneParts())
                    Log("!Invalid bone parts. Found duplicate bones in object '%s'.", GetName());
            }
            else if (F.find_chunk(EOBJ_CHUNK_BONEPARTS2))
            {
                m_BoneParts.resize(F.r_u32());
                for (auto& bp_it : m_BoneParts)
                {
                    F.r_stringZ(buf);
                    bp_it.alias = buf;
                    bp_it.bones.resize(F.r_u32());
                    for (auto& s_it : bp_it.bones)
                        F.r_stringZ(s_it);
                }
                if (!m_BoneParts.empty() && !VerifyBoneParts())
                    Log("!Invalid bone parts. Found duplicate bones in object '%s'.", GetName());
            }
        }

        if (bRes)
        {
            if (F.find_chunk(EOBJ_CHUNK_ACTORTRANSFORM))
            {
                F.r_fvector3(a_vPosition);
                F.r_fvector3(a_vRotate);
            }

            if (F.find_chunk(EOBJ_CHUNK_DESC))
            {
                F.r_stringZ(m_CreateName);
                F.r(&m_CreateTime, sizeof(m_CreateTime));
                F.r_stringZ(m_ModifName);
                F.r(&m_ModifTime, sizeof(m_ModifTime));
            }

            ResetSAnimation();
        }

        if (!bRes)
            break;
        UpdateBox();
        VerifyMeshNames();
    } while (0);

    return bRes;
}

bool CEditableObject::ExportOGF(LPCSTR fn, u8 infl)
{
    UpdateBox();
    CMemoryWriter F;

    if (PrepareOGF(F, infl, true, NULL))
    {
        return F.save_to(fn);
    }
    return false;
}
//------------------------------------------------------------------------------
bool CEditableObject::ExportOMF(LPCSTR fn)
{
    UpdateBox();
    CMemoryWriter F;
    if (PrepareOMF(F))
    {
        return F.save_to(fn);
    }
    return false;
}
//------------------------------------------------------------------------------
bool CEditableObject::ExportOBJ(LPCSTR fn)
{
    UpdateBox();
    CExportObjectOGF E(this);
    CMemoryWriter F;
    if (E.ExportAsWavefrontOBJ(F, fn))
    {
        return F.save_to(fn);
    }
    return false;
}
//------------------------------------------------------------------------------
#pragma endregion

#pragma region EditObjectEditor

//#include "EditObject.h"
//#include "EditMesh.h"
//#include "xrCore/Animation/Motion.hpp"
//#include "xrCore/Animation/Bone.hpp"
//#include "ExportSkeleton.h"
//#include "ExportObjectOGF.h"
//#include "D3DUtils.h"
//#include "ui_main.h"
//#include "render.h"
//#include "xrServerEntities/PropertiesListHelper.h"
#include "Layers/xrRender/ResourceManager.h"
//#include "ImageManager.h"

const float tex_w = LOD_SAMPLE_COUNT * LOD_IMAGE_SIZE;
const float tex_h = 1 * LOD_IMAGE_SIZE;
const float half_p_x = 0.5f * (1.f / tex_w);
const float half_p_y = 0.5f * (1.f / tex_h);
const float offs_x = 1.f / tex_w;
const float offs_y = 1.f / tex_h;

static Fvector LOD_pos[4] = {{-1.0f + offs_x, 1.0f - offs_y, 0.0f}, {1.0f - offs_x, 1.0f - offs_y, 0.0f},
    {1.0f - offs_x, -1.0f + offs_y, 0.0f}, {-1.0f + offs_x, -1.0f + offs_y, 0.0f}};

static FVF::LIT LOD[4] = {
    {{-1.0f, 1.0f, 0.0f}, 0xFFFFFFFF, {0.0f, 0.0f}}, // F 0
    {{1.0f, 1.0f, 0.0f}, 0xFFFFFFFF, {0.0f, 0.0f}}, // F 1
    {{1.0f, -1.0f, 0.0f}, 0xFFFFFFFF, {0.0f, 0.0f}}, // F 2
    {{-1.0f, -1.0f, 0.0f}, 0xFFFFFFFF, {0.0f, 0.0f}}, // F 3
};

bool CEditableObject::Reload()
{
    ClearGeometry();
    return Load(m_LoadName.c_str());
}

bool CEditableObject::RayPick(
    float& dist, const Fvector& S, const Fvector& D, const Fmatrix& inv_parent, SRayPickInfo* pinf)
{
    bool picked = false;
    for (EditMeshIt m = m_Meshes.begin(); m != m_Meshes.end(); m++)
        if ((*m)->RayPick(dist, S, D, inv_parent, pinf))
            picked = true;
    return picked;
}

void CEditableObject::RayQuery(SPickQuery& pinf)
{
    for (EditMeshIt m = m_Meshes.begin(); m != m_Meshes.end(); m++)
        (*m)->RayQuery(pinf);
}

void CEditableObject::RayQuery(const Fmatrix& parent, const Fmatrix& inv_parent, SPickQuery& pinf)
{
    for (EditMeshIt m = m_Meshes.begin(); m != m_Meshes.end(); m++)
        (*m)->RayQuery(parent, inv_parent, pinf);
}

void CEditableObject::BoxQuery(const Fmatrix& parent, const Fmatrix& inv_parent, SPickQuery& pinf)
{
    for (EditMeshIt m = m_Meshes.begin(); m != m_Meshes.end(); m++)
        (*m)->BoxQuery(parent, inv_parent, pinf);
}

bool CEditableObject::FrustumPick(const CFrustum& frustum, const Fmatrix& parent)
{
    for (EditMeshIt m = m_Meshes.begin(); m != m_Meshes.end(); m++)
        if ((*m)->FrustumPick(frustum, parent))
            return true;
    return false;
}

bool CEditableObject::BoxPick(CCustomObject* obj, const Fbox& box, const Fmatrix& inv_parent, SBoxPickInfoVec& pinf)
{
    bool picked = false;
    for (EditMeshIt m = m_Meshes.begin(); m != m_Meshes.end(); m++)
        if ((*m)->BoxPick(box, inv_parent, pinf))
        {
            pinf.back().s_obj = obj;
            picked = true;
        }
    return picked;
}

extern float ssaLIMIT;
extern float g_fSCREEN;
static const float ssaLim = 64.f * 64.f / (640 * 480);

void CEditableObject::Render(const Fmatrix& parent, int priority, bool strictB2F)
{
    if (!(m_LoadState.is(LS_RBUFFERS)))
        DefferedLoadRP();

    Fvector v;
    float r;
    Fbox bb;
    bb.xform(m_BBox, parent);
    bb.getsphere(v, r);

    if (EditorPref::GetInstance().object_flags.is(epoDrawLOD) &&
        (m_objectFlags.is(eoUsingLOD) && (CalcSSA(v, r) < ssaLim)))
    {
        if ((1 == priority) && (true == strictB2F))
            RenderLOD(parent);
    }
    else
    {
        RCache.set_xform_world(parent);
        if (m_objectFlags.is(eoHOM))
        {
            if ((1 == priority) && (false == strictB2F))
                RenderEdge(parent, 0, 0, 0x40B64646);

            if ((2 == priority) && (true == strictB2F))
                RenderSelection(parent, 0, 0, 0xA0FFFFFF);
        }
        else if (m_objectFlags.is(eoSoundOccluder))
        {
            if ((1 == priority) && (false == strictB2F))
                RenderEdge(parent, 0, 0, 0xFF000000);

            if ((2 == priority) && (true == strictB2F))
                RenderSelection(parent, 0, 0, 0xA00000FF);
        }
        else
        {
            if (psDeviceFlags.is(rsEdgedFaces) && (1 == priority) && (false == strictB2F))
                RenderEdge(parent);

            for (SurfaceIt s_it = m_Surfaces.begin(); s_it != m_Surfaces.end(); s_it++)
            {
                int pr = (*s_it)->_Priority();
                bool strict = (*s_it)->_StrictB2F();

                if ((priority == pr) && (strictB2F == strict))
                {
                    RCache.set_Shader((*s_it)->_Shader());
                    for (EditMeshIt _M = m_Meshes.begin(); _M != m_Meshes.end(); _M++)
                        if (IsSkeleton())
                            (*_M)->RenderSkeleton(parent, *s_it);
                        else
                        {
                             (*_M)->Render(parent, *s_it);

 
                        }
                            
                }
            }
        }
    }
}

void CEditableObject::RenderSingle(const Fmatrix& parent)
{
    for (int i = 0; i < 4; i++)
    {
        Render(parent, i, false);
        Render(parent, i, true);
    }
}

void CEditableObject::RenderAnimation(const Fmatrix&) {}
void CEditableObject::RenderEdge(const Fmatrix& parent, CEditableMesh* mesh, CSurface* surf, u32 color)
{
    if (!(m_LoadState.is(LS_RBUFFERS)))
        DefferedLoadRP();

    RCache.set_Shader(RImplementation.m_WireShader);
    if (mesh)
        mesh->RenderEdge(parent, surf, color);
    else
        for (EditMeshIt _M = m_Meshes.begin(); _M != m_Meshes.end(); _M++)
            (*_M)->RenderEdge(parent, surf, color);
}

void CEditableObject::RenderSelection(const Fmatrix& parent, CEditableMesh* mesh, CSurface* surf, u32 color)
{
    if (!(m_LoadState.is(LS_RBUFFERS)))
        DefferedLoadRP();

    RCache.set_xform_world(parent);
    RCache.set_Shader(RImplementation.m_SelectionShader);
    //   EDevice.RenderNearer(0.0005); Lord: think
    if (mesh)
        mesh->RenderSelection(parent, surf, color);
    else
        for (EditMeshIt _M = m_Meshes.begin(); _M != m_Meshes.end(); _M++)
            (*_M)->RenderSelection(parent, surf, color);
    //   EDevice.ResetNearer(); Lord: think
}

IC static void CalculateLODTC(int frame, int w_cnt, int h_cnt, Fvector2& lt, Fvector2& rb)
{
    Fvector2 ts;
    ts.set(1.f / (float)w_cnt, 1.f / (float)h_cnt);
    lt.x = (frame % w_cnt + 0) * ts.x + half_p_x;
    lt.y = (frame / w_cnt + 0) * ts.y + half_p_y;
    rb.x = (frame % w_cnt + 1) * ts.x - half_p_x;
    rb.y = (frame / w_cnt + 1) * ts.y - half_p_y;
}

void CEditableObject::GetLODFrame(int frame, Fvector p[4], Fvector2 t[4], const Fmatrix* parent)
{
    R_ASSERT(m_objectFlags.is(eoUsingLOD));
    Fvector P, S;
    m_BBox.get_CD(P, S);
    float r = _max(S.x, S.z); // sqrtf(S.x*S.x+S.z*S.z);
    Fmatrix T, matrix, rot;
    T.scale(r, S.y, r);
    T.translate_over(P);
    if (parent)
        T.mulA_43(*parent);

    float angle = frame * (PI_MUL_2 / float(LOD_SAMPLE_COUNT));
    rot.rotateY(-angle);
    matrix.mul(T, rot);
    Fvector2 lt, rb;
    CalculateLODTC(frame, LOD_SAMPLE_COUNT, 1, lt, rb);
    t[0].set(lt);
    t[1].set(rb.x, lt.y);
    t[2].set(rb);
    t[3].set(lt.x, rb.y);
    matrix.transform_tiny(p[0], LOD_pos[0]);
    matrix.transform_tiny(p[1], LOD_pos[1]);
    matrix.transform_tiny(p[2], LOD_pos[2]);
    matrix.transform_tiny(p[3], LOD_pos[3]);
}

void CEditableObject::RenderLOD(const Fmatrix& parent)
{
    Fvector C;
    C.sub(parent.c, Device.vCameraPosition);
    C.y = 0;
    float m = C.magnitude();
    if (m < EPS)
        return;
    C.div(m);
    int max_frame;
    float max_dot = 0;
    Fvector HPB;
    parent.getHPB(HPB);

    for (int frame = 0; frame < LOD_SAMPLE_COUNT; frame++)
    {
        float angle = angle_normalize(frame * (PI_MUL_2 / float(LOD_SAMPLE_COUNT)) + HPB.x);

        Fvector D;
        D.setHP(angle, 0);
        float dot = C.dotproduct(D);
        if (dot < 0.7072f)
            continue;

        if (dot > max_dot)
        {
            max_dot = dot;
            max_frame = frame;
        }
    }
    {
        Fvector p[4];
        Fvector2 t[4];
        GetLODFrame(max_frame, p, t);
        for (int i = 0; i < 4; i++)
        {
            LOD[i].p.set(p[i]);
            LOD[i].t.set(t[i]);
        }
        RCache.set_xform_world(parent);
        RCache.set_Shader(m_LODShader ? m_LODShader : RImplementation.m_WireShader);
        DUImpl.DrawPrimitiveLIT(D3DPT_TRIANGLEFAN, 2, LOD, 4, true, false);
    }
}

xr_string CEditableObject::GetLODTextureName()
{
    string512 nm;
    strcpy(nm, m_LibName.c_str());
    _ChangeSymbol(nm, '\\', '_');
    xr_string l_name;
    l_name = xr_string("lod_") + nm;
    return ImageLib.UpdateFileName(l_name);
}

void CEditableObject::OnDeviceCreate() {}
void CEditableObject::OnDeviceDestroy() { DefferedUnloadRP(); }
void CEditableObject::DefferedLoadRP()
{
    if (m_LoadState.is(LS_RBUFFERS))
        return;

    // skeleton
    if (IsSkeleton())
        vs_SkeletonGeom.create(FVF_SV, RCache.Vertex.Buffer(), RCache.Index.Buffer());

    //*/
    // создать LOD shader
    xr_string l_name = GetLODTextureName();
    xr_string fname = xr_string(l_name) + xr_string(".dds");
    m_LODShader.destroy();
    //    if (FS.exist(_game_textures_,fname.c_str()))
    if (m_objectFlags.is(eoUsingLOD))
        m_LODShader.create(GetLODShaderName(), l_name.c_str());
    m_LoadState.set(LS_RBUFFERS, TRUE);
}

void CEditableObject::DefferedUnloadRP()
{
    if (!(m_LoadState.is(LS_RBUFFERS)))
        return;
    // skeleton
    vs_SkeletonGeom.destroy();
    // удалить буфера
    for (EditMeshIt _M = m_Meshes.begin(); _M != m_Meshes.end(); _M++)
        if (*_M)
            (*_M)->GenerateRenderBuffers();
    // удалить shaders
    for (SurfaceIt s_it = m_Surfaces.begin(); s_it != m_Surfaces.end(); s_it++)
        (*s_it)->OnDeviceDestroy();
    // LOD
    m_LODShader.destroy();
    m_LoadState.set(LS_RBUFFERS, FALSE);
}

void CEditableObject::EvictObject()
{
    EditMeshIt m = m_Meshes.begin();
    for (; m != m_Meshes.end(); m++)
    {
        (*m)->UnloadCForm();
        (*m)->UnloadVNormals(true);
        (*m)->UnloadSVertices(true);
        (*m)->UnloadFNormals(true);
    }
    DefferedUnloadRP();
}

// bool CEditableObject::PrepareOGF(IWriter& F, u8 infl, bool gen_tb, CEditableMesh* mesh)
//{
//	return IsSkeleton()?PrepareSkeletonOGF(F,infl):PrepareRigidOGF(F,gen_tb,mesh);
//}
//
// bool CEditableObject::PrepareRigidOGF(IWriter& F, bool gen_tb, CEditableMesh* mesh)
//{
//    CExportObjectOGF E(this);
//    return E.Export(F,gen_tb,mesh);
//}
//
// bool CEditableObject::PrepareSVGeometry(IWriter& F, u8 infl)
//{
//    CExportSkeleton E(this);
//    return E.ExportGeometry(F, infl);
//}
//
// bool CEditableObject::PrepareSVKeys(IWriter& F)
//{
//    CExportSkeleton E(this);
//    return E.ExportMotionKeys(F);
//}
//
// bool CEditableObject::PrepareSVDefs(IWriter& F)
//{
//    CExportSkeleton E(this);
//    return E.ExportMotionDefs(F);
//}
//
// bool CEditableObject::PrepareSkeletonOGF(IWriter& F, u8 infl)
//{
//    CExportSkeleton E(this);
//    return E.Export(F,infl);
//}
//
// bool CEditableObject::PrepareOMF(IWriter& F)
//{
//    CExportSkeleton E(this);
//    return E.ExportMotions(F);
//}
//---------------------------------------------------------------------------

void __fastcall CEditableObject::OnChangeTransform(PropValue*)
{
    //    UI->RedrawScene(); Lord: think about it
}
//---------------------------------------------------------------------------

//#include "Layers/xrRender/blenders/Blender.h"
IC BOOL BE(BOOL A, BOOL B)
{
    bool a = !!A;
    bool b = !!B;
    return a == b;
}

bool CEditableObject::CheckShaderCompatible()
{
    bool bRes = true;
    for (SurfaceIt s_it = m_Surfaces.begin(); s_it != m_Surfaces.end(); s_it++)
    {
        IBlender* B = RImplementation.Resources->_FindBlender(*(*s_it)->m_ShaderName);
        Shader_xrLC* C = RImplementation.ShaderXRLC.Get(*(*s_it)->m_ShaderXRLCName);
        if (!B || !C)
        {
            //  ELog.Msg(mtError, "Object '%s': invalid or missing shader [E:'%s', C:'%s']", GetName(),
            //   (*s_it)->_ShaderName(), (*s_it)->_ShaderXRLCName()); Lord: Interpret
            bRes = false;
        }
        else
        {
            if (!BE(B->canBeLMAPped(), !C->flags.bLIGHT_Vertex))
            {
                //       ELog.Msg(mtError, "Object '%s': engine shader '%s' non compatible with compiler shader '%s'",
                //       GetName(),
                //      (*s_it)->_ShaderName(), (*s_it)->_ShaderXRLCName()); Lord: Interpret
                bRes = false;
            }
        }
    }
    return bRes;
}

void CEditableObject::AddBone(CBone* parent_bone)
{
    CBone* B = new CBone();

    string256 new_name;
    u32 i = 0;

    do
    {
        sprintf(new_name, "bone%.3d", i++);
    } while (FindBoneByName(new_name));

    B->SetName(new_name);
    B->SetWMap("");
    B->SetRestParams(0.01f, Fvector().set(0, 0, 0), Fvector().set(0, 0, 0));

    B->ResetData();
    B->Reset();

    if (parent_bone)
    {
        B->SetParentName(parent_bone->Name().c_str());
    }
    else
    {
        int bid = GetRootBoneID();
        GetBone(bid)->SetParentName(B->Name().c_str());
        B->SetParentName("");
    }

    m_BoneParts[0].bones.push_back(B->Name());
    m_Bones.push_back(B);
    PrepareBones();

    for (EditMeshIt _M = m_Meshes.begin(); _M != m_Meshes.end(); _M++)
    {
        (*_M)->UnloadSVertices();
    }
}

CBone* bone_to_delete = NULL;
u32 bone_to_delete_frame = 0;

void CEditableObject::DeleteBone(CBone* bone)
{
    CBone* PB = bone->Parent();

    for (BoneVec::iterator b_it = m_Bones.begin(); b_it != m_Bones.end(); ++b_it)
    {
        CBone* B = *b_it;
        if (B->Parent() == bone)
            B->SetParentName(PB ? PB->Name().c_str() : "");
    }
    BoneVec::iterator bit = std::find(m_Bones.begin(), m_Bones.end(), bone);
    if (bit == m_Bones.end())
        Msg("! bone not found -((");
    else
        m_Bones.erase(bit);

    BPIt bpit = BonePart(bone);
    RStringVec::iterator iit = (*bpit).bones.begin();
    RStringVec::iterator iit_e = (*bpit).bones.end();
    for (; iit != iit_e; ++iit)
    {
        if (*iit == bone->Name())
        {
            (*bpit).bones.erase(iit);
            break;
        }
    }

    bone_to_delete = bone;
    bone_to_delete_frame = Device.dwFrame;
    PrepareBones();

    for (EditMeshIt _M = m_Meshes.begin(); _M != m_Meshes.end(); _M++)
    {
        (*_M)->UnloadSVertices();
    }
}

//---------------------------------------------------------------------------

BPIt CEditableObject::BonePart(CBone* B)
{
    BPIt it = FirstBonePart();
    BPIt it_e = LastBonePart();
    for (; it != it_e; ++it)
    {
        RStringVec::iterator iit = (*it).bones.begin();
        RStringVec::iterator iit_e = (*it).bones.end();
        for (; iit != iit_e; ++iit)
        {
            if (*iit == B->Name())
                return it;
        }
    }
    return it_e;
}

void CEditableObject::RenameBone(CBone* bone, LPCSTR new_name)
{
    BPIt bpit = BonePart(bone);
    RStringVec::iterator iit = (*bpit).bones.begin();
    RStringVec::iterator iit_e = (*bpit).bones.end();

    for (; iit != iit_e; ++iit)
    {
        if (*iit == bone->Name())
        {
            *iit = new_name;
            break;
        }
    }

    for (BoneVec::iterator b_it = m_Bones.begin(); b_it != m_Bones.end(); ++b_it)
    {
        CBone* B = *b_it;
        if (B->ParentName() == bone->Name())
            B->SetParentName(new_name);
    }

    for (SMotionIt s_it = m_SMotions.begin(); s_it != m_SMotions.end(); ++s_it)
    {
        CSMotion* M = *s_it;
        for (BoneMotionVec::iterator bm_it = M->BoneMotions().begin(); bm_it != M->BoneMotions().end(); ++bm_it)
        {
            if (bm_it->name == bone->Name())
                bm_it->name = new_name;
        }
    }

    bone->SetName(new_name);

    Modified();
}

#pragma endregion
