#pragma once

#include "xrCore/Animation/Bone.hpp"
#include "xrCore/Animation/Motion.hpp"
#include "xrCore/_fbox.h"
#include "xrCore/_std_extensions.h"

 
#include "PropertiesListTypes.h"
#include "xrEngine/GameMtlLib.h"
#include "pick_defs.h"
 
#include "CSurface.h"
#include "Include/xrRender/Kinematics.h"

#include "PhysicsShellHolderEditorBase.h"
//----------------------------------------------------
struct SRayPickInfo;
class CEditableMesh;
class CFrustum;
class CCustomMotion;
class CBone;
class Shader;
class Mtl;
class CExporter;
class CMayaTranslator;
struct st_ObjectDB;
struct SXRShaderData;
struct ogf_desc;
class CCustomObject;

namespace KinematicsABT
{
struct additional_bone_transform;
}

 

#define LOD_SHADER_NAME "details\\lod"
#define LOD_SAMPLE_COUNT 8
#define LOD_IMAGE_SIZE 64
#define RENDER_SKELETON_LINKS 4

// refs
class XRayMtl;
class SSimpleImage;


using SurfaceVec = xr_vector<CSurface*>;
using SurfaceIt = SurfaceVec::iterator;

using EditMeshVec = xr_vector<CEditableMesh*>;
using EditMeshIt = EditMeshVec::iterator;

using OMotionVec = xr_vector<COMotion*>;
using OMotionIt = OMotionVec::iterator;

using SMotionVec = xr_vector<CSMotion*>;
using SMotionIt = SMotionVec::iterator;

struct SBonePart
{
    shared_str alias;
    RStringVec bones;
};
using BPVec = xr_vector<SBonePart>;
using BPIt = BPVec::iterator;

const u32 FVF_SV = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL;

class CEditableObject : public IKinematics, public CPhysicsShellHolderEditorBase
{
    friend class CSceneObject;
    friend class CEditableMesh;
    friend class TfrmPropertiesEObject;
    friend class CSector;
    friend class TUI_ControlSectorAdd;
    friend class ELibrary;
    friend class TfrmEditLibrary;
    friend class MeshExpUtility;

 
    ref_geom vs_SkeletonGeom;
 
    // desc
    shared_str m_CreateName;
    time_t m_CreateTime;
    shared_str m_ModifName;
    time_t m_ModifTime;

    // general
    xr_string m_ClassScript;

    SurfaceVec m_Surfaces;
    EditMeshVec m_Meshes;

    ref_shader m_LODShader;

    // skeleton
    BoneVec m_Bones;
    SMotionVec m_SMotions;
    BPVec m_BoneParts;
    CSMotion* m_ActiveSMotion;
    CPhysicsShell* m_physics_shell;
    Fmatrix* m_object_xform;

public:
    SAnimParams m_SMParam;
    xr_vector<shared_str> m_SMotionRefs;
    shared_str m_LODs;

public:
    // options
    Flags32 m_objectFlags;
    enum
    {
        eoDynamic = (1 << 0),
        eoProgressive = (1 << 1),
        eoUsingLOD = (1 << 2),
        eoHOM = (1 << 3),
        eoMultipleUsage = (1 << 4),
        eoSoundOccluder = (1 << 5),
        eoHQExport = (1 << 6),
        eoFORCE32 = u32(-1)
    };
    inline BOOL IsDynamic() { return m_objectFlags.is(eoDynamic); }
    inline BOOL IsStatic()
    {
        return !m_objectFlags.is(eoSoundOccluder) && !m_objectFlags.is(eoDynamic) && !m_objectFlags.is(eoHOM) &&
            !m_objectFlags.is(eoMultipleUsage);
    }
    inline BOOL IsMUStatic()
    {
        return !m_objectFlags.is(eoSoundOccluder) && !m_objectFlags.is(eoDynamic) && !m_objectFlags.is(eoHOM) &&
            m_objectFlags.is(eoMultipleUsage);
    }

private:
    // bounding volume
    Fbox m_BBox;

public:
    // temp variable for actor
    Fvector a_vPosition;
    Fvector a_vRotate;

    // temp variables for transformation
    Fvector t_vPosition;
    Fvector t_vScale;
    Fvector t_vRotate;

