#include "stdafx.h"
#pragma hdrstop

#include "EditMesh.h"
#include "EditObject.h"
#include "xrCore/Animation/Bone.hpp"
#include "utils/xrLC_Light/itterate_adjacents.h"
//#include "itterate_adjacents_dynamic.h"

#include "Utils/ETools/ETools.h"
//#include "UI_ToolsCustom.h"
#include "itterate_adjacents_dynamic.h"

#pragma region EditMesh
CEditableMesh::~CEditableMesh()
{
    Clear();
    R_ASSERT2(0 == m_RenderBuffers, "Render buffer still referenced.");
}

void CEditableMesh::Construct()
{
    m_Box.set(0, 0, 0, 0, 0, 0);
    m_Flags.assign(flVisible);
    m_Name = "";

    m_CFModel = 0;

    m_Vertices = 0;
    m_SmoothGroups = 0;
    m_Adjs = 0;
    m_Faces = 0;
    m_FaceNormals = 0;
    m_VertexNormals = 0;
    m_SVertices = 0;
    m_SVertInfl = 0;

    m_RenderBuffers = 0;

    m_FNormalsRefs = 0;
    m_VNormalsRefs = 0;
    m_AdjsRefs = 0;
    m_SVertRefs = 0;
}

void CEditableMesh::Clear()
{
    UnloadRenderBuffers();

    UnloadAdjacency();
    UnloadCForm();
    UnloadFNormals();
    UnloadVNormals();
    UnloadSVertices();

    VERIFY(m_FNormalsRefs == 0 && m_VNormalsRefs == 0 && m_AdjsRefs == 0 && m_SVertRefs == 0);

    xr_free(m_Vertices);
    xr_free(m_Faces);
    for (auto& vm_it : m_VMaps)
        xr_delete(vm_it);
    m_VMaps.clear();
    m_SurfFaces.clear();
    for (auto& ref_it : m_VMRefs)
        xr_free(ref_it.pts);
    m_VMRefs.clear();
}

void CEditableMesh::UnloadCForm()
{
    // ETOOLS::destroy_model(m_CFModel);
    xr_delete(m_CFModel);
}

void CEditableMesh::UnloadFNormals(bool force)
{
    m_FNormalsRefs--;
    if (force || m_FNormalsRefs <= 0)
    {
        xr_free(m_FaceNormals);
        m_FNormalsRefs = 0;
    }
}

void CEditableMesh::UnloadVNormals(bool force)
{
    m_VNormalsRefs--;
    if (force || m_VNormalsRefs <= 0)
    {
        xr_free(m_VertexNormals);
        m_VNormalsRefs = 0;
    }
}

void CEditableMesh::UnloadSVertices(bool force)
{
    m_SVertRefs--;
    if (force || m_SVertRefs <= 0)
    {
        xr_free(m_SVertices);
        m_SVertRefs = 0;
    }
}

void CEditableMesh::UnloadAdjacency(bool force)
{
    m_AdjsRefs--;
    if (force || m_AdjsRefs <= 0)
    {
        xr_delete(m_Adjs);
        m_AdjsRefs = 0;
    }
}

void CEditableMesh::RecomputeBBox()
{
    if (0 == m_VertCount)
    {
        m_Box.set(0, 0, 0, 0, 0, 0);
        return;
    }
    m_Box.set(m_Vertices[0], m_Vertices[0]);
    for (u32 k = 1; k < m_VertCount; k++)
        m_Box.modify(m_Vertices[k]);
}

void CEditableMesh::GenerateFNormals()
{
    m_FNormalsRefs++;
    if (m_FaceNormals)
        return;
    m_FaceNormals = xr_alloc<Fvector>(m_FaceCount);

    // face normals
    for (u32 k = 0; k < m_FaceCount; k++)
        m_FaceNormals[k].mknormal(m_Vertices[m_Faces[k].pv[0].pindex], m_Vertices[m_Faces[k].pv[1].pindex],
            m_Vertices[m_Faces[k].pv[2].pindex]);
}

BOOL CEditableMesh::m_bDraftMeshMode = FALSE;

void CEditableMesh::GenerateVNormals(const Fmatrix* parent_xform)
{
    m_VNormalsRefs++;
    if (m_VertexNormals)
        return;
    m_VertexNormals = xr_alloc<Fvector>(m_FaceCount * 3);

    // gen req
    GenerateFNormals();
    GenerateAdjacency();

    for (u32 f_i = 0; f_i < m_FaceCount; f_i++)
    {
        for (int k = 0; k < 3; k++)
        {
            Fvector& N = m_VertexNormals[f_i * 3 + k];
            IntVec& a_lst = (*m_Adjs)[m_Faces[f_i].pv[k].pindex];

            IntIt face_adj_it = std::find(a_lst.begin(), a_lst.end(), f_i);
            VERIFY(face_adj_it != a_lst.end());
            //
            N.set(m_FaceNormals[a_lst.front()]);
            if (m_bDraftMeshMode)
                continue;

            typedef itterate_adjacents<itterate_adjacents_params_dynamic<st_FaceVert>> iterate_adj;
            iterate_adj::recurse_tri_params p(N, m_SmoothGroups, m_FaceNormals, a_lst, m_Faces, m_FaceCount);
            iterate_adj::RecurseTri(face_adj_it - a_lst.begin(), p);
            float len = N.magnitude();
            if (len > EPS_S)
            {
                N.div(len);
            }
            else
            {
                //.                	Msg		("!Invalid smooth group found (Maya type). Object: '%s'. Vertex: [%3.2f,
                //%3.2f, %3.2f]",m_Parent->GetName(),VPUSH(m_Vertices[m_Faces[f_i].pv[k].pindex]));

                if (parent_xform)
                {
                    Fvector p0;
                    parent_xform->transform_tiny(p0, m_Vertices[m_Faces[f_i].pv[k].pindex]);
                    //    Tools->m_DebugDraw.AppendPoint(p0, 0xffff0000, true, true, "invalid vNORMAL"); Lord: Interpret
                }

                N.set(m_FaceNormals[a_lst.front()]);
            }
        }
    }

    UnloadFNormals();
    UnloadAdjacency();
}

