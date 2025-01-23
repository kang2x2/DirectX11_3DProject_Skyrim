#include "VIBuffer_Grid.h"

CVIBuffer_Grid::CVIBuffer_Grid(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CVIBuffer(_pDevice, _pContext)
{
}

CVIBuffer_Grid::CVIBuffer_Grid(const CVIBuffer_Grid& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Grid::Initialize_ProtoType(_uint _iTerrainWidth, _uint _iTerrainHeight)
{
	m_iNumVerticesX = _iTerrainWidth;
	m_iNumVerticesZ = _iTerrainHeight;
	m_iStride = sizeof(VTXPOSCOL);
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_iIndexStride = 4;
	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 4;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	m_iNumVBs = 1;

	m_pPos = new _float3[m_iNumVertices];

#pragma region Vertex Buffer

	VTXPOSCOL* pVertices = new VTXPOSCOL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSCOL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			// pVertices[iIndex].vPosition = _float3((_float)j + 49.f, -7.82f, i);
			pVertices[iIndex].vPosition = _float3(j, 0.f, i);
			pVertices[iIndex].vColor = _float4(1.f, 1.f, 1.f, 1.f);

			m_pPos[iIndex] = pVertices[iIndex].vPosition;
		}
	}

#pragma endregion

#pragma region Index Buffer

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint iNumIndices = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			// 연산 편하게 미리 만들어 둠
			_uint iIndecise[4] =
			{
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumIndices++] = iIndecise[0];
			pIndices[iNumIndices++] = iIndecise[1];

			pIndices[iNumIndices++] = iIndecise[1];
			pIndices[iNumIndices++] = iIndecise[2];
	
			pIndices[iNumIndices++] = iIndecise[2];
			pIndices[iNumIndices++] = iIndecise[3];

			pIndices[iNumIndices++] = iIndecise[3];
			pIndices[iNumIndices++] = iIndecise[0];
		}
	}

#pragma endregion

#pragma region Create Buffer

	/* 정점 버퍼를 만든다. */
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	// m_BufferDesc.ByteWidth = 정점하나의 크기(Byte) * 정점의 갯수;
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적버퍼로 할당한다. (Lock, unLock 호출 불가)*/
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	/* 인덱스 버퍼를 만든다. */
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적버퍼로 할당한다. (Lock, unLock 호출 불가)*/
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);


#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Grid::Initialize_Clone(void* pArg)
{
	return S_OK;
}

CVIBuffer_Grid* CVIBuffer_Grid::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CVIBuffer_Grid* pInstance = new CVIBuffer_Grid(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType(GRIDWIDTH, GRIDHEIGHT)))
	{
		MSG_BOX("Fail Create : CVIBuffer_Grid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer* CVIBuffer_Grid::Clone(void* pArg)
{
	CVIBuffer_Grid* pInstance = new CVIBuffer_Grid(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Fail Clone : CVIBuffer_Grid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Grid::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pPos);
}
