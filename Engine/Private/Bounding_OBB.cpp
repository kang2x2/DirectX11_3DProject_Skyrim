#include "Bounding_OBB.h"
#include "DebugDraw.h"

#include "Bounding_AABB.h"
#include "Bounding_Sphere.h"

CBounding_OBB::CBounding_OBB()
{
}

_vector CBounding_OBB::Get_WorldCenter()
{
	return 	{ m_pOBB->Center.x, m_pOBB->Center.y, m_pOBB->Center.z, 1.f };
}

HRESULT CBounding_OBB::Initialize(const BOUNDING_DESC* _pDesc)
{
	if (FAILED(__super::Initialize(_pDesc)))
		return E_FAIL;

	BOUNDING_OBB_DESC* pOBBDesc = (BOUNDING_OBB_DESC*)_pDesc;

	_float4		vRotation;

	XMStoreFloat4(&vRotation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(pOBBDesc->vDegree.x), XMConvertToRadians(pOBBDesc->vDegree.y), XMConvertToRadians(pOBBDesc->vDegree.z)));


	m_pOBB_Original = new BoundingOrientedBox(pOBBDesc->vCenter, pOBBDesc->vExtents, vRotation);
	m_pOBB = new BoundingOrientedBox(*m_pOBB_Original);
	
	return S_OK;
}

void CBounding_OBB::Update(_fmatrix _TransformMat)
{
	m_pOBB_Original->Transform(*m_pOBB, _TransformMat);
}

_bool CBounding_OBB::IsCollision(CCollider::COLLIDER_TYPE _eType, CBounding* _pBounding)
{
	m_bIsCol = false;

	switch (_eType)
	{
	case CCollider::TYPE_AABB:
		m_bIsCol = m_pOBB->Intersects(*((CBounding_AABB*)_pBounding)->Get_Bounding());
		break;
	case CCollider::TYPE_OBB:
		m_bIsCol = m_pOBB->Intersects(*((CBounding_OBB*)_pBounding)->Get_Bounding());
		break;
	case CCollider::TYPE_SPHERE:
		m_bIsCol = m_pOBB->Intersects(*((CBounding_Sphere*)_pBounding)->Get_Bounding());
		break;
	}

	return m_bIsCol;
}

#ifdef _DEBUG

HRESULT CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	_vector		vColor = m_bIsCol == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);

	DX::Draw(pBatch, *m_pOBB, vColor);

	return S_OK;
}

#endif

CBounding_OBB* CBounding_OBB::Create(const BOUNDING_DESC* _pDesc)
{
	CBounding_OBB* pInstance = new CBounding_OBB();

	if (FAILED(pInstance->Initialize(_pDesc)))
	{
		MSG_BOX("Fail Create : CBounding_OBB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_OBB::Free()
{
	__super::Free();

	Safe_Delete(m_pOBB_Original);
	Safe_Delete(m_pOBB);
}
