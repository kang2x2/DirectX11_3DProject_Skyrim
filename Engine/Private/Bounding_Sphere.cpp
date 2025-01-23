#include "Bounding_Sphere.h"
#include "DebugDraw.h"

#include "Bounding_AABB.h"
#include "Bounding_OBB.h"

CBounding_Sphere::CBounding_Sphere()
{
}

_vector CBounding_Sphere::Get_WorldCenter()
{
	return 	{ m_pSphere->Center.x, m_pSphere->Center.y, m_pSphere->Center.z, 1.f };
}

HRESULT CBounding_Sphere::Initialize(const BOUNDING_DESC* _pDesc)
{
	if (FAILED(__super::Initialize(_pDesc)))
		return E_FAIL;

	BOUNDING_SPHERE_DESC* pSphereDesc = (BOUNDING_SPHERE_DESC*)_pDesc;

	m_pSphere_Original = new BoundingSphere(pSphereDesc->vCenter, pSphereDesc->fRadius);
	m_pSphere = new BoundingSphere(*m_pSphere_Original);

	return S_OK;
}

void CBounding_Sphere::Update(_fmatrix _TransformMat)
{
	m_pSphere_Original->Transform(*m_pSphere, _TransformMat);
}

_bool CBounding_Sphere::IsCollision(CCollider::COLLIDER_TYPE _eType, CBounding* _pBounding)
{
	m_bIsCol = false;

	switch (_eType)
	{
	case CCollider::TYPE_AABB:
		m_bIsCol = m_pSphere->Intersects(*((CBounding_AABB*)_pBounding)->Get_Bounding());
		break;
	case CCollider::TYPE_OBB:
		m_bIsCol = m_pSphere->Intersects(*((CBounding_OBB*)_pBounding)->Get_Bounding());
		break;
	case CCollider::TYPE_SPHERE:
		m_bIsCol = m_pSphere->Intersects(*((CBounding_Sphere*)_pBounding)->Get_Bounding());
		break;
	}

	return m_bIsCol;
}

#ifdef _DEBUG

HRESULT CBounding_Sphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	_vector		vColor = m_bIsCol == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);

	DX::Draw(pBatch, *m_pSphere, vColor);

	return S_OK;
}

#endif

CBounding_Sphere* CBounding_Sphere::Create(const BOUNDING_DESC* _pDesc)
{
	CBounding_Sphere* pInstnace = new CBounding_Sphere();
	
	if (FAILED(pInstnace->Initialize(_pDesc)))
	{
		MSG_BOX("Fail Create : CBounding_Sphere");
		Safe_Release(pInstnace);
	}

	return pInstnace;
}

void CBounding_Sphere::Free()
{
	__super::Free();

	Safe_Delete(m_pSphere_Original);
	Safe_Delete(m_pSphere);
}
