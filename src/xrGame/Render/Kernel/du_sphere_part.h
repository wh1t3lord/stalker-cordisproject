//---------------------------------------------------------------------------

#ifndef du_sphere_partH
#define du_sphere_partH
//---------------------------------------------------------------------------
constexpr unsigned int DU_SPHERE_PART_NUMVERTEX = 82;
constexpr unsigned int DU_SPHERE_PART_NUMFACES = 160;
constexpr unsigned int DU_SPHERE_PART_NUMLINES = 176;

extern __declspec(dllexport) Fvector du_sphere_part_vertices[];
extern __declspec(dllexport) u16 du_sphere_part_faces[];
extern __declspec(dllexport) u16 du_sphere_part_lines[];
#endif
