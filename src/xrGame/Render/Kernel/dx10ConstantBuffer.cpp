#include "StdAfx.h"
#include "dx10ConstantBuffer.h"

#include "dx10BufferUtils.h"

dx10ConstantBuffer::~dx10ConstantBuffer()
{
    RImplementation.Resources->_DeleteConstantBuffer(this);
    //	Flush();
    if (this->m_pBuffer)
    {
        this->m_pBuffer->Release();
        this->m_pBuffer = nullptr;
    }

    xr_free(this->m_pBufferData);
}

dx10ConstantBuffer::dx10ConstantBuffer(ID3DShaderReflectionConstantBuffer* pTable) : m_bChanged(true)
{
    D3D_SHADER_BUFFER_DESC Desc;

    CHK_DX(pTable->GetDesc(&Desc));

    this->m_strBufferName._set(Desc.Name);
    this->m_eBufferType = Desc.Type;
    this->m_uiBufferSize = Desc.Size;

    //	Fill member list with variable descriptions
    this->m_MembersList.resize(Desc.Variables);
    this->m_MembersNames.resize(Desc.Variables);
    for (u32 i = 0; i < Desc.Variables; ++i)
    {
        ID3DShaderReflectionVariable* pVar;
        ID3DShaderReflectionType* pType;

        D3D_SHADER_VARIABLE_DESC var_desc;

        pVar = pTable->GetVariableByIndex(i);
        VERIFY(pVar);
        pType = pVar->GetType();
        VERIFY(pType);
        pType->GetDesc(&m_MembersList[i]);
        //	Buffers with the same layout can contain totally different members
        CHK_DX(pVar->GetDesc(&var_desc));
        this->m_MembersNames[i] = var_desc.Name;
    }

    this->m_uiMembersCRC = crc32(&m_MembersList[0], Desc.Variables * sizeof(m_MembersList[0]));

    R_CHK(dx10BufferUtils::CreateConstantBuffer(&this->m_pBuffer, Desc.Size));
    VERIFY(this->m_pBuffer);
    this->m_pBufferData = xr_malloc(Desc.Size);
    VERIFY(this->m_pBufferData);
}

bool dx10ConstantBuffer::Similar(dx10ConstantBuffer& _in)
{
    if (m_strBufferName._get() != _in.m_strBufferName._get())
        return false;

    if (m_eBufferType != _in.m_eBufferType)
        return false;

    if (m_uiMembersCRC != _in.m_uiMembersCRC)
        return false;

    if (m_MembersList.size() != _in.m_MembersList.size())
        return false;

    if (memcmp(&m_MembersList[0], &_in.m_MembersList[0], m_MembersList.size() * sizeof(m_MembersList[0])))
        return false;

    VERIFY(m_MembersNames.size() == _in.m_MembersNames.size());

    int iMemberNum = m_MembersNames.size();
    for (int i = 0; i < iMemberNum; ++i)
    {
        if (m_MembersNames[i].c_str() != _in.m_MembersNames[i].c_str())
            return false;
    }

    return true;
}

void dx10ConstantBuffer::Flush()
{
    if (this->m_bChanged)
    {
        D3D11_MAPPED_SUBRESOURCE pSubRes;
        HRESULT hr;
        hr = HW.pContext->Map(this->m_pBuffer, 0, D3D_MAP_WRITE_DISCARD, 0, &pSubRes);

        if (FAILED(hr))
        {
            ASSERT(TEXT("Can't apply map!"));
            return;
        }

        if (!pSubRes.pData)
        {
            ASSERT(TEXT("pData was null"));
            return;
        }

        if (!this->m_pBufferData)
        {
            ASSERT(TEXT("was null!"));
            return;
        }

        CopyMemory(pSubRes.pData, this->m_pBufferData, this->m_uiBufferSize);
        HW.pContext->Unmap(this->m_pBuffer, 0);
        this->m_bChanged = false;
    }
}
