#include "Cell.h"
#include "VIBuffer_Cell_Outline.h"
#include "VIBuffer_Cell_Fill.h"

CCell::CCell(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice(_pDevice)
	, m_pContext(_pContext)
{
#ifdef _DEBUG
	Safe_AddRef(m_pVIBufferOutCom);
	Safe_AddRef(m_pVIBufferInCom);
#endif
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCell::Initialize(const _float3* _pPoints, _uint _iIndex)
{
	m_iIndex = _iIndex;

	memcpy(m_localPoints, _pPoints, sizeof(_float3) * POINT_END);

	_float3 vLines[LINE_END];

	XMStoreFloat3(&vLines[LINE_AB], XMLoadFloat3(&m_localPoints[POINT_B]) - XMLoadFloat3(&m_localPoints[POINT_A]));
	XMStoreFloat3(&vLines[LINE_BC], XMLoadFloat3(&m_localPoints[POINT_C]) - XMLoadFloat3(&m_localPoints[POINT_B]));
	XMStoreFloat3(&vLines[LINE_CA], XMLoadFloat3(&m_localPoints[POINT_A]) - XMLoadFloat3(&m_localPoints[POINT_C]));

	for (size_t i = 0; i < LINE_END; ++i)
	{
		m_normals[i] = _float3(vLines[i].z * -1.f, 0.f, vLines[i].x);

		XMStoreFloat3(&m_normals[i], XMVector3Normalize(XMLoadFloat3(&m_normals[i])));
	}

#ifdef _DEBUG
	m_pVIBufferOutCom = CVIBuffer_Cell_Outline::Create(m_pDevice, m_pContext, m_localPoints);
	if (nullptr == m_pVIBufferOutCom)
		return E_FAIL;

	m_pVIBufferInCom = CVIBuffer_Cell_Fill::Create(m_pDevice, m_pContext, m_localPoints);
	if (nullptr == m_pVIBufferInCom)
		return E_FAIL;
#endif

	return S_OK;
}

void CCell::Update(_fmatrix _matWorld)
{
	for (size_t i = 0; i < POINT_END; ++i)
	{
		XMStoreFloat3(&m_localPoints[i], 
			XMVector3TransformCoord(XMLoadFloat3(&m_localPoints[i]), _matWorld));
	}
}

_bool CCell::Compare_Points(const _float3* _pSourPoints, const _float3* _pDestPoints)
{
	if (XMVector3Equal(XMLoadFloat3(&m_localPoints[POINT_A]), (XMLoadFloat3(_pSourPoints))))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_localPoints[POINT_B]), (XMLoadFloat3(_pDestPoints))))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_localPoints[POINT_C]), (XMLoadFloat3(_pDestPoints))))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_localPoints[POINT_B]), (XMLoadFloat3(_pSourPoints))))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_localPoints[POINT_A]), (XMLoadFloat3(_pDestPoints))))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_localPoints[POINT_C]), (XMLoadFloat3(_pDestPoints))))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_localPoints[POINT_C]), (XMLoadFloat3(_pSourPoints))))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_localPoints[POINT_A]), (XMLoadFloat3(_pDestPoints))))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_localPoints[POINT_B]), (XMLoadFloat3(_pDestPoints))))
			return true;
	}

	return false;
}

_bool CCell::IsOut(_fvector _vPoints, _fmatrix _matWorld, _int* _pINeighborIndex)
{
	for (size_t i = 0; i < LINE_END; i++)
	{
		_vector	vSour = XMVector3Normalize(_vPoints - XMLoadFloat3(&m_localPoints[i]));
		_vector	vDest = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_normals[i]), _matWorld));

		if (0 < XMVectorGetX(XMVector3Dot(vSour, vDest)))
		{
			*_pINeighborIndex = m_iNeighborIndices[i];
			return true;
		}
	}

	return false;
}

#ifdef _DEBUG
HRESULT CCell::Render(CELLRENDER_TYPE _eType)
{
	if(_eType == CELL_OUTLINE)
		m_pVIBufferOutCom->Render();

	else if (_eType == CELL_FILL)
		m_pVIBufferInCom->Render();

	return S_OK;
}
#endif

CCell* CCell::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const _float3* _pPoints, _uint _iIndex)
{
	CCell* pInstance = new CCell(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_pPoints, _iIndex)))
	{
		MSG_BOX("Fail Create : CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pVIBufferOutCom);
	Safe_Release(m_pVIBufferInCom);
#endif
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
