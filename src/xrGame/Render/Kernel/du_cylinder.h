//---------------------------------------------------------------------------

#ifndef du_cylinderH
#define du_cylinderH
//---------------------------------------------------------------------------
constexpr unsigned int DU_CYLINDER_NUMVERTEX = 26;
constexpr unsigned int DU_CYLINDER_NUMFACES = 48;
constexpr unsigned int DU_CYLINDER_NUMLINES = 30; // 36

extern __declspec(dllexport) Fvector du_cylinder_vertices[];
extern __declspec(dllexport) u16 du_cylinder_faces[];
extern __declspec(dllexport) u16 du_cylinder_lines[];

#endif
