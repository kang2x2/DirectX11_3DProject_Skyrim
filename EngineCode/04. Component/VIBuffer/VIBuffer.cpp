#include "VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CComponent(_pDevice, _pContext)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_BufferDesc(rhs.m_BufferDesc)
	, m_InitialData(rhs.m_InitialData)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_eTopology(rhs.m_eTopology)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iIndexStride(rhs.m_iIndexStride)
	, m_iNumIndices(rhs.m_iNumIndices)
	, m_iNumVBs(rhs.m_iNumVBs)

{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	ID3D11Buffer* pVertexBuffer[] =
	{
		m_pVB,
	};

	_uint	iStrides[] =
	{
		m_iStride,
	};
	
	_uint	iOffsets[] =
	{
		0,
	};

	// 버텍스 버퍼 할당
	m_pContext->IASetVertexBuffers(0, m_iNumVBs, pVertexBuffer, iStrides, iOffsets);

	// 인덱스 버퍼 할당
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	// 해당 정점들을 어떤 방식으로 그릴 것인지 결정.
	m_pContext->IASetPrimitiveTopology(m_eTopology);
	
	// 인덱스가 가르키는 정점을 활용하여 그린다.
	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer::Create_Buffer(_Inout_ ID3D11Buffer** ppOut)
{
	if (m_pDevice == nullptr)
		return E_FAIL;

	/* 버퍼 정보와 초기화 정보를 넘겨 버텍스나 인덱스 버퍼 생성 */
	return m_pDevice->CreateBuffer(&m_BufferDesc, &m_InitialData, ppOut);
}

void CVIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