/*
void CEditableMesh::GenerateVNormals(const Fmatrix* parent_xform)
{
    m_VNormalsRefs++;
    if (m_VertexNormals)		return;
    m_VertexNormals				= xr_alloc<Fvector>(m_FaceCount*3);

    // gen req
    GenerateFNormals	();
    GenerateAdjacency	();

    // vertex normals
    if (m_Flags.is(flSGMask))
    {
        for (u32 f_i=0; f_i<m_FaceCount; f_i++ )
        {
            u32 sg				= m_SmoothGroups[f_i];
            Fvector& FN 		= m_FaceNormals[f_i];
            for (int k=0; k<3; k++)
            {
                Fvector& N 	= m_VertexNormals[f_i*3+k];
                IntVec& a_lst=(*m_Adjs)[m_Faces[f_i].pv[k].pindex];
//
                typedef itterate_adjacents< itterate_adjacents_params_dynamic<st_FaceVert> > iterate_adj ;
                iterate_adj::recurse_tri_params p( N, m_SmoothGroups, m_FaceNormals, a_lst, m_Faces, m_FaceCount );
                iterate_adj::RecurseTri( 0, p );
//
                if (sg)
                {
                    N.set		(0,0,0);

                    VERIFY(a_lst.size());
                    for (IntIt i_it=a_lst.begin(); i_it!=a_lst.end(); i_it++)
                        if (sg&m_SmoothGroups[*i_it])
                            N.add	(m_FaceNormals[*i_it]);

                    float len 	= N.magnitude();
                    if (len>EPS_S)
                    {
                        N.div	(len);
                    }else
                    {
//.                    	Msg		("!Invalid smooth group found (MAX type). Object: '%s'. Vertex: [%3.2f, %3.2f,
%3.2f]",m_Parent->GetName(),VPUSH(m_Vertices[m_Faces[f_i].pv[k].pindex]));
#ifdef _EDITOR
                        Fvector p0;
                        p0 = m_Vertices[m_Faces[f_i].pv[k].pindex];
                        Tools->m_DebugDraw.AppendPoint(p0, 0xffff0000, true, true, "invalid vNORMAL");
#endif
                        N.set	(m_FaceNormals[a_lst.front()]);
                    }
                }else
                {
                    N.set		(FN);
                }
            }
        }
    }else{
        for (u32 f_i=0; f_i<m_FaceCount; f_i++ )
        {
            u32 sg			= m_SmoothGroups[f_i];
            Fvector& FN 	= m_FaceNormals[f_i];
            for (int k=0; k<3; k++)
            {
                Fvector& N 	= m_VertexNormals[f_i*3+k];
                if (sg!=-1)
                {
                    N.set		(0,0,0);
                    IntVec& a_lst=(*m_Adjs)[m_Faces[f_i].pv[k].pindex];
//
                typedef itterate_adjacents< itterate_adjacents_params_dynamic<st_FaceVert> > iterate_adj ;
                iterate_adj::recurse_tri_params p( N, m_SmoothGroups, m_FaceNormals, a_lst, m_Faces, m_FaceCount );
                iterate_adj::RecurseTri( 0, p );
//
                    VERIFY		(a_lst.size());
                    for (IntIt i_it=a_lst.begin(); i_it!=a_lst.end(); i_it++)
                    {
                        if (sg != m_SmoothGroups[*i_it])
                            continue;
                        N.add	(m_FaceNormals[*i_it]);
                    }
                    float len 	= N.magnitude();
                    if (len>EPS_S)
                    {
                        N.div	(len);
                    }else
                    {
//.                    	Msg		("!Invalid smooth group found (Maya type). Object: '%s'. Vertex: [%3.2f, %3.2f,
%3.2f]",m_Parent->GetName(),VPUSH(m_Vertices[m_Faces[f_i].pv[k].pindex]));

#ifdef _EDITOR
                        if(parent_xform)
                        {
                        Fvector p0;
                          parent_xform->transform_tiny(p0, m_Vertices[m_Faces[f_i].pv[k].pindex]);
                        Tools->m_DebugDraw.AppendPoint(p0, 0xffff0000, true, true, "invalid vNORMAL");
                        }
#endif

                        N.set	(m_FaceNormals[a_lst.front()]);
                    }
                }
                else
                {
                    N.set		(FN);

                    //IntVec& a_lst=(*m_Adjs)[m_Faces[f_i].pv[k].pindex];
                    //VERIFY(a_lst.size());
                    //for (IntIt i_it=a_lst.begin(); i_it!=a_lst.end(); i_it++)
                    //	N.add	(m_FNormals[*i_it]);
     //               float len 	= N.magnitude();
     //               if (len>EPS_S){
        //                N.div	(len);
     //               }else{
     //               	Msg		("!Invalid smooth group found (No smooth). Object: '%s'. Vertex: [%3.2f, %3.2f,
%3.2f]",m_Parent->GetName(),VPUSH(m_Verts[m_Faces[f_i].pv[k].pindex]));
     //                   N.set	(m_FNormals[a_lst.front()]);
     //               }

                }
            }
        }
    }
    UnloadFNormals		();
    UnloadAdjacency		();
}
*/
void CEditableMesh::GenerateSVertices(u32 influence)
{
    if (!m_Parent->IsSkeleton())
        return;

    m_SVertRefs++;
    if (m_SVertInfl != influence)
        UnloadSVertices(true);
    if (m_SVertices)
        return;
    m_SVertices = xr_alloc<st_SVert>(m_FaceCount * 3);
    m_SVertInfl = influence;

    //	CSMotion* active_motion=m_Parent->ResetSAnimation();
    m_Parent->CalculateAnimation(0);

    // generate normals
    GenerateFNormals();
    GenerateVNormals(0);

    for (u32 f_id = 0; f_id < m_FaceCount; f_id++)
    {
        st_Face& F = m_Faces[f_id];

        for (int k = 0; k < 3; ++k)
        {
            st_SVert& SV = m_SVertices[f_id * 3 + k];
            const Fvector& N = m_VertexNormals[f_id * 3 + k];
            const st_FaceVert& fv = F.pv[k];
            const Fvector& P = m_Vertices[fv.pindex];

            const st_VMapPtLst& vmpt_lst = m_VMRefs[fv.vmref];

            st_VertexWB wb;
            for (u8 vmpt_id = 0; vmpt_id != vmpt_lst.count; ++vmpt_id)
            {
                const st_VMap& VM = *m_VMaps[vmpt_lst.pts[vmpt_id].vmap_index];
                if (VM.type == vmtWeight)
                {
                    wb.push_back(
                        st_WB(m_Parent->GetBoneIndexByWMap(VM.name.c_str()), VM.getW(vmpt_lst.pts[vmpt_id].index)));

                    if (wb.back().bone == BI_NONE)
                    {
                        //   ELog.DlgMsg(mtError, "Can't find bone assigned to weight map %s", *VM.name); Lord:
                        //   Interpret
                        FATAL("Editor crashed."); // Lord: подумать как это можно было обойти бы
                        return;
                    }
                }
                else if (VM.type == vmtUV)
                    SV.uv.set(VM.getUV(vmpt_lst.pts[vmpt_id].index));
            }

            VERIFY(m_SVertInfl <= 4);

            wb.prepare_weights(m_SVertInfl);

            SV.offs = P;
            SV.norm = N;
            SV.bones.resize(wb.size());
            for (u8 k = 0; k < (u8)SV.bones.size(); k++)
            {
                SV.bones[k].id = wb[k].bone;
                SV.bones[k].w = wb[k].weight;
            }
        }
    }

    // restore active motion
    UnloadFNormals();
    UnloadVNormals();
}

void CEditableMesh::GenerateAdjacency()
{
    m_AdjsRefs++;
    if (m_Adjs)
        return;
    m_Adjs = new AdjVec();
    VERIFY(m_Faces);
    m_Adjs->resize(m_VertCount);
    //.	Log				(".. Update adjacency");
    for (u32 f_id = 0; f_id < m_FaceCount; f_id++)
        for (int k = 0; k < 3; k++)
            (*m_Adjs)[m_Faces[f_id].pv[k].pindex].push_back(f_id);
}

CSurface* CEditableMesh::GetSurfaceByFaceID(u32 fid)
{
    R_ASSERT(fid < m_FaceCount);
    for (auto& it : m_SurfFaces)
    {
        IntVec& face_lst = it.second;
        IntIt f_it = std::lower_bound(face_lst.begin(), face_lst.end(), (int)fid);
        if (f_it != face_lst.end() && *f_it == (int)fid)
            return it.first;
        //.		if (std::find(face_lst.begin(),face_lst.end(),fid)!=face_lst.end()) return sp_it->first;
    }
    return 0;
}

void CEditableMesh::GetFaceTC(u32 fid, const Fvector2* tc[3])
{
    R_ASSERT(fid < m_FaceCount);
    st_Face& F = m_Faces[fid];
    for (int k = 0; k < 3; k++)
    {
        st_VMapPt& vmr = m_VMRefs[F.pv[k].vmref].pts[0];
        tc[k] = &(m_VMaps[vmr.vmap_index]->getUV(vmr.index));
    }
}

void CEditableMesh::GetFacePT(u32 fid, const Fvector* pt[3])
{
    R_ASSERT(fid < m_FaceCount);
    st_Face& F = m_Faces[fid];

    for (int k = 0; k < 3; ++k)
        pt[k] = &m_Vertices[F.pv[k].pindex];
}

int CEditableMesh::GetFaceCount(bool bMatch2Sided, bool bIgnoreOCC)
{
    int f_cnt = 0;
    for (auto& it : m_SurfFaces)
    {
        CSurface* S = it.first;
        if (S->m_GameMtlName == "materials\\occ" && bIgnoreOCC)
            continue;

        if (bMatch2Sided)
        {
            if (S->m_Flags.is(CSurface::sf2Sided))
                f_cnt += it.second.size() * 2;
            else
                f_cnt += it.second.size();
        }
        else
        {
            f_cnt += it.second.size();
        }
    }
    return f_cnt;
}

float CEditableMesh::CalculateSurfaceArea(CSurface* surf, bool bMatch2Sided)
{
    auto sp_it = m_SurfFaces.find(surf);
    if (sp_it == m_SurfFaces.end())
        return 0;
    float area = 0;
    IntVec& pol_lst = sp_it->second;
    for (int k = 0; k < int(pol_lst.size()); k++)
    {
        st_Face& F = m_Faces[pol_lst[k]];
        Fvector c, e01, e02;
        e01.sub(m_Vertices[F.pv[1].pindex], m_Vertices[F.pv[0].pindex]);
        e02.sub(m_Vertices[F.pv[2].pindex], m_Vertices[F.pv[0].pindex]);
        area += c.crossproduct(e01, e02).magnitude() / 2.f;
    }
    if (bMatch2Sided && sp_it->first->m_Flags.is(CSurface::sf2Sided))
        area *= 2;
    return area;
}

float CEditableMesh::CalculateSurfacePixelArea(CSurface* surf, bool bMatch2Sided)
{
    auto sp_it = m_SurfFaces.find(surf);
    if (sp_it == m_SurfFaces.end())
        return 0;
    float area = 0;
    IntVec& pol_lst = sp_it->second;
    for (int k = 0; k < int(pol_lst.size()); k++)
    {
        //		st_Face& F		= m_Faces[pol_lst[k]];
        Fvector c, e01, e02;
        const Fvector2* tc[3];
        GetFaceTC(pol_lst[k], tc);
        e01.sub(Fvector().set(tc[1]->x, tc[1]->y, 0), Fvector().set(tc[0]->x, tc[0]->y, 0));
        e02.sub(Fvector().set(tc[2]->x, tc[2]->y, 0), Fvector().set(tc[0]->x, tc[0]->y, 0));
        area += c.crossproduct(e01, e02).magnitude() / 2.f;
    }
    if (bMatch2Sided && sp_it->first->m_Flags.is(CSurface::sf2Sided))
        area *= 2;
    return area;
}

