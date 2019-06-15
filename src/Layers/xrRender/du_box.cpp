//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma hdrstop

#include "du_box.h"
#include "SDK_ObjectShape.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)
Fvector du_box_vertices2[DU_BOX_NUMVERTEX2] = {{-0.5f, -0.5f, -0.5f}, {-0.5f, +0.5f, -0.5f}, {+0.5f, +0.5f, -0.5f},
    {+0.5f, +0.5f, -0.5f}, {+0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {+0.5f, -0.5f, -0.5f}, {+0.5f, +0.5f, -0.5f},
    {+0.5f, +0.5f, +0.5f}, {+0.5f, +0.5f, +0.5f}, {+0.5f, -0.5f, +0.5f}, {+0.5f, -0.5f, -0.5f}, {+0.5f, +0.5f, +0.5f},
    {-0.5f, +0.5f, +0.5f}, {-0.5f, -0.5f, +0.5f}, {+0.5f, +0.5f, +0.5f}, {-0.5f, -0.5f, +0.5f}, {+0.5f, -0.5f, +0.5f},
    {-0.5f, +0.5f, +0.5f}, {-0.5f, +0.5f, -0.5f}, {-0.5f, -0.5f, +0.5f}, {-0.5f, +0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, +0.5f}, {+0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, +0.5f}, {-0.5f, -0.5f, -0.5f}, {+0.5f, -0.5f, -0.5f},
    {+0.5f, -0.5f, +0.5f}, {-0.5f, -0.5f, +0.5f}, {-0.5f, +0.5f, -0.5f}, {-0.5f, +0.5f, +0.5f}, {+0.5f, +0.5f, +0.5f},
    {-0.5f, +0.5f, -0.5f}, {+0.5f, +0.5f, +0.5f}, {+0.5f, +0.5f, -0.5f}};

FVF::L du_box_vertices[DU_BOX_NUMVERTEX] = {
    {-0.5f, -0.5f, -0.5f, Cordis::SDK::ShapesColors::kEdgeColor},
    {-0.5f, +0.5f, -0.5f, Cordis::SDK::ShapesColors::kEdgeColor},
    {+0.5f, +0.5f, -0.5f, Cordis::SDK::ShapesColors::kEdgeColor},
    {+0.5f, -0.5f, -0.5f, Cordis::SDK::ShapesColors::kEdgeColor},
    {-0.5f, +0.5f, +0.5f, Cordis::SDK::ShapesColors::kEdgeColor},
    {-0.5f, -0.5f, +0.5f, Cordis::SDK::ShapesColors::kEdgeColor},
    {+0.5f, +0.5f, +0.5f, Cordis::SDK::ShapesColors::kEdgeColor},
    {+0.5f, -0.5f, +0.5f, Cordis::SDK::ShapesColors::kEdgeColor},
};

WORD du_box_faces[DU_BOX_NUMFACES * 3] = {
    0, 1, 2, 2, 3, 0, 3, 2, 6, 6, 7, 3, 6, 4, 5, 6, 5, 7, 4, 1, 5, 1, 0, 5, 3, 5, 0, 3, 7, 5, // bottom
    1, 4, 6, 1, 6, 2 // top
};

WORD du_box_lines[DU_BOX_NUMLINES * 2] = {0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 7, 6, 7, 6, 4, 0, 5, 1, 4, 2, 6, 3, 7};
