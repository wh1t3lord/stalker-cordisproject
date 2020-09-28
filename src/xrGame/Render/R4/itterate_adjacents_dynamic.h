#pragma once


#include "Common/face_smoth_flags.h"

template <typename typeVertex>
struct itterate_adjacents_params_dynamic
{
    typedef typeVertex type_vertex;
    typedef typename typeVertex::type_face type_face;
    // typedef xr_vector<type_face*> vecFace;

private:
    Fvector& normal;
    const unsigned int* m_SmoothGroups;
    const Fvector* m_FaceNormals;
    const IntVec& a_lst;
    const type_face* m_Faces;
    unsigned int m_FaceCount;
    xr_vector<unsigned char> m_procesed;

public:
    itterate_adjacents_params_dynamic(Fvector& _normal, const unsigned int* _SmoothGroups, const Fvector* _FaceNormals, const IntVec& _a_lst,
        const type_face* _Faces, unsigned int _FaceCount) : normal(_normal), m_SmoothGroups(_SmoothGroups), m_FaceNormals(_FaceNormals), a_lst(_a_lst), m_Faces(_Faces),
          m_FaceCount(_FaceCount)

    {
        normal.set(0, 0, 0);
        m_procesed.resize(a_lst.size(), 0);
    }
    inline const u32 current_adjacents_size() const { return u32(a_lst.size()); }

private:
    inline static bool has_same_edge(const type_face* F1, const type_face* F2, u16& F1_edge_index, u16& F2_edge_index)
    {
        F1_edge_index = u16(-1);
        F2_edge_index = u16(-1);

        for (int e = 0; e < 3; e++)
        {
            type_vertex v1_a, v1_b;
            F1->EdgeVerts(e, v1_a, v1_b);
            if (v1_a.gt(v1_b))
                std::swap(v1_a, v1_b);

            for (int r = 0; r < 3; ++r)
            {
                type_vertex v2_a, v2_b;
                F2->EdgeVerts(r, v2_a, v2_b);
                if (v2_a.gt(v2_b))
                    std::swap(v2_a, v2_b);

                if ((v1_a.eq(v2_a)) && (v1_b.eq(v2_b)))
                {
                    F1_edge_index = e;
                    F2_edge_index = r;
                    return true;
                }
            }
        }
        return false;
    }

    inline unsigned int face_idx(unsigned int adj_idx) const
    {
        VERIFY(adj_idx < a_lst.size());
        return a_lst[adj_idx];
    }

    inline const type_face* current_adjacents_face(unsigned int adj_idx) const
    {
        VERIFY(m_Faces);
        unsigned int index = face_idx(adj_idx);
        VERIFY(index < m_FaceCount);
        return &m_Faces[index];
    }

    inline bool do_connect_faces(unsigned int start_face_idx, unsigned int test_face_idx, unsigned short start_common_edge_idx, unsigned short test_common_edge_idx)
    {
        VERIFY(start_face_idx < m_FaceCount);
        VERIFY(test_face_idx < m_FaceCount);
        return do_connect_faces_by_faces_edge_flags(
            m_SmoothGroups[start_face_idx], m_SmoothGroups[test_face_idx], start_common_edge_idx, test_common_edge_idx);
    }

    inline bool is_processed(unsigned int adj_idx) const
    {
        VERIFY(adj_idx < a_lst.size());
        return !!m_procesed[adj_idx];
    }

    inline void set_processed(unsigned int adj_idx)
    {
        VERIFY(adj_idx < a_lst.size());
        m_procesed[adj_idx] = unsigned char(1);
    }

    inline void add_adjacents(unsigned int test_face_idx)
    {
        VERIFY(test_face_idx < m_FaceCount);

        normal.add(m_FaceNormals[test_face_idx]);
    }

public:
    inline bool add_adjacents(unsigned int start_face_adj_idx, unsigned int test_face_adj_idx)
    {
        if (is_processed(test_face_adj_idx))
            return false;

        const type_face* start_face = current_adjacents_face(start_face_adj_idx);
        const type_face* test_face = current_adjacents_face(test_face_adj_idx);

        unsigned int test_face_idx = face_idx(test_face_adj_idx);
        unsigned int start_face_idx = face_idx(start_face_adj_idx);

        unsigned short StartFace_common_edge_index = unsigned short(-1);
        unsigned short TestFace_common_edge_index = unsigned short(-1);

        if (has_same_edge(start_face, test_face, StartFace_common_edge_index, TestFace_common_edge_index))
        {
            if ((start_face_idx == test_face_idx) ||
                do_connect_faces(
                    start_face_idx, test_face_idx, StartFace_common_edge_index, TestFace_common_edge_index))
            {
                set_processed(test_face_adj_idx);
                add_adjacents(test_face_idx);
                return true;
            }
        }
        return false;
    }
};

 