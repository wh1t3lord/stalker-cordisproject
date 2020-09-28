//---------------------------------------------------------------------------

#ifndef du_sphereH
#define du_sphereH
//---------------------------------------------------------------------------
constexpr unsigned int DU_SPHERE_NUMVERTEX = 92;
constexpr unsigned int DU_SPHERE_NUMFACES = 180;
constexpr unsigned int DU_SPHERE_NUMVERTEXL = 60;
constexpr unsigned int DU_SPHERE_NUMLINES = 60;

extern __declspec(dllexport) Fvector du_sphere_vertices[];
extern __declspec(dllexport) WORD du_sphere_faces[];
extern __declspec(dllexport) Fvector du_sphere_verticesl[];
extern __declspec(dllexport) WORD du_sphere_lines[];

#endif