    bool bOnModified;
    inline bool IsModified() { return bOnModified; }
    inline void Modified() { bOnModified = true; }
    xr_string m_LoadName;
    int m_RefCount;

protected:
    int m_ObjectVersion;

    void ClearGeometry();

    void PrepareBones();
    void DefferedLoadRP();
    void DefferedUnloadRP();

    void __stdcall OnChangeTransform(PropValue* prop);
    void __stdcall OnChangeShader(PropValue* prop);

public:
    enum
    {
        LS_RBUFFERS = (1 << 0),
    };
    Flags32 m_LoadState;

    xr_string m_LibName;

public:
    // constructor/destructor methods
    CEditableObject(LPCSTR name);
    CEditableObject(void) noexcept = default;
    virtual ~CEditableObject();

    LPCSTR GetName() { return m_LibName.c_str(); }
    void SetVersionToCurrent(BOOL bCreate, BOOL bModif);

    void Optimize();

    inline EditMeshIt FirstMesh() { return m_Meshes.begin(); }
    inline EditMeshIt LastMesh() { return m_Meshes.end(); }
    inline EditMeshVec& Meshes() { return m_Meshes; }
    inline int MeshCount() { return m_Meshes.size(); }
    inline void AppendMesh(CEditableMesh* M) { m_Meshes.push_back(M); }
    inline SurfaceVec& Surfaces() { return m_Surfaces; }
    inline SurfaceIt FirstSurface() { return m_Surfaces.begin(); }
    inline SurfaceIt LastSurface() { return m_Surfaces.end(); }
    inline int SurfaceCount() { return m_Surfaces.size(); }
    inline int Version() { return m_ObjectVersion; }
    // LOD
    xr_string GetLODTextureName();
    LPCSTR GetLODShaderName() { return LOD_SHADER_NAME; }
    void GetLODFrame(int frame, Fvector p[4], Fvector2 t[4], const Fmatrix* parent = 0);

    // skeleton
    inline BPIt FirstBonePart() { return m_BoneParts.begin(); }
    inline BPIt LastBonePart() { return m_BoneParts.end(); }
    inline BPVec& BoneParts() { return m_BoneParts; }
    inline int BonePartCount() { return m_BoneParts.size(); }
    inline BPIt BonePart(CBone* B);

    auto FirstBone() { return m_Bones.begin(); }
    auto LastBone() { return m_Bones.end(); }
    inline BoneVec& Bones() { return m_Bones; }
    inline int BoneCount() const { return m_Bones.size(); }
    shared_str BoneNameByID(int id);
    int GetRootBoneID();
    int PartIDByName(LPCSTR name);
    inline CBone* GetBone(u32 idx)
    {
        VERIFY(idx < m_Bones.size());
        return m_Bones[idx];
    }
    inline const CBone* GetBone(u32 idx) const
    {
        VERIFY(idx < m_Bones.size());
        return m_Bones[idx];
    }
    void GetBoneWorldTransform(u32 bone_idx, float t, CSMotion* motion, Fmatrix& matrix);
    inline SMotionIt FirstSMotion() { return m_SMotions.begin(); }
    inline SMotionIt LastSMotion() { return m_SMotions.end(); }
    SMotionVec& SMotions() { return m_SMotions; }
    inline int SMotionCount() { return m_SMotions.size(); }
    inline bool IsAnimated() { return SMotionCount() || m_SMotionRefs.size(); }
    inline void SkeletonPlay() { m_SMParam.Play(); }
    inline void SkeletonStop() { m_SMParam.Stop(); }
    inline void SkeletonPause(bool val) { m_SMParam.Pause(val); }
    // get object properties methods

    inline xr_string& GetClassScript() { return m_ClassScript; }
    inline const Fbox& GetBox() const { return m_BBox; }
    inline LPCSTR GetLODs() { return m_LODs.c_str(); }
    // animation
    inline bool IsSkeleton() { return !!m_Bones.size(); }
    inline bool IsSMotionActive() { return IsSkeleton() && m_ActiveSMotion; }
    CSMotion* GetActiveSMotion() { return m_ActiveSMotion; }
    void SetActiveSMotion(CSMotion* mot);
    bool CheckBoneCompliance(CSMotion* M);
    bool VerifyBoneParts();
    void OptimizeSMotions();

