#ifndef skeleton_motion_defs_inluded
#define skeleton_motion_defs_inluded

#pragma once

constexpr unsigned int MAX_PARTS = 4;

constexpr float SAMPLE_FPS = 30.f;
constexpr float SAMPLE_SPF = (1.f / SAMPLE_FPS);
constexpr float END_EPS = SAMPLE_SPF + EPS;
constexpr float KEY_Quant = 32767.f;
constexpr float KEY_QuantI = 1.f / KEY_Quant;

#endif