int CEditableMesh::GetSurfFaceCount(CSurface* surf, bool bMatch2Sided)
{
    auto sp_it = m_SurfFaces.find(surf);
    if (sp_it == m_SurfFaces.end())
        return 0;
    int f_cnt = sp_it->second.size();
    if (bMatch2Sided && sp_it->first->m_Flags.is(CSurface::sf2Sided))
        f_cnt *= 2;
    return f_cnt;
}

void CEditableMesh::DumpAdjacency()
{
    Log("Adjacency dump.");
    Log("------------------------------------------------------------------------");
    /*    for (u32 i=0; i<m_Adjs.size(); i++){
            IntVec& a_lst	= m_Adjs[i];
            AnsiString s; s = "Point "; s+=AnsiString(i); s+=":";
            AnsiString s1;
            for (u32 j=0; j<a_lst.size(); j++){ s1=a_lst[j]; s+=" "+s1; }
            Log(s.c_str());
        }
    */
}

//----------------------------------------------------------------------------

int CEditableMesh::FindVMapByName(VMapVec& vmaps, const char* name, u8 t, bool polymap)
{
    for (auto vm_it = vmaps.begin(); vm_it != vmaps.end(); vm_it++)
    {
        if ((*vm_it)->type == t && xr_stricmp((*vm_it)->name.c_str(), name) == 0 && polymap == (*vm_it)->polymap)
            return vm_it - vmaps.begin();
    }
    return -1;
}

//----------------------------------------------------------------------------

bool CEditableMesh::Validate() { return true; }
//----------------------------------------------------------------------------
#pragma endregion

#pragma region EditMeshIO

#define EMESH_CURRENT_VERSION 0x0011
//----------------------------------------------------
#define EMESH_CHUNK_VERSION 0x1000
#define EMESH_CHUNK_MESHNAME 0x1001
#define EMESH_CHUNK_FLAGS 0x1002
#define EMESH_CHUNK_NOT_USED_0 0x1003
#define EMESH_CHUNK_BBOX 0x1004
#define EMESH_CHUNK_VERTS 0x1005
#define EMESH_CHUNK_FACES 0x1006
#define EMESH_CHUNK_VMAPS_0 0x1007
#define EMESH_CHUNK_VMREFS 0x1008
#define EMESH_CHUNK_SFACE 0x1009
#define EMESH_CHUNK_BOP 0x1010
#define EMESH_CHUNK_VMAPS_1 0x1011
#define EMESH_CHUNK_VMAPS_2 0x1012
#define EMESH_CHUNK_SG 0x1013

void CEditableMesh::SaveMesh(IWriter& F)
{
    F.open_chunk(EMESH_CHUNK_VERSION);
    F.w_u16(EMESH_CURRENT_VERSION);
    F.close_chunk();

    F.open_chunk(EMESH_CHUNK_MESHNAME);
    F.w_stringZ(m_Name);
    F.close_chunk();

    F.w_chunk(EMESH_CHUNK_BBOX, &m_Box, sizeof(m_Box));
    F.w_chunk(EMESH_CHUNK_FLAGS, &m_Flags, 1);
    F.w_chunk(EMESH_CHUNK_BOP, &m_Ops, sizeof(m_Ops));

    F.open_chunk(EMESH_CHUNK_VERTS);
    F.w_u32(m_VertCount);
    F.w(m_Vertices, m_VertCount * sizeof(Fvector));

    F.close_chunk();

    F.open_chunk(EMESH_CHUNK_FACES);
    F.w_u32(m_FaceCount);
    F.w(m_Faces, m_FaceCount * sizeof(st_Face));
    F.close_chunk();

    if (GetSmoothGroups())
    {
        F.open_chunk(EMESH_CHUNK_SG);
        F.w(GetSmoothGroups(), m_FaceCount * sizeof(u32));
        F.close_chunk();
    }

    F.open_chunk(EMESH_CHUNK_VMREFS);
    F.w_u32(m_VMRefs.size());
    for (auto& it : m_VMRefs)
    {
        int sz = it.count;
        VERIFY(sz <= 255);
        F.w_u8((u8)sz);
        F.w(it.pts, sizeof(st_VMapPt) * sz);
    }
    F.close_chunk();

    F.open_chunk(EMESH_CHUNK_SFACE);
    F.w_u16((u16)m_SurfFaces.size()); /* surface polygon count*/
    for (std::pair<CSurface* const, IntVec>& it : m_SurfFaces)
    {
        F.w_stringZ(it.first->_Name()); /* surface name*/
        IntVec& pol_lst = it.second;
        F.w_u32(pol_lst.size()); /* surface-polygon indices*/
        F.w(&*pol_lst.begin(), sizeof(int) * pol_lst.size());
    }
    F.close_chunk();

    F.open_chunk(EMESH_CHUNK_VMAPS_2);
    F.w_u32(m_VMaps.size());
    for (st_VMap*& it : m_VMaps)
    {
        F.w_stringZ(it->name);
        F.w_u8(it->dim);
        F.w_u8(it->polymap);
        F.w_u8(it->type);
        F.w_u32(it->size());
        F.w(it->getVMdata(), it->VMdatasize());
        F.w(it->getVIdata(), it->VIdatasize());
        if (it->polymap)
            F.w(it->getPIdata(), it->PIdatasize());
    }
    F.close_chunk();
}

bool CEditableMesh::LoadMesh(IReader& F)
{
    u32 version = 0;

    R_ASSERT(F.r_chunk(EMESH_CHUNK_VERSION, &version));
    if (version != EMESH_CURRENT_VERSION)
    {
        // ELog.DlgMsg(mtError, "CEditableMesh: unsupported file version. Mesh can't load."); Lord: Interpret
        return false;
    }

    R_ASSERT(F.find_chunk(EMESH_CHUNK_MESHNAME));
    F.r_stringZ(m_Name);

    R_ASSERT(F.r_chunk(EMESH_CHUNK_BBOX, &m_Box));
    R_ASSERT(F.r_chunk(EMESH_CHUNK_FLAGS, &m_Flags));
    F.r_chunk(EMESH_CHUNK_BOP, &m_Ops);

    R_ASSERT(F.find_chunk(EMESH_CHUNK_VERTS));
    m_VertCount = F.r_u32();
    if (m_VertCount < 3)
    {
        Log("!CEditableMesh: Vertices<3.");
        return false;
    }
    m_Vertices = xr_alloc<Fvector>(m_VertCount);
    F.r(m_Vertices, m_VertCount * sizeof(Fvector));

    R_ASSERT(F.find_chunk(EMESH_CHUNK_FACES));
    m_FaceCount = F.r_u32();
    m_Faces = xr_alloc<st_Face>(m_FaceCount);
    if (m_FaceCount == 0)
    {
        Log("!CEditableMesh: Faces==0.");
        return false;
    }
    F.r(m_Faces, m_FaceCount * sizeof(st_Face));

    m_SmoothGroups = xr_alloc<u32>(m_FaceCount);
    MemFill32(m_SmoothGroups, m_Flags.is(flSGMask) ? 0 : u32(-1), m_FaceCount);
    u32 sg_chunk_size = F.find_chunk(EMESH_CHUNK_SG);
    if (sg_chunk_size)
    {
        VERIFY(m_FaceCount * sizeof(u32) == sg_chunk_size);
        F.r(m_SmoothGroups, m_FaceCount * sizeof(u32));
    }

    R_ASSERT(F.find_chunk(EMESH_CHUNK_VMREFS));
    m_VMRefs.resize(F.r_u32());
    int sz_vmpt = sizeof(st_VMapPt);
    for (auto& it : m_VMRefs)
    {
        it.count = F.r_u8();
        it.pts = xr_alloc<st_VMapPt>(it.count);
        F.r(it.pts, sz_vmpt * it.count);
    }

    R_ASSERT(F.find_chunk(EMESH_CHUNK_SFACE));
    string128 surf_name;
    u32 sface_cnt = F.r_u16(); // surface-face count
    for (u32 sp_i = 0; sp_i < sface_cnt; sp_i++)
    {
        F.r_stringZ(surf_name, sizeof(surf_name));
        int surf_id;
        CSurface* surf = m_Parent->FindSurfaceByName(surf_name, &surf_id);
        VERIFY(surf);
        IntVec& face_lst = m_SurfFaces[surf];
        face_lst.resize(F.r_u32());
        if (face_lst.empty())
        {
            Log("!Empty surface found: %s", surf->_Name());
            return false;
        }
        F.r(&*face_lst.begin(), face_lst.size() * sizeof(int));
        std::sort(face_lst.begin(), face_lst.end());
    }

    if (F.find_chunk(EMESH_CHUNK_VMAPS_2))
    {
        m_VMaps.resize(F.r_u32());
        for (auto vm_it = m_VMaps.begin(); vm_it != m_VMaps.end(); ++vm_it)
        {
            *vm_it = new st_VMap();
            F.r_stringZ((*vm_it)->name);
            (*vm_it)->dim = F.r_u8();
            (*vm_it)->polymap = F.r_u8();
            (*vm_it)->type = F.r_u8();
            (*vm_it)->resize(F.r_u32());
            F.r((*vm_it)->getVMdata(), (*vm_it)->VMdatasize());
            F.r((*vm_it)->getVIdata(), (*vm_it)->VIdatasize());
            if ((*vm_it)->polymap)
                F.r((*vm_it)->getPIdata(), (*vm_it)->PIdatasize());
        }
    }
    else
    {
        if (F.find_chunk(EMESH_CHUNK_VMAPS_1))
        {
            m_VMaps.resize(F.r_u32());
            for (auto vm_it = m_VMaps.begin(); vm_it != m_VMaps.end(); ++vm_it)
            {
                *vm_it = new st_VMap();
                F.r_stringZ((*vm_it)->name);
                (*vm_it)->dim = F.r_u8();
                (*vm_it)->type = F.r_u8();
                (*vm_it)->resize(F.r_u32());
                F.r((*vm_it)->getVMdata(), (*vm_it)->VMdatasize());
            }
        }
        else
        {
            R_ASSERT(F.find_chunk(EMESH_CHUNK_VMAPS_0));
            m_VMaps.resize(F.r_u32());
            for (auto vm_it = m_VMaps.begin(); vm_it != m_VMaps.end(); ++vm_it)
            {
                *vm_it = new st_VMap();
                F.r_stringZ((*vm_it)->name);
                (*vm_it)->dim = 2;
                (*vm_it)->type = vmtUV;
                (*vm_it)->resize(F.r_u32());
                F.r((*vm_it)->getVMdata(), (*vm_it)->VMdatasize());
            }
        }
        // update vmaps
        RebuildVMaps();
    }
    /*
    Lord: EPrefs по сути настройка сцены как у нас view, лист объектов и так далее, главное здесь object_flags,
    Interpret данный класс и только потом уже тестировать
    
    if (!EPrefs->object_flags.is(epoDeffLoadRB))
    {
        GenerateFNormals();
        GenerateAdjacency();
        GenerateVNormals(0);
        GenerateRenderBuffers();
        UnloadFNormals();
        UnloadAdjacency();
        UnloadVNormals();
    }

    if (!EPrefs->object_flags.is(epoDeffLoadCF))
        GenerateCFModel();
    */

//    OptimizeMesh(false);
 //   RebuildVMaps();

    return true;
}
#pragma endregion

