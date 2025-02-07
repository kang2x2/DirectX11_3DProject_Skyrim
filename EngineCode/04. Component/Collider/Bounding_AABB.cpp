#include "Bounding_AABB.h"
#include "DebugDraw.h"

#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

CBounding_AABB::CBounding_AABB()
{
}

_vector CBounding_AABB::Get_WorldCenter()
{
	return 	{ m_pAABB->Center.x, m_pAABB->Center.y, m_pAABB->Center.z, 1.f };
}

HRESULT CBounding_AABB::Initialize(const BOUNDING_DESC* _pDesc)
{
	if (FAILED(__super::Initialize(_pDesc)))
		return E_FAIL;

	BOUNDING_AABB_DESC* pAABBDesc = (BOUNDING_AABB_DESC*)_pDesc;

	m_pAABB_Original = new BoundingBox(pAABBDesc->vCenter, pAABBDesc->vExtents);
	m_pAABB = new BoundingBox(*m_pAABB_Original);

	return S_OK;
}

void CBounding_AABB::Update(_fmatrix _TransformMat)
{
	_matrix		Matrix = _TransformMat;

	Matrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(_TransformMat.r[0]));
	Matrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(_TransformMat.r[1]));
	Matrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(_TransformMat.r[2]));

	m_pAABB_Original->Transform(*m_pAABB, Matrix);
}

_bool CBounding_AABB::IsCollision(CCollider::COLLIDER_TYPE _eType, CBounding* _pBounding)
{
	/* 내 바운딩과 상대 바운딩과의 출동 처리. */
	m_bIsCol = false;

	switch (_eType)
	{
	case CCollider::TYPE_AABB:
		m_bIsCol = m_pAABB->Intersects(*((CBounding_AABB*)_pBounding)->Get_Bounding());
		break;
	case CCollider::TYPE_OBB:
		m_bIsCol = m_pAABB->Intersects(*((CBounding_OBB*)_pBounding)->Get_Bounding());
		break;
	case CCollider::TYPE_SPHERE:
		m_bIsCol = m_pAABB->Intersects(*((CBounding_Sphere*)_pBounding)->Get_Bounding());
		break;
	}

	return m_bIsCol;
}

void CBounding_AABB::Set_ColliderDesc(_float _fExtents)
{
	m_pAABB->Extents.z = _fExtents;
	m_pAABB->Extents.x = _fExtents;
	m_pAABB_Original->Extents.z = _fExtents;
	m_pAABB_Original->Extents.x = _fExtents;

}

#ifdef _DEBUG

HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* _pBatch)
{
	_vector		vColor = m_bIsCol == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);

	DX::Draw(_pBatch, *m_pAABB, vColor);

	return S_OK;
}

#endif

CBounding_AABB* CBounding_AABB::Create(const BOUNDING_DESC* _pDesc)
{
	CBounding_AABB* pInstance = new CBounding_AABB();

	if (FAILED(pInstance->Initialize(_pDesc)))
	{
		MSG_BOX("Fail Create : CBounding_AABB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_AABB::Free()
{
	__super::Free();

	Safe_Delete(m_pAABB_Original);
	Safe_Delete(m_pAABB);
}
