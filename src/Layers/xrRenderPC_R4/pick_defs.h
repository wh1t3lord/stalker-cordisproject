//----------------------------------------------------
// file: SceneClassList.h
//----------------------------------------------------
#pragma once
//----------------------------------------------------

#include "stdafx.h"

class CEditableObject;
class CEditableMesh;
class CCustomObject;

class SPickQuery
{
    Flags32 m_Flags;

public:
    Fvector m_Start;
    Fvector m_Direction;
    float m_Dist;
    Fbox m_BB;

public:
    class SResult
    {
    public:
        Fvector verts[3];
        float u, v;
        float range;
        CEditableObject* e_obj;
        CEditableMesh* e_mesh;
        //.            u32				tag;
        union
        {
            u32 tag; // 4b
            struct
            {
                u32 material : 14; //
                u32 suppress_shadows : 1; //
                u32 suppress_wm : 1; //
                u32 sector : 16; //
            };
        };

        bool operator<(const SResult& F) const { return range < F.range; }
        SResult(const SResult& F)
        {
            verts[0] = F.verts[0];
            verts[1] = F.verts[1];
            verts[2] = F.verts[2];
            u = F.u;
            v = F.v;
            range = F.range;
            tag = F.tag;
            e_obj = F.e_obj;
            e_mesh = F.e_mesh;
        }
        SResult(const Fmatrix& parent, CDB::RESULT* r, CEditableObject* obj, CEditableMesh* mesh)
        {
            parent.transform_tiny(verts[0], r->verts[0]);
            parent.transform_tiny(verts[1], r->verts[1]);
            parent.transform_tiny(verts[2], r->verts[2]);
            u = r->u;
            v = r->v;
            range = r->range;
            tag = r->dummy;
            e_obj = obj;
            e_mesh = mesh;
        }
        SResult(CDB::RESULT* r, CEditableObject* obj, CEditableMesh* mesh)
        {
            verts[0] = r->verts[0];
            verts[1] = r->verts[1];
            verts[2] = r->verts[2];
            u = r->u;
            v = r->v;
            range = r->range;
            tag = r->dummy;
            e_obj = obj;
            e_mesh = mesh;
        }
    };
  //  DEFINE_VECTOR(SResult, ResultVec, ResultIt);
    using ResultVec = xr_vector<SResult>;
    using ResultIt = ResultVec::iterator;

protected:
    ResultVec results;

public:
    inline void prepare_rq(const Fvector& start, const Fvector& dir, float dist, u32 flags)
    {
        m_Start.set(start);
        m_Direction.set(dir);
        m_Dist = dist;
        m_Flags.assign(flags);
        results.clear();
    }
    inline void prepare_bq(const Fbox& bbox, u32 flags)
    {
        m_BB.set(bbox);
        m_Flags.assign(flags);
        results.clear();
    }
    inline void append_mtx(const Fmatrix& parent, CDB::RESULT* R, CEditableObject* obj, CEditableMesh* mesh)
    {
        SResult D(parent, R, obj, mesh);
        if (m_Flags.is(CDB::OPT_ONLYNEAREST) && !results.empty())
        {
            SResult& S = results.back();
            if (D.range < S.range)
                S = D;
        }
        else
            results.push_back(D);
    }
    inline void append(CDB::RESULT* R, CEditableObject* obj, CEditableMesh* mesh)
    {
        SResult D(R, obj, mesh);
        if (m_Flags.is(CDB::OPT_ONLYNEAREST) && !results.empty())
        {
            SResult& S = results.back();
            if (D.range < S.range)
                S = D;
        }
        else
            results.push_back(D);
    }
    inline int r_count(void) { return results.size(); }
    inline SResult* r_begin(void) { return &*(results.begin()); } // Lord: Пока хз правильно ли или нет
    inline SResult* r_end(void) { return &*(results.end()); } // Lord: Пока хз правильно ли или нет
    inline void r_clear(void) { results.clear(); }
    inline void r_sort(void) { std::sort(results.begin(), results.end()); }
};

struct SRayPickObjVisualInfo
{
    u16 bone_id;
    Fvector normal;
    IKinematics* K;
};

struct SRayPickInfo
{
    CDB::RESULT inf;
    SRayPickObjVisualInfo visual_inf;
    CCustomObject* s_obj;
    CEditableObject* e_obj;
    CEditableMesh* e_mesh;
    Fvector pt;
    SRayPickInfo(void)
    {
        Reset();
        visual_inf.bone_id = u16(-1);
    }
    inline void Reset(void)
    {
        ZeroMemory(this, sizeof(SRayPickInfo));
        inf.range = 5000;
    }
    inline void SetRESULT(CDB::MODEL* M, CDB::RESULT* R)
    {
        inf = *R;
        inf.id = (M->get_tris() + inf.id)->dummy;
    }
};
//DEFINE_VECTOR(CDB::RESULT, BPInfVec, BPInfIt);
using BPInfVec = xr_vector<CDB::RESULT>;
using BPInfIt = BPInfVec::iterator;
struct SBoxPickInfo
{
    BPInfVec inf;
    CCustomObject* s_obj;
    CEditableObject* e_obj;
    CEditableMesh* e_mesh;
    SBoxPickInfo(void) { Reset(); }
    inline void Reset(void) { ZeroMemory(this, sizeof(SBoxPickInfo)); }
    inline void AddRESULT(CDB::MODEL* M, CDB::RESULT* R)
    {
        inf.push_back(*R);
        inf.back().id = (M->get_tris() + inf.back().id)->dummy;
    }
};
//DEFINE_VECTOR(SBoxPickInfo, SBoxPickInfoVec, SBoxPickInfoIt);
using SBoxPickInfoVec = xr_vector<SBoxPickInfo>;
using SBoxPickInfoIt = SBoxPickInfoVec::iterator;
//----------------------------------------------------