    bool LoadBoneData(IReader& F);
    void SaveBoneData(IWriter& F);
    void ResetBones();
    CSMotion* ResetSAnimation(bool bGotoBindPose = true);
    void CalculateAnimation(CSMotion* motion);
    void CalculateBindPose();
    void GotoBindPose();
    void OnBindTransformChange();

    // statistics methods
    void GetFaceWorld(const Fmatrix& parent, CEditableMesh* M, int idx, Fvector* verts);
    int GetFaceCount(bool bMatch2Sided = true, bool bIgnoreOCC = true);
    int GetVertexCount();
    int GetSurfFaceCount(LPCSTR surf_name);

    // render methods
    void Render(const Fmatrix& parent, int priority, bool strictB2F);
    void RenderSelection(const Fmatrix& parent, CEditableMesh* m = 0, CSurface* s = 0, u32 c = 0x40E64646);
    void RenderEdge(const Fmatrix& parent, CEditableMesh* m = 0, CSurface* s = 0, u32 c = 0xFFC0C0C0);
    void RenderBones(const Fmatrix& parent);
    void RenderAnimation(const Fmatrix& parent);
    void RenderSingle(const Fmatrix& parent);
    void RenderSkeletonSingle(const Fmatrix& parent);
    void RenderLOD(const Fmatrix& parent);

    // update methods
    void OnFrame(void);
    void UpdateBox(void);
    void EvictObject(void);

    // pick methods
    bool RayPick(float& dist, const Fvector& S, const Fvector& D, const Fmatrix& inv_parent, SRayPickInfo* pinf = nullptr);
 
    void AddBone(CBone* parent_bone);
    void DeleteBone(CBone* bone);
    void RenameBone(CBone* bone, LPCSTR new_name);

    void RayQuery(SPickQuery& pinf);
    void RayQuery(const Fmatrix& parent, const Fmatrix& inv_parent, SPickQuery& pinf);
    void BoxQuery(const Fmatrix& parent, const Fmatrix& inv_parent, SPickQuery& pinf);
    bool BoxPick(CCustomObject* obj, const Fbox& box, const Fmatrix& inv_parent, SBoxPickInfoVec& pinf);
    bool FrustumPick(const CFrustum& frustum, const Fmatrix& parent);
    bool SpherePick(const Fvector& center, float radius, const Fmatrix& parent);

    // bone
    CBone* PickBone(const Fvector& S, const Fvector& D, const Fmatrix& parent);
    void SelectBones(bool bVal);
    void SelectBone(CBone* b, bool bVal);
    void ClampByLimits(bool bSelOnly);
 
    // change position/orientation methods
    void TranslateToWorld(const Fmatrix& parent);

    // clone/copy methods
    void RemoveMesh(CEditableMesh* mesh);

    bool RemoveSMotion(LPCSTR name);
    bool RenameSMotion(LPCSTR old_name, LPCSTR new_name);
    bool AppendSMotion(LPCSTR fname, SMotionVec* inserted = 0);
    void ClearSMotions();
    bool SaveSMotions(LPCSTR fname);

    // load/save methods
    bool Reload();
 
    bool Load(LPCSTR fname);
 
 
    bool Load(IReader&);
 
    bool Save(LPCSTR fname);
    void Save(IWriter&);
 
  //  void FillMotionList(LPCSTR pref, ListItemsVec& items, int modeID);
  //  void FillBoneList(LPCSTR pref, ListItemsVec& items, int modeID);
 //   void FillSurfaceList(LPCSTR pref, ListItemsVec& items, int modeID);
 ////   void FillSurfaceProps(CSurface* surf, LPCSTR pref, PropItemVec& items);
  //  void FillBasicProps(LPCSTR pref, PropItemVec& items);
  //  void FillSummaryProps(LPCSTR pref, PropItemVec& items); // Lord: All fill to interpret imgui
    bool CheckShaderCompatible();
 

    // contains methods
    CEditableMesh* FindMeshByName(LPCSTR name, CEditableMesh* Ignore = 0);
    void VerifyMeshNames();
    bool ContainsMesh(const CEditableMesh* m);
    CSurface* FindSurfaceByName(LPCSTR surf_name, int* s_id = 0);
    int FindBoneByNameIdx(LPCSTR name);
    auto FindBoneByNameIt(LPCSTR name);
    CBone* FindBoneByName(LPCSTR name);
    int GetSelectedBones(BoneVec& sel_bones);
    u16 GetBoneIndexByWMap(LPCSTR wm_name);
    CSMotion* FindSMotionByName(LPCSTR name, const CSMotion* Ignore = 0);
    void GenerateSMotionName(char* buffer, LPCSTR start_name, const CSMotion* M);
    bool GenerateBoneShape(bool bSelOnly);