#pragma region EditMeshPick

static IntVec sml_processed;
static Fvector sml_normal;
static float m_fSoftAngle;

//----------------------------------------------------

//----------------------------------------------------
// номер face должен соответствовать списку
//----------------------------------------------------
void CEditableMesh::GenerateCFModel()
{
    UnloadCForm();
    // Collect faces
    CDB::Collector* CL = ETOOLS::create_collector();
    // double sided
    for (SurfFaces::iterator sp_it = m_SurfFaces.begin(); sp_it != m_SurfFaces.end(); sp_it++)
    {
        IntVec& face_lst = sp_it->second;
        for (IntIt it = face_lst.begin(); it != face_lst.end(); it++)
        {
            st_Face& F = m_Faces[*it];
            ETOOLS::collector_add_face_d(
                CL, m_Vertices[F.pv[0].pindex], m_Vertices[F.pv[1].pindex], m_Vertices[F.pv[2].pindex], *it);
            if (sp_it->first->m_Flags.is(CSurface::sf2Sided))
                ETOOLS::collector_add_face_d(
                    CL, m_Vertices[F.pv[2].pindex], m_Vertices[F.pv[1].pindex], m_Vertices[F.pv[0].pindex], *it);
        }
    }
    m_CFModel = ETOOLS::create_model_cl(CL);
    ETOOLS::destroy_collector(CL);
}

void CEditableMesh::RayQuery(SPickQuery& pinf)
{
    if (!m_CFModel)
        GenerateCFModel();
    //*
    ETOOLS::ray_query(m_CFModel, pinf.m_Start, pinf.m_Direction, pinf.m_Dist);
    for (int r = 0; r < ETOOLS::r_count(); r++)
        pinf.append(ETOOLS::r_begin() + r, m_Parent, this);
    /*
        XRC.ray_query	(m_CFModel, pinf.m_Start, pinf.m_Direction, pinf.m_Dist);
        for (int r=0; r<XRC.r_count(); r++)
            pinf.append	(XRC.r_begin()+r,m_Parent,this);
    //*/
}

void CEditableMesh::RayQuery(const Fmatrix& parent, const Fmatrix& inv_parent, SPickQuery& pinf)
{
    if (!m_CFModel)
        GenerateCFModel();
    //*
    ETOOLS::ray_query_m(inv_parent, m_CFModel, pinf.m_Start, pinf.m_Direction, pinf.m_Dist);
    for (int r = 0; r < ETOOLS::r_count(); r++)
        pinf.append_mtx(parent, ETOOLS::r_begin() + r, m_Parent, this);
    /*
        XRC.ray_query	(inv_parent, m_CFModel, pinf.m_Start, pinf.m_Direction, pinf.m_Dist);
        for (int r=0; r<XRC.r_count(); r++)
            pinf.append_mtx(parent,XRC.r_begin()+r,m_Parent,this);
    //*/
}

void CEditableMesh::BoxQuery(const Fmatrix& parent, const Fmatrix& inv_parent, SPickQuery& pinf)
{
    if (!m_CFModel)
        GenerateCFModel();
    ETOOLS::box_query_m(inv_parent, m_CFModel, pinf.m_BB);
    for (int r = 0; r < ETOOLS::r_count(); r++)
        pinf.append_mtx(parent, ETOOLS::r_begin() + r, m_Parent, this);
}

static const float _sqrt_flt_max = _sqrt(flt_max * 0.5f);

bool CEditableMesh::RayPick(
    float& distance, const Fvector& start, const Fvector& direction, const Fmatrix& inv_parent, SRayPickInfo* pinf)
{
    if (!m_Flags.is(flVisible))
        return false;

    if (!m_CFModel)
        GenerateCFModel();
    //.	float m_r 		= pinf?pinf->inf.range+EPS_L:UI->ZFar();// (bugs: не всегда выбирает) //S ????

    ETOOLS::ray_options(CDB::OPT_ONLYNEAREST | CDB::OPT_CULL);
    ETOOLS::ray_query_m(inv_parent, m_CFModel, start, direction, _sqrt_flt_max);

    if (ETOOLS::r_count())
    {
        CDB::RESULT* I = ETOOLS::r_begin();
        if (I->range < distance)
        {
            if (pinf)
            {
                pinf->SetRESULT(m_CFModel, I);
                pinf->e_obj = m_Parent;
                pinf->e_mesh = this;
                pinf->pt.mul(direction, pinf->inf.range);
                pinf->pt.add(start);
            }
            distance = I->range;
            return true;
        }
    }
    return false;
}

//----------------------------------------------------

bool CEditableMesh::CHullPickMesh(PlaneVec& pl, const Fmatrix& parent)
{
    Fvector p;
    boolVec inside(m_VertCount, true);
    for (u32 v_id = 0; v_id < m_VertCount; v_id++)
    {
        parent.transform_tiny(p, m_Vertices[v_id]);
        for (PlaneIt p_it = pl.begin(); p_it != pl.end(); p_it++)
            if (p_it->classify(p) > EPS_L)
            {
                inside[v_id] = false;
                break;
            }
    }
    for (u32 f_id = 0; f_id < m_FaceCount; f_id++)
        if (inside[m_Faces[f_id].pv[0].pindex] && inside[m_Faces[f_id].pv[1].pindex] &&
            inside[m_Faces[f_id].pv[2].pindex])
            return true;
    return false;
}
//----------------------------------------------------

void CEditableMesh::RecurseTri(int id)
{
    // Check if triangle already processed
    if (std::find(sml_processed.begin(), sml_processed.end(), id) != sml_processed.end())
        return;

    sml_processed.push_back(id);

    // recurse
    for (int k = 0; k < 3; k++)
    {
        IntVec& PL = (*m_Adjs)[m_Faces[id].pv[k].pindex];
        for (IntIt pl_it = PL.begin(); pl_it != PL.end(); pl_it++)
        {
            Fvector& test_normal = m_FaceNormals[*pl_it];
            float cosa = test_normal.dotproduct(sml_normal);
            if (cosa < m_fSoftAngle)
                continue;
            RecurseTri(*pl_it);
        }
    }
}
//----------------------------------------------------

