#ifndef QueryHelper_included
#define QueryHelper_included
#pragma once

//	Interface
#ifdef USE_OGL
inline HRESULT CreateQuery(GLuint* pQuery);
inline HRESULT GetData(GLuint query, void* pData, UINT DataSize);
inline HRESULT BeginQuery(GLuint query);
inline HRESULT EndQuery(GLuint query);
inline HRESULT ReleaseQuery(GLuint pQuery);
#else
inline HRESULT CreateQuery(ID3DQuery** ppQuery);
inline HRESULT GetData(ID3DQuery* pQuery, void* pData, UINT DataSize);
inline HRESULT BeginQuery(ID3DQuery* pQuery);
inline HRESULT EndQuery(ID3DQuery* pQuery);
inline HRESULT ReleaseQuery(ID3DQuery* pQuery);
#endif

//	Implementation

#if defined(USE_OGL)

inline HRESULT CreateQuery(GLuint* pQuery)
{
    glGenQueries(1, pQuery);
    return S_OK;
}

inline HRESULT GetData(GLuint query, void* pData, UINT DataSize)
{
    if (DataSize == sizeof(GLint64))
        CHK_GL(glGetQueryObjecti64v(query, GL_QUERY_RESULT, (GLint64*)pData));
    else
        CHK_GL(glGetQueryObjectiv(query, GL_QUERY_RESULT, (GLint*)pData));
    return S_OK;
}

inline HRESULT BeginQuery(GLuint query)
{
    CHK_GL(glBeginQuery(GL_SAMPLES_PASSED, query));
    return S_OK;
}

inline HRESULT EndQuery(GLuint query)
{
    CHK_GL(glEndQuery(GL_SAMPLES_PASSED));
    return S_OK;
}

inline HRESULT ReleaseQuery(GLuint query)
{
    CHK_GL(glDeleteQueries(1, &query));
    return S_OK;
}

#elif defined(USE_DX11)

inline HRESULT CreateQuery(ID3DQuery** ppQuery)
{
    D3D_QUERY_DESC desc;
    desc.MiscFlags = 0;
    desc.Query = D3D_QUERY_OCCLUSION;
    return HW.pDevice->CreateQuery(&desc, ppQuery);
}

inline HRESULT GetData(ID3DQuery* pQuery, void* pData, UINT DataSize)
{
    //	Use D3Dxx_ASYNC_GETDATA_DONOTFLUSH for prevent flushing
    return HW.pContext->GetData(pQuery, pData, DataSize, 0);
}

inline HRESULT BeginQuery(ID3DQuery* pQuery)
{
    HW.pContext->Begin(pQuery);
    return S_OK;
}

inline HRESULT EndQuery(ID3DQuery* pQuery)
{
    HW.pContext->End(pQuery);
    return S_OK;
}

inline HRESULT ReleaseQuery(ID3DQuery* pQuery)
{
    _RELEASE(pQuery);
    return S_OK;
}
#endif //	USE_DX11

#endif // QueryHelper_included