    // device dependent routine
    void OnDeviceCreate();
    void OnDeviceDestroy();

    // utils
    void PrepareOGFDesc(ogf_desc& desc);
    // skeleton
    bool PrepareSVGeometry(IWriter& F, u8 infl);
    bool PrepareSVKeys(IWriter& F);
    bool PrepareSVDefs(IWriter& F);
    bool PrepareSkeletonOGF(IWriter& F, u8 infl);
    // rigid
    bool PrepareRigidOGF(IWriter& F, bool gen_tb, CEditableMesh* mesh);
 
    // ogf
    bool PrepareOGF(IWriter& F, u8 infl, bool gen_tb, CEditableMesh* mesh);
    bool ExportOGF(LPCSTR fname, u8 skl_infl);
    // omf
    bool PrepareOMF(IWriter& F);
    bool ExportOMF(LPCSTR fname);
    // obj
    bool ExportOBJ(LPCSTR name);
 
    LPCSTR GenerateSurfaceName(LPCSTR base_name);
#ifdef _MAX_EXPORT
    BOOL ExtractTexName(Texmap* src, LPSTR dest);
    BOOL ParseStdMaterial(StdMat* src, CSurface* dest);
    BOOL ParseMultiMaterial(MultiMtl* src, u32 mid, CSurface* dest);
    BOOL ParseXRayMaterial(XRayMtl* src, u32 mid, CSurface* dest);
    CSurface* CreateSurface(Mtl* M, u32 mat_id);
    bool ImportMAXSkeleton(CExporter* exporter);
#endif


    bool ImportLWO(const char* fname, bool optimize);
    bool ImportLWO(st_ObjectDB* I);
    Flags32 m_Flags;


#ifdef _MAYA_EXPORT
    BOOL ParseMAMaterial(CSurface* dest, SXRShaderData& d);
    CSurface* CreateSurface(LPCSTR m_name, SXRShaderData& d);
    CSurface* CreateSurface(MObject shader);
#endif

    bool ExportLWO(LPCSTR fname);
    bool Validate();

private:
    float GetBonesBottom();

public:
    void CalculateRootObjectAnimation(const Fmatrix& anchor);
    void GetAnchorForRootObjectAnimation(Fmatrix& anchor);
    bool AnimateRootObject(CSMotion* motion);

private:
    virtual void Bone_Calculate(CBoneData* bd, Fmatrix* parent) { VERIFY(false); }
    virtual void Bone_GetAnimPos(Fmatrix& pos, u16 id, u8 channel_mask, bool ignore_callbacks) { VERIFY(false); }
    virtual bool PickBone(
        const Fmatrix& parent_xform, pick_result& r, float dist, const Fvector& start, const Fvector& dir, u16 bone_id)
    {
        VERIFY(false);
        return false;
    }
    virtual void EnumBoneVertices(SEnumVerticesCallback& C, u16 bone_id) { VERIFY(false); }
    // Low level interface
    virtual u16 LL_BoneID(LPCSTR B)
    {
        int id = FindBoneByNameIdx(B);
        VERIFY(id < u16(-1));
        return (u16)id;
    }
    virtual u16 LL_BoneID(const shared_str& B) { return LL_BoneID(B.c_str()); }
    virtual LPCSTR LL_BoneName_dbg(u16 ID);

    virtual CInifile* LL_UserData() { return 0; }
    virtual accel* LL_Bones()
    {
        VERIFY(false);
        return 0;
    }

    virtual CBoneInstance& LL_GetBoneInstance(u16 bone_id);

    virtual CBoneData& LL_GetData(u16 bone_id);

