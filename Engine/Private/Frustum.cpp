#include "Frustum.h"
#include "PipeLine.h"
#include "OcTree.h"

IMPLEMENT_SINGLETON(CFrustum)

CFrustum::CFrustum()
	: m_pPipeLine(CPipeLine::GetInstance()), m_pOcTree(nullptr)
{
	Safe_AddRef(m_pPipeLine);
}

HRESULT CFrustum::Initialize()
{
	// 투영 좌표계
	m_vOriginalPoints[0] = _float3(-1.f, 1.f, 0.f);
	m_vOriginalPoints[1] = _float3(1.f, 1.f, 0.f);
	m_vOriginalPoints[2] = _float3(1.f, -1.f, 0.f);
	m_vOriginalPoints[3] = _float3(-1.f, -1.f, 0.f);

	m_vOriginalPoints[4] = _float3(-1.f, 1.f, 1.f);
	m_vOriginalPoints[5] = _float3(1.f, 1.f, 1.f);
	m_vOriginalPoints[6] = _float3(1.f, -1.f, 1.f);
	m_vOriginalPoints[7] = _float3(-1.f, -1.f, 1.f);

	return S_OK;
}

void CFrustum::Tick()
{
	_matrix		ProjMatrixInv = m_pPipeLine->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_PROJ);
	_matrix		ViewMatrixInv = m_pPipeLine->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW);

	/* 여덟개 점을 월드까지 변환시키자. */
	for (size_t i = 0; i < 8; i++)
	{
		XMStoreFloat3(&m_vPoints[i], XMVector3TransformCoord(XMLoadFloat3(&m_vOriginalPoints[i]), ProjMatrixInv));
		XMStoreFloat3(&m_vPoints[i], XMVector3TransformCoord(XMLoadFloat3(&m_vPoints[i]), ViewMatrixInv));
	}

	/* 월드 상의 평면 여섯개를 구성하자. */
	Make_Planes(m_vPoints, m_WorldPlanes);

	if (m_pOcTree != nullptr)
	{
		m_pOcTree->Update();
	}
}

void CFrustum::Transform_ToLocalSpace(_fmatrix _WorldMatrixInverse)
{
	_float3		vLocalPoints[8];

	for (size_t i = 0; i < 8; i++)
	{
		/* 월드상에 정보를 담고 있는 점 여덟개에 지형의 월드역행려을 곱해서 지형의 로컬로 옮긴다. */
		XMStoreFloat3(&vLocalPoints[i], XMVector3TransformCoord(XMLoadFloat3(&m_vPoints[i]), _WorldMatrixInverse));
	}

	/* 로컬 상의 평면 여섯개를 구성하자. */
	Make_Planes(vLocalPoints, m_LocalPlanes);
}

_bool CFrustum::IsIn_Frustum_World(_fvector _vWorldPos, _float _fRadius)
{
	/* 0 < ax + by + cz + d */

	/* Plane		: a b c d */
	/* vWorldPos	: x y z 1 */
	for (size_t i = 0; i < 6; i++)
	{
		// XMPlaneDotCoord Result : ax + by + cz + d
		// DirectX 라이브러리는 벡터와 관련된 모든 연산을 벡터 형식으로 수행함.
		// 즉, XMPlaneDotCoord 함수에서 반환되는 vector의 각 성분에 ax, by, cz, d를 저장하는게 아님.
		// 반환 vector의 x 성분에 (a, b, c)(x, y, z) 두 벡터의 내적 결과인 스칼라 값 + d가 저장.
		// 때문에 x성분을 추출해서 비교하는 것.
		// 추가적으로 내적의 결과는 항상 -1에서 1사이의 값이 아니다.
		// 두 벡터가 정규화되었다는 기준 하에 -1 에서 1사이의 값을 반환하는 것.
		if (_fRadius < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_WorldPlanes[i]), _vWorldPos)))
			return false;
	}

	return true;
}

_bool CFrustum::IsIn_Frustum_Local(_fvector _vLocalPos, _float _fRadius)
{
	for (size_t i = 0; i < 6; i++)
	{
		if (_fRadius < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_LocalPlanes[i]), _vLocalPos)))
			return false;
	}

	return true;
}

void CFrustum::InitOcTree(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, _float3 _vCenter, _float3 _vRootRange)
{
	m_pOcTree = nullptr;
	m_pOcTree = new COcTree(_pDevice, _pContext);

	m_pOcTree->Initialize(_vCenter, _vRootRange);
}

void CFrustum::Make_Planes(const _float3* _pPoints, _float4* _pPlanes)
{
	// XMPlaneFromPoints 함수는 평면 방정식을 위한 a, b, c, d를 구하기 위한 함수이다. 
	XMStoreFloat4(&_pPlanes[0], XMPlaneFromPoints(XMLoadFloat3(&_pPoints[1]), XMLoadFloat3(&_pPoints[5]), XMLoadFloat3(&_pPoints[6])));
	XMStoreFloat4(&_pPlanes[1], XMPlaneFromPoints(XMLoadFloat3(&_pPoints[4]), XMLoadFloat3(&_pPoints[0]), XMLoadFloat3(&_pPoints[3])));
	XMStoreFloat4(&_pPlanes[2], XMPlaneFromPoints(XMLoadFloat3(&_pPoints[4]), XMLoadFloat3(&_pPoints[5]), XMLoadFloat3(&_pPoints[1])));
	XMStoreFloat4(&_pPlanes[3], XMPlaneFromPoints(XMLoadFloat3(&_pPoints[3]), XMLoadFloat3(&_pPoints[2]), XMLoadFloat3(&_pPoints[6])));
	XMStoreFloat4(&_pPlanes[4], XMPlaneFromPoints(XMLoadFloat3(&_pPoints[5]), XMLoadFloat3(&_pPoints[4]), XMLoadFloat3(&_pPoints[7])));
	XMStoreFloat4(&_pPlanes[5], XMPlaneFromPoints(XMLoadFloat3(&_pPoints[0]), XMLoadFloat3(&_pPoints[1]), XMLoadFloat3(&_pPoints[2])));
}

#ifdef _DEBUG
HRESULT CFrustum::RenderOcTree()
{
	if (m_pOcTree != nullptr)
	{
		m_pOcTree->Render();
	}

	return S_OK;
}
#endif

void CFrustum::Free()
{
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pOcTree);
}
