#include "StdAfx.h"
#include "CSurface.h"

void CSurface::CreateImageData()
{
    VERIFY(0 == m_ImageData);
    m_ImageData = new SSimpleImage();
    m_ImageData->name = m_Texture;
    m_ImageData->layers.push_back(U32Vec());
    ImageLib.LoadTextureData(*m_ImageData->name, m_ImageData->layers.back(), m_ImageData->w, m_ImageData->h);
}

void CSurface::RemoveImageData() 
{
    if (m_ImageData)
        xr_delete(m_ImageData);
}
