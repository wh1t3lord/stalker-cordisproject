//---------------------------------------------------------------------------

#ifndef du_sphereH
#define du_sphereH
//---------------------------------------------------------------------------
constexpr unsigned int DU_SPHERE_NUMVERTEX = 92;
constexpr unsigned int DU_SPHERE_NUMFACES = 180;
constexpr unsigned int DU_SPHERE_NUMVERTEXL = 60;
constexpr unsigned int DU_SPHERE_NUMLINES = 60;

extern ECORE_API Fvector du_sphere_vertices[];
extern ECORE_API WORD du_sphere_faces[];
extern ECORE_API Fvector du_sphere_verticesl[];
extern ECORE_API WORD du_sphere_lines[];

#endif
