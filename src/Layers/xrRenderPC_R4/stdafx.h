#pragma once

#include "Common/Common.hpp"

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include "xrEngine/stdafx.h"
#include <d3dx9.h>

#include <D3D11.h>
#include <D3Dx11core.h>
#include <D3DCompiler.h>

#include "Layers/xrRender/xrD3DDefs.h"
#include "Layers/xrRender/Debug/dxPixEventWrapper.h"

#define R_GL 0
#define R_R1 1
#define R_R2 2
#define R_R3 3
#define R_R4 4
#define RENDER R_R4

#include "xrParticles/psystem.h"
#include "Layers/xrRender/HW.h"
#include "Layers/xrRender/Shader.h"
#include "Layers/xrRender/R_Backend.h"
#include "Layers/xrRender/R_Backend_Runtime.h"
#include "Layers/xrRender/ResourceManager.h"
#include "xrEngine/vis_common.h"
#include "xrEngine/Render.h"
#include "Common/_d3d_extensions.h"
#include "xrEngine/IGame_Level.h"
#include "Layers/xrRender/blenders/Blender.h"
#include "Layers/xrRender/blenders/Blender_CLSID.h"
#include "Layers/xrRender/xrRender_console.h"
#include "r4.h"

#include "xrCore/Imgui/imgui.h"
#include "xrCore/Imgui/imgui_impl_sdl.h"
#include "xrCore/Imgui/imgui_impl_dx11.h"

IC void jitter(CBlender_Compile& C)
{
    //	C.r_Sampler	("jitter0",	JITTER(0), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
    //	C.r_Sampler	("jitter1",	JITTER(1), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
    //	C.r_Sampler	("jitter2",	JITTER(2), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
    //	C.r_Sampler	("jitter3",	JITTER(3), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
    C.r_dx10Texture("jitter0", JITTER(0));
    C.r_dx10Texture("jitter1", JITTER(1));
    C.r_dx10Texture("jitter2", JITTER(2));
    C.r_dx10Texture("jitter3", JITTER(3));
    C.r_dx10Texture("jitter4", JITTER(4));
    C.r_dx10Texture("jitterMipped", r2_jitter_mipped);
    C.r_dx10Sampler("smp_jitter");
}

// Lord: протестить
using AStringVec = xr_vector<xr_string>;
using AStringIt = AStringVec::iterator;

using LPAStringVec = xr_vector<xr_string*>;
using LPAStringIt = LPAStringVec::iterator;

#include "EditorPref.h"
#include "SDK_Names.h"
#include "SDKUI.h"
inline float CalcSSA(Fvector& C, float R)
{
    float distSQ = Device.vCameraPosition.distance_to_sqr(C);
    return R * R / distSQ;
}
// Lord: Понять как это используется и улучшить эти возможности
enum
{
    rsFilterLinear = (1ul << 20ul),
    rsEdgedFaces = (1ul << 21ul),
    rsRenderTextures = (1ul << 22ul),
    rsLighting = (1ul << 23ul),
    rsFog = (1ul << 24ul),
    rsRenderRealTime = (1ul << 25ul),
    rsDrawGrid = (1ul << 26ul),
    rsDrawSafeRect = (1ul << 27ul),
    rsMuteSounds = (1ul << 28ul),
    rsEnvironment = (1ul << 29ul),
};