    virtual const IBoneData& GetBoneData(u16 bone_id) const { return *GetBone(bone_id); }
    virtual u16 LL_BoneCount() const { return (u16)BoneCount(); }
    virtual u16 LL_VisibleBoneCount()
    {
        VERIFY(false);
        return 0;
    }
    virtual ICF Fmatrix& LL_GetTransform(u16 bone_id) { return GetBone(bone_id)->_LTransform(); }
    virtual ICF const Fmatrix& LL_GetTransform(u16 bone_id) const { return GetBone(bone_id)->_LTransform(); }
    virtual ICF Fmatrix& LL_GetTransform_R(u16 bone_id);
    virtual Fobb& LL_GetBox(u16 bone_id);
    virtual void LL_GetBindTransform(xr_vector<Fmatrix>& matrices) { VERIFY(false); }
    virtual int LL_GetBoneGroups(xr_vector<xr_vector<u16>>& groups)
    {
        VERIFY(false);
        return 0;
    }

    virtual u16 LL_GetBoneRoot()
    {
        u16 root_id = (u16)GetRootBoneID();
        VERIFY(root_id < u16(-1));
        return root_id;
    }
    virtual void LL_SetBoneRoot(u16 bone_id) { VERIFY(false); }
    virtual BOOL LL_GetBoneVisible(u16 bone_id) { return TRUE; }
    virtual void LL_SetBoneVisible(u16 bone_id, BOOL val, BOOL bRecursive) { VERIFY(false); }
    virtual u64 LL_GetBonesVisible() { return u64(-1); }
    virtual void LL_SetBonesVisible(u64 mask) { VERIFY(false); }

    virtual void LL_AddTransformToBone(KinematicsABT::additional_bone_transform& offset) {} //--#SM+#--
    virtual void LL_ClearAdditionalTransform(u16 bone_id){}; //--#SM+#--

    // Main functionality
    virtual void CalculateBones(BOOL bForceExact = FALSE) {} // Recalculate skeleton
    virtual void CalculateBones_Invalidate() {}
    virtual void Callback(UpdateCallback C, void* Param) { VERIFY(false); }
    //	Callback: data manipulation
    virtual void SetUpdateCallback(UpdateCallback pCallback) { VERIFY(false); }
    virtual void SetUpdateCallbackParam(void* pCallbackParam) { VERIFY(false); }
    virtual UpdateCallback GetUpdateCallback()
    {
        VERIFY(false);
        return 0;
    }
    virtual void* GetUpdateCallbackParam()
    {
        VERIFY(false);
        return 0;
    }
    // UpdateCallback				Update_Callback;
    // void*						Update_Callback_Param;
    virtual IRenderVisual* dcast_RenderVisual() { return 0; }
    virtual IKinematicsAnimated* dcast_PKinematicsAnimated()
    {
        VERIFY(false);
        return 0;
    }

// debug
#ifdef DEBUG
    virtual void DebugRender(Fmatrix& XFORM) { VERIFY(false); }
    virtual shared_str getDebugName() { return m_ModifName; }
#endif
private:
    virtual IKinematics* ObjectKinematics() { return this; }
};
//----------------------------------------------------
#define EOBJ_CURRENT_VERSION 0x0010
//----------------------------------------------------
#define EOBJ_CHUNK_OBJECT_BODY 0x7777
#define EOBJ_CHUNK_VERSION 0x0900
#define EOBJ_CHUNK_REFERENCE 0x0902
#define EOBJ_CHUNK_FLAGS 0x0903
#define EOBJ_CHUNK_SURFACES 0x0905
#define EOBJ_CHUNK_SURFACES2 0x0906
#define EOBJ_CHUNK_SURFACES3 0x0907
#define EOBJ_CHUNK_EDITMESHES 0x0910
#define EOBJ_CHUNK_CLASSSCRIPT 0x0912
#define EOBJ_CHUNK_BONES 0x0913
#define EOBJ_CHUNK_SMOTIONS 0x0916
#define EOBJ_CHUNK_SURFACES_XRLC 0x0918
#define EOBJ_CHUNK_BONEPARTS 0x0919
#define EOBJ_CHUNK_ACTORTRANSFORM 0x0920
#define EOBJ_CHUNK_BONES2 0x0921
#define EOBJ_CHUNK_DESC 0x0922
#define EOBJ_CHUNK_BONEPARTS2 0x0923
#define EOBJ_CHUNK_SMOTIONS2 0x0924
#define EOBJ_CHUNK_LODS 0x0925
#define EOBJ_CHUNK_SMOTIONS3 0x0926
//----------------------------------------------------
