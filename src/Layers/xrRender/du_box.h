//---------------------------------------------------------------------------

#ifndef du_boxH
#define du_boxH
//---------------------------------------------------------------------------
constexpr unsigned int DU_BOX_NUMVERTEX = 8;
constexpr unsigned int DU_BOX_NUMFACES = 12;
constexpr unsigned int DU_BOX_NUMLINES = 12;
constexpr unsigned int DU_BOX_NUMVERTEX2 = 36;

extern ECORE_API FVF::L du_box_vertices[];
extern ECORE_API WORD du_box_faces[];
extern ECORE_API WORD du_box_lines[];

extern ECORE_API Fvector du_box_vertices2[];
#endif