void CEditableMesh::GetTiesFaces(int start_id, U32Vec& fl, float fSoftAngle, bool bRecursive)
{
    R_ASSERT(start_id < int(m_FaceCount));
    m_fSoftAngle = cosf(deg2rad(fSoftAngle));
    GenerateFNormals();
    GenerateAdjacency();
    VERIFY(m_FaceNormals);
    if (bRecursive)
    {
        sml_processed.clear();
        sml_normal.set(m_FaceNormals[start_id]);
        RecurseTri(start_id);
        fl.insert(fl.begin(), sml_processed.begin(), sml_processed.end());
    }
    else
    {
        for (int k = 0; k < 3; k++)
            fl.insert(fl.end(), (*m_Adjs)[m_Faces[start_id].pv[k].pindex].begin(),
                (*m_Adjs)[m_Faces[start_id].pv[k].pindex].end());
        std::sort(fl.begin(), fl.end());
        fl.erase(std::unique(fl.begin(), fl.end()), fl.end());
    }
    UnloadFNormals();
    UnloadAdjacency();
}
//----------------------------------------------------

bool CEditableMesh::BoxPick(const Fbox& box, const Fmatrix& inv_parent, SBoxPickInfoVec& pinf)
{
    if (!m_CFModel)
        GenerateCFModel();

    ETOOLS::box_query_m(inv_parent, m_CFModel, box);
    if (ETOOLS::r_count())
    {
        pinf.push_back(SBoxPickInfo());
        pinf.back().e_obj = m_Parent;
        pinf.back().e_mesh = this;

        for (CDB::RESULT* I = ETOOLS::r_begin(); I != ETOOLS::r_end(); I++)
            pinf.back().AddRESULT(m_CFModel, I);
        return true;
    }
    return false;
}
//----------------------------------------------------

