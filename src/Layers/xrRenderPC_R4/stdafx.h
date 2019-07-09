#pragma once

#include "Common/Common.hpp"

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include "xrEngine/stdafx.h"


#include "Layers/xrRender/xrD3DDefs.h"
#include "Layers/xrRender/Debug/dxPixEventWrapper.h"



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



// Lord: протестить


#include "EditorPref.h"
#include "SDK_Names.h"
#include "SDKUI.h"
