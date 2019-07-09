//---------------------------------------------------------------------------

#ifndef du_coneH
#define du_coneH
//---------------------------------------------------------------------------
constexpr unsigned int DU_CONE_NUMVERTEX = 18;
constexpr unsigned int DU_CONE_NUMFACES = 32;
constexpr unsigned int DU_CONE_NUMLINES = 24;

extern __declspec(dllexport) Fvector du_cone_vertices[];
extern __declspec(dllexport) WORD du_cone_faces[];
extern __declspec(dllexport) WORD du_cone_lines[];
#endif
