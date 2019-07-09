//---------------------------------------------------------------------------

#ifndef du_boxH
#define du_boxH
//---------------------------------------------------------------------------
constexpr unsigned int DU_BOX_NUMVERTEX = 8;
constexpr unsigned int DU_BOX_NUMFACES = 12;
constexpr unsigned int DU_BOX_NUMLINES = 12;
constexpr unsigned int DU_BOX_NUMVERTEX2 = 36;

extern __declspec(dllexport) FVF::L du_box_vertices[];
extern __declspec(dllexport) WORD du_box_faces[];
extern __declspec(dllexport) WORD du_box_lines[];

extern __declspec(dllexport) Fvector du_box_vertices2[];
#endif
