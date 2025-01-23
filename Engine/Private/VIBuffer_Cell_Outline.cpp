#include "VIBuffer_Cell_Outline.h"

CVIBuffer_Cell_Outline::CVIBuffer_Cell_Outline(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CVIBuffer(_pDevice, _pContext)
{
}

CVIBuffer_Cell_Outline::CVIBuffer_Cell_Outline(const CVIBuffer_Cell_Outline& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cell_Outline::Initialize_ProtoType(const _float3* _Points)
{
	m_iStride = sizeof(VTXPOSCELL); // 버텍스 하나 사이즈
	m_iNumVertices = 3;
	m_iIndexStride = 2; // 인덱스 하나 사이즈
	m_iNumIndices = 4;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
	m_iNumVBs = 1;

#pragma region Vertex
	/* 버퍼 생성*/
	ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices; // 정점 하나의 크기
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; // 정적버퍼로 할당(Lock, UnLock 불가)
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXPOSCELL* pVertices = new VTXPOSCELL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSCELL) * m_iNumVertices);

	memcpy(pVertices, _Points, sizeof(_float3) * m_iNumVertices);

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region Index
	/* 버퍼 생성 */
	ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; // 정적버퍼로 할당(Lock, UnLock 불가)
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	pIndices[3] = 0;

	ZeroMemory(&m_InitialData, sizeof(m_InitialData));
	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;

}

HRESULT CVIBuffer_Cell_Outline::Initialize_Clone(void* pArg)
{
	return S_OK;
}

CVIBuffer_Cell_Outline* CVIBuffer_Cell_Outline::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const _float3* _Points)
{
	CVIBuffer_Cell_Outline* pInstance = new CVIBuffer_Cell_Outline(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType(_Points)))
	{
		MSG_BOX("Fail Create : CVIBuffer_Cell_Outline");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Cell_Outline::Clone(void* _pArg)
{
	CVIBuffer_Cell_Outline* pInstance = new CVIBuffer_Cell_Outline(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CVIBuffer_Cell_Outline");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cell_Outline::Free()
{
	__super::Free();
}