bool CEditableMesh::FrustumPick(const CFrustum& frustum, const Fmatrix& parent)
{
    if (!m_Flags.is(flVisible))
        return false;

    Fvector p[3];
    for (u32 i = 0; i < m_FaceCount; i++)
    {
        for (int k = 0; k < 3; k++)
            parent.transform_tiny(p[k], m_Vertices[m_Faces[i].pv[k].pindex]);
        if (frustum.testPolyInside(p, 3))
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------

void CEditableMesh::FrustumPickFaces(const CFrustum& frustum, const Fmatrix& parent, U32Vec& fl)
{
    if (!m_Flags.is(flVisible))
        return;

    Fvector p[3];
    // bool bCulling = EPrefs->bp_cull; // Lord: Interpret EPrefs
    bool bCulling = false;
    for (u32 p_id = 0; p_id < m_FaceCount; p_id++)
    {
        for (int k = 0; k < 3; ++k)
            parent.transform_tiny(p[k], m_Vertices[m_Faces[p_id].pv[k].pindex]);

        if (bCulling)
        {
            Fplane P;
            P.build(p[0], p[1], p[2]);
            //     if (P.classify(EDevice.m_Camera.GetPosition()) < 0) Lord: check
            if (P.classify(Device.vCameraPosition) < 0)
                continue;
        }
        if (frustum.testPolyInside(p, 3))
            fl.push_back(p_id);
    }
}
#pragma endregion

#pragma region EditMeshModify

//----------------------------------------------------
void CEditableMesh::Transform(const Fmatrix& parent)
{
    // transform position
    for (u32 k = 0; k < m_VertCount; ++k)
        parent.transform_tiny(m_Vertices[k]);

    // RecomputeBBox
    RecomputeBBox();
    // update normals & cform

    UnloadRenderBuffers();
    UnloadCForm();

    UnloadFNormals(true);
    UnloadVNormals(true);
    UnloadSVertices(true);
}

//----------------------------------------------------

int CEditableMesh::FindSimilarUV(st_VMap* vmap, Fvector2& _uv)
{
    int sz = vmap->size();
    for (int k = 0; k < sz; ++k)
    {
        const Fvector2& uv = vmap->getUV(k);
        if (uv.similar(_uv))
            return k;
    }
    return -1;
}

int CEditableMesh::FindSimilarWeight(st_VMap* vmap, float _w)
{
    int sz = vmap->size();
    for (int k = 0; k < sz; ++k)
    {
        float w = vmap->getW(k);
        if (fsimilar(w, _w))
            return k;
    }
    return -1;
}

void CEditableMesh::RebuildVMaps()
{
    //.	Log			("Rebuilding VMaps...");
    IntVec m_VertVMap;
    m_VertVMap.resize(m_VertCount, -1);
    VMapVec nVMaps;
    VMRefsVec nVMRefs;
    // refs copy to new
    {
        nVMRefs.resize(m_VMRefs.size());
        for (auto& it : m_VMRefs)
        {
            it.count = it.count;
            it.pts = xr_alloc<st_VMapPt>(it.count);
        }
    }

    for (u32 f_id = 0; f_id < m_FaceCount; f_id++)
    {
        st_Face& F = m_Faces[f_id];
        for (int k = 0; k < 3; k++)
        {
            u32 pts_cnt = m_VMRefs[F.pv[k].vmref].count;
            for (u32 pt_id = 0; pt_id < pts_cnt; pt_id++)
            {
                st_VMapPt* n_pt_it = &nVMRefs[F.pv[k].vmref].pts[pt_id];
                st_VMapPt* o_pt_it = &m_VMRefs[F.pv[k].vmref].pts[pt_id];
                st_VMap* vmap = m_VMaps[o_pt_it->vmap_index];
                switch (vmap->type)
                {
                case vmtUV:
                {
                    int& pm = m_VertVMap[F.pv[k].pindex];
                    if (-1 == pm)
                    { // point map
                        pm = F.pv[k].vmref;
                        int vm_idx = FindVMapByName(nVMaps, vmap->name.c_str(), vmap->type, false);
                        if (-1 == vm_idx)
                        {
                            nVMaps.push_back(new st_VMap(vmap->name.c_str(), vmap->type, false));
                            vm_idx = nVMaps.size() - 1;
                        }
                        st_VMap* nVMap = nVMaps[vm_idx];

                        //						int uv_idx = FindSimilarUV(nVMap,vmap->getUV(pt_it->index));
                        //						if (uv_idx==-1){
                        //							uv_idx	= nVMap->size();
                        //							nVMap->appendUV(vmap->getUV(pt_it->index));
                        //							nVMap->appendVI(F.pv[k].pindex);
                        //						}

                        nVMap->appendUV(vmap->getUV(o_pt_it->index));
                        nVMap->appendVI(F.pv[k].pindex);
                        n_pt_it->index = nVMap->size() - 1;
                        n_pt_it->vmap_index = vm_idx;
                    }
                    else
                    { // poly map
                        int vm_idx = FindVMapByName(nVMaps, vmap->name.c_str(), vmap->type, true);
                        if (-1 == vm_idx)
                        {
                            nVMaps.push_back(new st_VMap(vmap->name.c_str(), vmap->type, true));
                            vm_idx = nVMaps.size() - 1;
                        }
                        st_VMap* nVMapPM = nVMaps[vm_idx];

                        //						int uv_idx = FindSimilarUV(nVMapPM,vmap->getUV(pt_it->index));
                        //						if (uv_idx==-1){
                        //							uv_idx	= nVMapPM->size();
                        //							nVMapPM->appendUV(vmap->getUV(pt_it->index));
                        //							nVMapPM->appendVI(F.pv[k].pindex);
                        //							nVMapPM->appendPI(f_it-m_Faces.begin());
                        //						}
                        //						n_pt_it->index = uv_idx;

                        nVMapPM->appendUV(vmap->getUV(o_pt_it->index));
                        nVMapPM->appendVI(F.pv[k].pindex);
                        nVMapPM->appendPI(f_id);
                        n_pt_it->index = nVMapPM->size() - 1;
                        n_pt_it->vmap_index = vm_idx;
                    }
                }
                break;
                case vmtWeight:
                {
                    int vm_idx = FindVMapByName(nVMaps, vmap->name.c_str(), vmap->type, false);
                    if (-1 == vm_idx)
                    {
                        nVMaps.push_back(new st_VMap(vmap->name.c_str(), vmap->type, false));
                        vm_idx = nVMaps.size() - 1;
                    }
                    st_VMap* nWMap = nVMaps[vm_idx];
                    nWMap->appendW(vmap->getW(o_pt_it->index));
                    nWMap->appendVI(F.pv[k].pindex);
                    n_pt_it->index = nWMap->size() - 1;
                    n_pt_it->vmap_index = vm_idx;
                }
                break;
                }
            }
        }
    }
    for (auto& it : m_VMaps)
        xr_delete(it);

    m_VMaps.clear();
    m_VMaps = nVMaps;
    // clear refs
    for (auto& it : m_VMRefs)
        xr_free(it.pts);
    m_VMRefs.clear();
    m_VMRefs = nVMRefs;
}
// Lord: constexpr использовать
#define MX 25
#define MY 15
#define MZ 25
static Fvector VMmin, VMscale;
static U32Vec VM[MX + 1][MY + 1][MZ + 1];
static Fvector VMeps;

static FvectorVec m_NewPoints;

bool CEditableMesh::OptimizeFace(st_Face& face)
{
    Fvector points[3];
    int mface[3];
    int k;

    for (k = 0; k < 3; k++)
    {
        points[k].set(m_Vertices[face.pv[k].pindex]);
        mface[k] = -1;
    }

    // get similar vert idx list
    for (k = 0; k < 3; k++)
    {
        U32Vec* vl;
        int ix, iy, iz;
        ix = iFloor(float(points[k].x - VMmin.x) / VMscale.x * MX);
        iy = iFloor(float(points[k].y - VMmin.y) / VMscale.y * MY);
        iz = iFloor(float(points[k].z - VMmin.z) / VMscale.z * MZ);
        vl = &(VM[ix][iy][iz]);
        for (U32It it = vl->begin(); it != vl->end(); it++)
        {
            FvectorIt v = m_NewPoints.begin() + (*it);
            if (v->similar(points[k], EPS))
                mface[k] = *it;
        }
    }
    for (k = 0; k < 3; k++)
    {
        if (mface[k] == -1)
        {
            mface[k] = m_NewPoints.size();
            m_NewPoints.push_back(points[k]);
            int ix, iy, iz;
            ix = iFloor(float(points[k].x - VMmin.x) / VMscale.x * MX);
            iy = iFloor(float(points[k].y - VMmin.y) / VMscale.y * MY);
            iz = iFloor(float(points[k].z - VMmin.z) / VMscale.z * MZ);
            VM[ix][iy][iz].push_back(mface[k]);
            int ixE, iyE, izE;
            ixE = iFloor(float(points[k].x + VMeps.x - VMmin.x) / VMscale.x * MX);
            iyE = iFloor(float(points[k].y + VMeps.y - VMmin.y) / VMscale.y * MY);
            izE = iFloor(float(points[k].z + VMeps.z - VMmin.z) / VMscale.z * MZ);
            if (ixE != ix)
                VM[ixE][iy][iz].push_back(mface[k]);
            if (iyE != iy)
                VM[ix][iyE][iz].push_back(mface[k]);
            if (izE != iz)
                VM[ix][iy][izE].push_back(mface[k]);
            if ((ixE != ix) && (iyE != iy))
                VM[ixE][iyE][iz].push_back(mface[k]);
            if ((ixE != ix) && (izE != iz))
                VM[ixE][iy][izE].push_back(mface[k]);
            if ((iyE != iy) && (izE != iz))
                VM[ix][iyE][izE].push_back(mface[k]);
            if ((ixE != ix) && (iyE != iy) && (izE != iz))
                VM[ixE][iyE][izE].push_back(mface[k]);
        }
    }

    if ((mface[0] == mface[1]) || (mface[1] == mface[2]) || (mface[0] == mface[2]))
    {
        Msg("!Optimize: Invalid face found. Removed.");
        return false;
    }
    else
    {
        face.pv[0].pindex = mface[0];
        face.pv[1].pindex = mface[1];
        face.pv[2].pindex = mface[2];
        return true;
    }
}

void CEditableMesh::OptimizeMesh(BOOL NoOpt)
{
    if (!NoOpt)
    {
        UnloadRenderBuffers();
        UnloadCForm();

        UnloadFNormals(true);
        UnloadVNormals(true);
        UnloadSVertices(true);
        UnloadAdjacency(true);

        // clear static data
        for (int x = 0; x < MX + 1; x++)
            for (int y = 0; y < MY + 1; y++)
                for (int z = 0; z < MZ + 1; z++)
                    VM[x][y][z].clear();
        VMscale.set(m_Box.vMax.x - m_Box.vMin.x + EPS_S, m_Box.vMax.y - m_Box.vMin.y + EPS_S,
            m_Box.vMax.z - m_Box.vMin.z + EPS_S);

        VMeps.set(VMscale.x / MX / 2, VMscale.y / MY / 2, VMscale.z / MZ / 2);
        VMeps.x = (VMeps.x < EPS_L) ? VMeps.x : EPS_L;
        VMeps.y = (VMeps.y < EPS_L) ? VMeps.y : EPS_L;
        VMeps.z = (VMeps.z < EPS_L) ? VMeps.z : EPS_L;

        m_NewPoints.clear();
        m_NewPoints.reserve(m_VertCount);

        xr_vector<bool> faces_mark;
        faces_mark.resize(m_FaceCount, false);
        int i_del_face = 0;
        for (u32 k = 0; k < m_FaceCount; k++)
        {
            if (!OptimizeFace(m_Faces[k]))
            {
                faces_mark[k] = true;

                //. -----in plugin
                //.              i_del_face			= 0;

                //. -----in editor
                i_del_face++;
            }
        }

        m_VertCount = m_NewPoints.size();
        xr_free(m_Vertices);
        m_Vertices = xr_alloc<Fvector>(m_VertCount);
        memcpy(m_Vertices, &*m_NewPoints.begin(), m_NewPoints.size() * sizeof(Fvector));

        if (i_del_face)
        {
            st_Face* old_faces = m_Faces;
            u32* old_sg = m_SmoothGroups;

            m_Faces = xr_alloc<st_Face>(m_FaceCount - i_del_face);
            m_SmoothGroups = xr_alloc<u32>(m_FaceCount - i_del_face);

            u32 new_dk = 0;
            for (u32 dk = 0; dk < m_FaceCount; ++dk)
            {
                if (faces_mark[dk])
                {
                    for (auto it : m_SurfFaces)
                    {
                        IntVec& pol_lst = it.second;
                        for (int k = 0; k < int(pol_lst.size()); ++k)
                        {
                            int& f = pol_lst[k];
                            if (f > (int)dk)
                            {
                                --f;
                            }
                            else if (f == (int)dk)
                            {
                                pol_lst.erase(pol_lst.begin() + k);
                                --k;
                            }
                        }
                    }
                    continue;
                }
                //. -----in plugin
                //.				new_dk++;

                m_Faces[new_dk] = old_faces[dk];
                m_SmoothGroups[new_dk] = old_sg[dk];

                //. -----in editors
                ++new_dk;
            }
            m_FaceCount = m_FaceCount - i_del_face;
            xr_free(old_faces);
            xr_free(old_sg);
        }
    }
}

#pragma endregion

#pragma region EditMeshRender
#include "Layers/xrRender/D3DUtils.h"

//----------------------------------------------------
#define F_LIM (10000)
#define V_LIM (F_LIM * 3)

//----------------------------------------------------
void CEditableMesh::GenerateRenderBuffers()
{
    //    CTimer T;
    //    T.Start();
    /*
        CMemoryWriter 	F;
        m_Parent->PrepareOGF(F,false,this);
        IReader R		(F.pointer(), F.size());
        m_Visual 		= ::Render->Models->Create(GetName(),&R);
    //    Log				("Time: ",T.GetElapsed_sec());
    //	string_path fn;
    //	strconcat		(fn,"_alexmx_\\",GetName(),".ogf");
    //	FS.update_path	(fn,_import_,fn);
    //	F.save_to		(fn);
        return;
    */
    if (m_RenderBuffers)
        return;
    m_RenderBuffers = new RBMap();

    GenerateVNormals(0);

    VERIFY(m_VertexNormals);

    for (SurfFaces::iterator sp_it = m_SurfFaces.begin(); sp_it != m_SurfFaces.end(); sp_it++)
    {
        IntVec& face_lst = sp_it->second;
        CSurface* _S = sp_it->first;
        int num_verts = face_lst.size() * 3;
        RBVector rb_vec;
        int v_cnt = num_verts;
        int start_face = 0;
        int num_face;
        VERIFY3(v_cnt, "Empty surface arrive.", _S->_Name());
        do
        {
            rb_vec.push_back(st_RenderBuffer(0, (v_cnt < V_LIM) ? v_cnt : V_LIM));
            st_RenderBuffer& rb = rb_vec.back();
            if (_S->m_Flags.is(CSurface::sf2Sided))
                rb.dwNumVertex *= 2;
            num_face = (v_cnt < V_LIM) ? v_cnt / 3 : F_LIM;

            int buf_size = D3DXGetFVFVertexSize(_S->_FVF()) * rb.dwNumVertex;
            R_ASSERT2(buf_size, "Empty buffer size or bad FVF.");

            void* bytes = 0;
            //    R_CHK(HW.pDevice->CreateVertexBuffer(buf_size, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &pVB, 0));


            //    R_CHK(pVB->Lock(0, 0, (LPVOID*)&bytes, 0));
            
           // pVB->Unlock();
            
            D3D11_BUFFER_DESC vbDesc = {0}; // Lord: подумать над переносом правильно ли было сделано
            vbDesc.Usage = D3D11_USAGE_DEFAULT;
            vbDesc.ByteWidth = sizeof(FVF::L) * num_verts;
            vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vbDesc.CPUAccessFlags = 0;
            vbDesc.MiscFlags = 0;
            xr_vector<FVF::L> mData;
            // @ Filling
            {
               /// LPBYTE data = src_data;
             //   u32 dwFVF = surf->_FVF();
                u32 dwTexCnt = ((_S->_FVF() & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT);
                for (int fl_i = start_face; fl_i < start_face + num_face; ++fl_i)
                {
                    u32 f_index = face_lst[fl_i];
                    VERIFY(f_index < m_FaceCount);
                    st_Face& face = m_Faces[f_index];
 
                    for (int k = 0; k < 3; ++k)
                    {
                        st_FaceVert& fv = face.pv[k];
                        u32 norm_id = f_index * 3 + k; // fv.pindex;
                        VERIFY2(norm_id < m_FaceCount * 3, "Normal index out of range.");
                        VERIFY2(fv.pindex < (int)m_VertCount, "Point index out of range.");
                        Fvector& PN = m_VertexNormals[norm_id];
                        Fvector& V = m_Vertices[fv.pindex];
                        int sz = 0;
 
 
                        int offs = 0;
                        FVF::L mask;
                        Fvector2 data_UV;
                        for (int t = 0; t < dwTexCnt; t++)
                        {
                             VERIFY2((t + offs) < (int)m_VMRefs[fv.vmref].count, "- VMap layer index out of range");
                             st_VMapPt& vm_pt = m_VMRefs[fv.vmref].pts[t + offs];
                             if (m_VMaps[vm_pt.vmap_index]->type != vmtUV)
                             {
                                 offs++;
                                 t--;
                                 continue;
                             }
                             VERIFY2(vm_pt.vmap_index < int(m_VMaps.size()), "- VMap index out of range");
                             st_VMap* vmap = m_VMaps[vm_pt.vmap_index];
                             VERIFY2(vm_pt.index < vmap->size(), "- VMap point index out of range");
                             data_UV = vmap->getUV(vm_pt.index);
                        }
                        mask.set(V, D3DCOLOR_ARGB(255,128,128,255));
                        mData.push_back(mask);
                    }
                }
            }
            D3D11_SUBRESOURCE_DATA vbData = {0};
            vbData.pSysMem = mData.data();
            vbData.SysMemPitch = 0;
            vbData.SysMemSlicePitch = 0;

            ID3D11Buffer* pVB = nullptr;
            R_CHK(HW.pDevice->CreateBuffer(&vbDesc, &vbData, &pVB));
         //   D3D11_MAPPED_SUBRESOURCE vbSub;
        ////    R_CHK(HW.pContext->Map(pVB, 0, D3D_MAP_WRITE_DISCARD, 0, &vbSub));
         //   FillRenderBuffer(face_lst, start_face, num_face, _S, (LPBYTE&)bytes);
         //   CopyMemory(vbSub.pData, bytes, sizeof(bytes));
         //   HW.pContext->Unmap(pVB, 0);

            rb.pGeom.create(FVF::F_L, pVB, 0);
            v_cnt -= V_LIM;
            start_face += (_S->m_Flags.is(CSurface::sf2Sided)) ? rb.dwNumVertex / 6 : rb.dwNumVertex / 3;
        } while (v_cnt > 0);
        if (num_verts > 0)
            m_RenderBuffers->insert(std::make_pair(_S, rb_vec));
    }
    UnloadVNormals();
}

//----------------------------------------------------

void CEditableMesh::UnloadRenderBuffers()
{
    if (m_RenderBuffers)
    {
        for (RBMapPairIt rbmp_it = m_RenderBuffers->begin(); rbmp_it != m_RenderBuffers->end(); rbmp_it++)
        {
            for (RBVecIt rb_it = rbmp_it->second.begin(); rb_it != rbmp_it->second.end(); rb_it++)
                if (rb_it->pGeom)
                {
                    _RELEASE(rb_it->pGeom->vb);
                    _RELEASE(rb_it->pGeom->ib);
                    rb_it->pGeom.destroy();
                }
        }
        xr_delete(m_RenderBuffers);
    }
}

//----------------------------------------------------

void CEditableMesh::FillRenderBuffer(
    IntVec& face_lst, int start_face, int num_face, const CSurface* surf, LPBYTE& src_data)
{
    LPBYTE data = src_data;
    u32 dwFVF = surf->_FVF();
    u32 dwTexCnt = ((dwFVF & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT);
    for (int fl_i = start_face; fl_i < start_face + num_face; ++fl_i)
    {
        u32 f_index = face_lst[fl_i];
        VERIFY(f_index < m_FaceCount);
        st_Face& face = m_Faces[f_index];
        for (int k = 0; k < 3; ++k)
        {
            st_FaceVert& fv = face.pv[k];
            u32 norm_id = f_index * 3 + k; // fv.pindex;
            VERIFY2(norm_id < m_FaceCount * 3, "Normal index out of range.");
            VERIFY2(fv.pindex < (int)m_VertCount, "Point index out of range.");
            Fvector& PN = m_VertexNormals[norm_id];
            Fvector& V = m_Vertices[fv.pindex];
            int sz = 0;
            int res = dwFVF & 2;

            if (res)
            {
                sz = sizeof(Fvector);
                VERIFY2(fv.pindex < int(m_VertCount), "- Face index out of range.");
                CopyMemory(data, &V, sz);
                data += sz;
            }

            if (dwFVF & 0x010)
            {
                sz = sizeof(Fvector);
                CopyMemory(data, &PN, sz);
                data += sz;
            }
            
            sz = sizeof(Fvector2);
            int offs = 0;
            for (int t = 0; t < (int)dwTexCnt; t++)
            {
                VERIFY2((t + offs) < (int)m_VMRefs[fv.vmref].count, "- VMap layer index out of range");
                st_VMapPt& vm_pt = m_VMRefs[fv.vmref].pts[t + offs];
                if (m_VMaps[vm_pt.vmap_index]->type != vmtUV)
                {
                    offs++;
                    t--;
                    continue;
                }
                VERIFY2(vm_pt.vmap_index < int(m_VMaps.size()), "- VMap index out of range");
                st_VMap* vmap = m_VMaps[vm_pt.vmap_index];
                VERIFY2(vm_pt.index < vmap->size(), "- VMap point index out of range");
                CopyMemory(data, &vmap->getUV(vm_pt.index), sz);
                data += sz;
                //                Msg("%3.2f, %3.2f",vmap->getUV(vm_pt.index).x,vmap->getUV(vm_pt.index).y);
            }
        }
        if (surf->m_Flags.is(CSurface::sf2Sided))
        {
            for (int k = 2; k >= 0; k--)
            {
                st_FaceVert& fv = face.pv[k];
                Fvector& PN = m_VertexNormals[f_index * 3 + k];
                int sz;
                if (dwFVF & D3DFVF_XYZ)
                {
                    sz = sizeof(Fvector);
                    VERIFY2(fv.pindex < int(m_VertCount), "- Face index out of range.");
                    CopyMemory(data, &m_Vertices[fv.pindex], sz);
                    data += sz;
                }
                if (dwFVF & D3DFVF_NORMAL)
                {
                    sz = sizeof(Fvector);
                    Fvector N;
                    N.invert(PN);
                    CopyMemory(data, &N, sz);
                    data += sz;
                }
                sz = sizeof(Fvector2);
                int offs = 0;
                for (int t = 0; t < (int)dwTexCnt; t++)
                {
                    VERIFY2((t + offs) < (int)m_VMRefs[fv.vmref].count, "- VMap layer index out of range");
                    st_VMapPt& vm_pt = m_VMRefs[fv.vmref].pts[t];
                    if (m_VMaps[vm_pt.vmap_index]->type != vmtUV)
                    {
                        offs++;
                        t--;
                        continue;
                    }
                    VERIFY2(vm_pt.vmap_index < int(m_VMaps.size()), "- VMap index out of range");
                    st_VMap* vmap = m_VMaps[vm_pt.vmap_index];
                    VERIFY2(vm_pt.index < vmap->size(), "- VMap point index out of range");
                    CopyMemory(data, &vmap->getUV(vm_pt.index), sz);
                    data += sz;

                    //	                Msg("%3.2f, %3.2f",vmap->getUV(vm_pt.index).x,vmap->getUV(vm_pt.index).y);
                }
            }
        }
    }
}

//----------------------------------------------------
void CEditableMesh::Render(const Fmatrix& parent, CSurface* S)
{
    if (0 == m_RenderBuffers)
        GenerateRenderBuffers();
    // visibility test
    if (!m_Flags.is(flVisible))
        return;
    // frustum test
    Fbox bb;
    bb.set(m_Box);
    bb.xform(parent);
    if (!RImplementation.occ_visible(bb))
        return;
    // render
    RBMapPairIt rb_pair = m_RenderBuffers->find(S);
    if (rb_pair != m_RenderBuffers->end())
    {
        RBVector& rb_vec = rb_pair->second;
        for (RBVecIt rb_it = rb_vec.begin(); rb_it != rb_vec.end(); rb_it++)
            RCache.dbg_DP(D3DPT_TRIANGLELIST, rb_it->pGeom, 0, rb_it->dwNumVertex / 3);
 
    }
}

//----------------------------------------------------
#define MAX_VERT_COUNT 0xFFFF
static Fvector RB[MAX_VERT_COUNT];
static int RB_cnt = 0;

void CEditableMesh::RenderList(const Fmatrix& parent, u32 color, bool bEdge, IntVec& fl)
{
    //	if (!m_Visible) return;
    //	if (!m_LoadState.is(LS_RBUFFERS)) CreateRenderBuffers();
    // Lord: здесь подумать над методом setRS, то есть как в dx11 поставить рендерстэйт
    if (fl.size() == 0)
        return;
    RCache.set_xform_world(parent);
 //   EDevice.RenderNearer(0.0006); Lord: что за метод лул
    RB_cnt = 0;
    if (bEdge)
    {
        RCache.set_Shader(RImplementation.m_WireShader);
       // RCache.set_States(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    }
    else
        RCache.set_Shader(RImplementation.m_SelectionShader);

    for (IntIt dw_it = fl.begin(); dw_it != fl.end(); ++dw_it)
    {
        st_Face& face = m_Faces[*dw_it];
        for (int k = 0; k < 3; ++k)
            RB[RB_cnt++].set(m_Vertices[face.pv[k].pindex]);

        if (RB_cnt == MAX_VERT_COUNT)
        {
            DUImpl.DrawPrimitiveL(D3DPT_TRIANGLELIST, RB_cnt / 3, RB, RB_cnt, color, true, false);
            RB_cnt = 0;
        }
    }

    if (RB_cnt)
        DUImpl.DrawPrimitiveL(D3DPT_TRIANGLELIST, RB_cnt / 3, RB, RB_cnt, color, true, false);

    if (bEdge)
    {
        //      EDevice.SetRS(D3DRS_FILLMODE, EDevice.dwFillMode);
    }


   // EDevice.ResetNearer();
}

//----------------------------------------------------

void CEditableMesh::RenderSelection(const Fmatrix& parent, CSurface* s, u32 color)
{
    if (0 == m_RenderBuffers)
        GenerateRenderBuffers();
    //	if (!m_Visible) return;
    Fbox bb;
    bb.set(m_Box);
    bb.xform(parent);

    if (!RImplementation.occ_visible(bb))
        return;

    // render
    RCache.set_xform_world(parent);
    if (s)
    {
        SurfFaces::iterator sp_it = m_SurfFaces.find(s);
        if (sp_it != m_SurfFaces.end())
            RenderList(parent, color, false, sp_it->second);
    }
    else
    {
       // EDevice.SetRS(D3DRS_TEXTUREFACTOR, color);
        for (RBMapPairIt p_it = m_RenderBuffers->begin(); p_it != m_RenderBuffers->end(); p_it++)
        {
            RBVector& rb_vec = p_it->second;
            for (RBVecIt rb_it = rb_vec.begin(); rb_it != rb_vec.end(); rb_it++)
                RCache.dbg_DP(D3DPT_TRIANGLELIST, rb_it->pGeom, 0, rb_it->dwNumVertex / 3);
        }
       // EDevice.SetRS(D3DRS_TEXTUREFACTOR, 0xffffffff);
    }
}

//----------------------------------------------------

void CEditableMesh::RenderEdge(const Fmatrix& parent, CSurface* s, u32 color)
{
    if (0 == m_RenderBuffers)
        GenerateRenderBuffers();
    //	if (!m_Visible) return;
    RCache.set_xform_world(parent);
    RCache.set_Shader(RImplementation.m_WireShader);
    //EDevice.RenderNearer(0.001);

    // render
 //   EDevice.SetRS(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    if (s)
    {
        SurfFaces::iterator sp_it = m_SurfFaces.find(s);
        if (sp_it != m_SurfFaces.end())
            RenderList(parent, color, true, sp_it->second);
    }
    else
    {
  //      EDevice.SetRS(D3DRS_TEXTUREFACTOR, color);
        for (RBMapPairIt p_it = m_RenderBuffers->begin(); p_it != m_RenderBuffers->end(); p_it++)
        {
            RBVector& rb_vec = p_it->second;
            for (RBVecIt rb_it = rb_vec.begin(); rb_it != rb_vec.end(); rb_it++)
                RCache.dbg_DP(D3DPT_TRIANGLELIST, rb_it->pGeom, 0, rb_it->dwNumVertex / 3);
        }
     //   EDevice.SetRS(D3DRS_TEXTUREFACTOR, 0xffffffff);
    }
 //   EDevice.SetRS(D3DRS_FILLMODE, EDevice.dwFillMode);
//    EDevice.ResetNearer();
}

//----------------------------------------------------

#define SKEL_MAX_FACE_COUNT 10000

struct svertRender
{
    Fvector P;
    Fvector N;
    Fvector2 uv;
};

void CEditableMesh::RenderSkeleton(const Fmatrix&, CSurface* S)
{
    if (false == IsGeneratedSVertices(RENDER_SKELETON_LINKS))
        GenerateSVertices(RENDER_SKELETON_LINKS);

    R_ASSERT2(m_SVertices, "SVertices empty!");
    SurfFaces::iterator sp_it = m_SurfFaces.find(S);
    R_ASSERT(sp_it != m_SurfFaces.end());
    IntVec& face_lst = sp_it->second;
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase;

    svertRender* pv = (svertRender*)Stream->Lock(SKEL_MAX_FACE_COUNT * 3, m_Parent->vs_SkeletonGeom->vb_stride, vBase);
    Fvector P0, N0, P1, N1;

    int f_cnt = 0;
    for (IntIt i_it = face_lst.begin(); i_it != face_lst.end(); i_it++)
    {
        for (int k = 0; k < 3; k++, pv++)
        {
            st_SVert& SV = m_SVertices[*i_it * 3 + k];
            pv->uv.set(SV.uv);
            float total = SV.bones[0].w;

            const Fmatrix& M = m_Parent->m_Bones[SV.bones[0].id]->_RenderTransform();
            M.transform_tiny(pv->P, SV.offs);
            M.transform_dir(pv->N, SV.norm);

            Fvector P, N;

            for (u8 cnt = 1; cnt < (u8)SV.bones.size(); cnt++)
            {
                total += SV.bones[cnt].w;
                const Fmatrix& M = m_Parent->m_Bones[SV.bones[cnt].id]->_RenderTransform();
                M.transform_tiny(P, SV.offs);
                M.transform_dir(N, SV.norm);
                pv->P.lerp(pv->P, P, SV.bones[cnt].w / total);
                pv->N.lerp(pv->N, N, SV.bones[cnt].w / total);
            }
        }
        f_cnt++;
        if (S->m_Flags.is(CSurface::sf2Sided))
        {
            pv->P.set((pv - 1)->P);
            pv->N.invert((pv - 1)->N);
            pv->uv.set((pv - 1)->uv);
            pv++;
            pv->P.set((pv - 3)->P);
            pv->N.invert((pv - 3)->N);
            pv->uv.set((pv - 3)->uv);
            pv++;
            pv->P.set((pv - 5)->P);
            pv->N.invert((pv - 5)->N);
            pv->uv.set((pv - 5)->uv);
            pv++;
            f_cnt++;
        }
        if (f_cnt >= SKEL_MAX_FACE_COUNT - 1)
        {
            Stream->Unlock(f_cnt * 3, m_Parent->vs_SkeletonGeom->vb_stride);
            RCache.dbg_DP(D3DPT_TRIANGLELIST, m_Parent->vs_SkeletonGeom, vBase, f_cnt);
            pv = (svertRender*)Stream->Lock(SKEL_MAX_FACE_COUNT * 3, m_Parent->vs_SkeletonGeom->vb_stride, vBase);
            f_cnt = 0;
        }
    }
    Stream->Unlock(f_cnt * 3, m_Parent->vs_SkeletonGeom->vb_stride);
    if (f_cnt)
        RCache.dbg_DP(D3DPT_TRIANGLELIST, m_Parent->vs_SkeletonGeom, vBase, f_cnt);
}

//----------------------------------------------------

#pragma endregion
