// Magic Software, Inc.
// http://www.magic-software.com
// http://www.wild-magic.com
// Copyright (c) 2004.  All Rights Reserved
//
// The Wild Magic Library (WML) source code is supplied under the terms of
// the license agreement http://www.magic-software.com/License/WildMagic.pdf
// and may not be copied or disclosed except in accordance with the terms of
// that agreement.
#include "StdAfx.h"
#pragma hdrstop

#include "WmlQuaternion.h"
template <>
const Wml::Quaternion<float> Wml::Quaternion<float>::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f);
template <>
const Wml::Quaternion<float> Wml::Quaternion<float>::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
template <>
int Wml::Quaternion<float>::ms_iNext[3] = {1, 2, 0};

template <>
const Wml::Quaternion<double> Wml::Quaternion<double>::IDENTITY(1.0, 0.0, 0.0, 0.0);
template <>
const Wml::Quaternion<double> Wml::Quaternion<double>::ZERO(0.0, 0.0, 0.0, 0.0);
template <>
int Wml::Quaternion<double>::ms_iNext[3] = {1, 2, 0};
