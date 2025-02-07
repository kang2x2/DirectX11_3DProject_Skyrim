#include "Collision_Manager.h"

#include "DebugDraw.h"

#include "GameInstance.h"

#include "GameObject.h"

#include "Collider.h"

#include "Bounding.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

IMPLEMENT_SINGLETON(CCollision_Manager)

CCollision_Manager::CCollision_Manager()
{
}

void CCollision_Manager::Collision_AABBTransition(CCollider* _pCollider, CCollider* _pTargetCollider)
{
	if (_pCollider->Get_ColliderType() != CCollider::TYPE_AABB ||
		_pTargetCollider->Get_ColliderType() != CCollider::TYPE_AABB)
		return;

	_bool m_bIsCol = Is_Collsion(_pCollider, _pTargetCollider);

	COL_MINMAX_FLOAT3 tMinMaxFloat3Desc;
	COL_TARGET_MINMAX_FLOAT3 tTargetMinMaxFloat3Desc;
	COL_OVERLAP_OFFSET tOverlapOffsetDesc;

	if (m_bIsCol)
	{
		MinMax_Calculator(_pCollider, _pTargetCollider, tMinMaxFloat3Desc, tTargetMinMaxFloat3Desc);
		Overlap_Calculater(tOverlapOffsetDesc, tMinMaxFloat3Desc, tTargetMinMaxFloat3Desc);

		_float4 vPos = {};
		XMStoreFloat4(&vPos, dynamic_cast<CTransform*>(_pCollider->Get_OwnerObj()->
			Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION));

		_float3 vOffset = {};
		vOffset.x = tOverlapOffsetDesc.vMaxFloat3.x - tOverlapOffsetDesc.vMinFloat3.x;
		vOffset.y = tOverlapOffsetDesc.vMaxFloat3.y - tOverlapOffsetDesc.vMinFloat3.y;
		vOffset.z = tOverlapOffsetDesc.vMaxFloat3.z - tOverlapOffsetDesc.vMinFloat3.z;
	
		/* x */
		if (vOffset.x < vOffset.z) {
			if (tMinMaxFloat3Desc.vMinFloat3.x < tOverlapOffsetDesc.vMinFloat3.x) {
				dynamic_cast<CTransform*>(_pCollider->Get_OwnerObj()->
					Get_Component(TEXT("Com_Transform")))->
					Set_State(CTransform::STATE_POSITION, XMVectorSet(vPos.x - vOffset.x, vPos.y, vPos.z, 1.f));
			}
			else {
				dynamic_cast<CTransform*>(_pCollider->Get_OwnerObj()->
					Get_Component(TEXT("Com_Transform")))->
					Set_State(CTransform::STATE_POSITION, XMVectorSet(vPos.x + vOffset.x, vPos.y, vPos.z, 1.f));
			}
		}

		/* z */
		if (vOffset.z < vOffset.x) {
			if (tMinMaxFloat3Desc.vMinFloat3.z < tOverlapOffsetDesc.vMinFloat3.z) {
				dynamic_cast<CTransform*>(_pCollider->Get_OwnerObj()->
					Get_Component(TEXT("Com_Transform")))->
					Set_State(CTransform::STATE_POSITION, XMVectorSet(vPos.x, vPos.y, vPos.z - vOffset.z, 1.f));
			}
			else {
				dynamic_cast<CTransform*>(_pCollider->Get_OwnerObj()->
					Get_Component(TEXT("Com_Transform")))->
					Set_State(CTransform::STATE_POSITION, XMVectorSet(vPos.x, vPos.y, vPos.z + vOffset.z, 1.f));
			}
		}
	}
}

_bool CCollision_Manager::Collision_Enter(CCollider* _pCollider, CCollider* _pTargetCollider)
{
	if (Is_Collsion(_pCollider, _pTargetCollider))
	{
		for (auto iter = _pCollider->Get_MapHadCol()->begin(); iter != _pCollider->Get_MapHadCol()->end(); ++iter)
		{
			if (iter->first == _pTargetCollider->Get_ColliderID())
			{
				return false;
			}
		}

		_pCollider->Add_MapHadCol(_pTargetCollider->Get_ColliderID(), _pTargetCollider);

		return true;
	}

	return false;
}
_bool CCollision_Manager::Collision_Stay(CCollider* _pCollider, CCollider* _pTargetCollider)
{
	if (Is_Collsion(_pCollider, _pTargetCollider))
	{
		for (auto iter = _pCollider->Get_MapHadCol()->begin(); iter != _pCollider->Get_MapHadCol()->end(); ++iter)
		{
			if (iter->first == _pTargetCollider->Get_ColliderID())
			{
				return true;
			}
		}
	}

	return false;
}
_bool CCollision_Manager::Collision_Out(CCollider* _pCollider, CCollider* _pTargetCollider)
{
	_uint i = 0;

	for (auto iter = _pCollider->Get_MapHadCol()->begin(); iter != _pCollider->Get_MapHadCol()->end(); ++iter)
	{
		if (iter->first == _pTargetCollider->Get_ColliderID())
		{
			if (!Is_Collsion(_pCollider, _pTargetCollider))
			{
				_pCollider->Get_MapHadCol()->erase(iter);
				_pCollider->Set_bISCol(false);
				_pTargetCollider->Set_bISCol(false);

				return true;
			}
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////

_bool CCollision_Manager::Is_Collsion(CCollider* _pCollider, CCollider* _pTargetCollider)
{
	_bool m_bIsCol = false;

	if (_pCollider->Get_ColliderType() == CCollider::TYPE_AABB)
	{
		CBounding_AABB* pAABB = dynamic_cast<CBounding_AABB*>(_pCollider->Get_Bounding());
		
		CBounding_AABB* pTargetAABB = dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding());
		CBounding_OBB* pTargetOBB = dynamic_cast<CBounding_OBB*>(_pTargetCollider->Get_Bounding());
		CBounding_Sphere* pTargetSphere = dynamic_cast<CBounding_Sphere*>(_pTargetCollider->Get_Bounding());

		switch (_pTargetCollider->Get_ColliderType())
		{
		case CCollider::TYPE_AABB:
			m_bIsCol = pAABB->Get_Bounding()->Intersects(*pTargetAABB->Get_Bounding());
			break;
		case CCollider::TYPE_OBB:
			m_bIsCol = pAABB->Get_Bounding()->Intersects(*pTargetOBB->Get_Bounding());
			break;
		case CCollider::TYPE_SPHERE:
			m_bIsCol = pAABB->Get_Bounding()->Intersects(*pTargetSphere->Get_Bounding());
			break;
		}
	}

	else if (_pCollider->Get_ColliderType() == CCollider::TYPE_OBB)
	{
		CBounding_OBB* pOBB = dynamic_cast<CBounding_OBB*>(_pCollider->Get_Bounding());

		CBounding_AABB* pTargetAABB = dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding());
		CBounding_OBB* pTargetOBB = dynamic_cast<CBounding_OBB*>(_pTargetCollider->Get_Bounding());
		CBounding_Sphere* pTargetSphere = dynamic_cast<CBounding_Sphere*>(_pTargetCollider->Get_Bounding());

		switch (_pTargetCollider->Get_ColliderType())
		{
		case CCollider::TYPE_AABB:
			m_bIsCol = pOBB->Get_Bounding()->Intersects(*pTargetAABB->Get_Bounding());
			break;
		case CCollider::TYPE_OBB:
			m_bIsCol = pOBB->Get_Bounding()->Intersects(*pTargetOBB->Get_Bounding());
			break;
		case CCollider::TYPE_SPHERE:
			m_bIsCol = pOBB->Get_Bounding()->Intersects(*pTargetSphere->Get_Bounding());
			break;
		}
	}

	else if (_pCollider->Get_ColliderType() == CCollider::TYPE_SPHERE)
	{
		CBounding_Sphere* pSphere = dynamic_cast<CBounding_Sphere*>(_pCollider->Get_Bounding());

		CBounding_AABB* pTargetAABB = dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding());
		CBounding_OBB* pTargetOBB = dynamic_cast<CBounding_OBB*>(_pTargetCollider->Get_Bounding());
		CBounding_Sphere* pTargetSphere = dynamic_cast<CBounding_Sphere*>(_pTargetCollider->Get_Bounding());

		switch (_pTargetCollider->Get_ColliderType())
		{
		case CCollider::TYPE_AABB:
			m_bIsCol = pSphere->Get_Bounding()->Intersects(*pTargetAABB->Get_Bounding());
			break;
		case CCollider::TYPE_OBB:
			m_bIsCol = pSphere->Get_Bounding()->Intersects(*pTargetOBB->Get_Bounding());
			break;
		case CCollider::TYPE_SPHERE:
			m_bIsCol = pSphere->Get_Bounding()->Intersects(*pTargetSphere->Get_Bounding());
			break;
		}
	}

	if (m_bIsCol)
	{
		_pCollider->Set_bISCol(true);
		_pTargetCollider->Set_bISCol(true);
	}
	else if (!m_bIsCol)
	{
		_pCollider->Set_bISCol(false);
		_pTargetCollider->Set_bISCol(false);
	}

	return m_bIsCol;
}

void CCollision_Manager::MinMax_Calculator(class CCollider* _pCollider, class CCollider* _pTargetCollider, COL_MINMAX_FLOAT3& _pMinMaxFloat3Desc, COL_TARGET_MINMAX_FLOAT3& _pTargetMinMaxFloat3Desc)
{
	/* x */
	_pMinMaxFloat3Desc.vMinFloat3.x = dynamic_cast<CBounding_AABB*>(_pCollider->Get_Bounding())->Get_Bounding()->Center.x -
		dynamic_cast<CBounding_AABB*>(_pCollider->Get_Bounding())->Get_Bounding()->Extents.x;
	_pMinMaxFloat3Desc.vMaxFloat3.x = dynamic_cast<CBounding_AABB*>(_pCollider->Get_Bounding())->Get_Bounding()->Center.x +
		dynamic_cast<CBounding_AABB*>(_pCollider->Get_Bounding())->Get_Bounding()->Extents.x;
	_pTargetMinMaxFloat3Desc.vMinFloat3.x = dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding())->Get_Bounding()->Center.x -
		dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding())->Get_Bounding()->Extents.x;
	_pTargetMinMaxFloat3Desc.vMaxFloat3.x = dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding())->Get_Bounding()->Center.x +
		dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding())->Get_Bounding()->Extents.x;

	/* y */
	_pMinMaxFloat3Desc.vMinFloat3.y = dynamic_cast<CBounding_AABB*>(_pCollider->Get_Bounding())->Get_Bounding()->Center.y -
		dynamic_cast<CBounding_AABB*>(_pCollider->Get_Bounding())->Get_Bounding()->Extents.y;
	_pMinMaxFloat3Desc.vMaxFloat3.y = dynamic_cast<CBounding_AABB*>(_pCollider->Get_Bounding())->Get_Bounding()->Center.y +
		dynamic_cast<CBounding_AABB*>(_pCollider->Get_Bounding())->Get_Bounding()->Extents.y;
	_pTargetMinMaxFloat3Desc.vMinFloat3.y = dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding())->Get_Bounding()->Center.y -
		dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding())->Get_Bounding()->Extents.y;
	_pTargetMinMaxFloat3Desc.vMaxFloat3.y = dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding())->Get_Bounding()->Center.y +
		dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding())->Get_Bounding()->Extents.y;

	/* z */
	_pMinMaxFloat3Desc.vMinFloat3.z = dynamic_cast<CBounding_AABB*>(_pCollider->Get_Bounding())->Get_Bounding()->Center.z -
		dynamic_cast<CBounding_AABB*>(_pCollider->Get_Bounding())->Get_Bounding()->Extents.z;
	_pMinMaxFloat3Desc.vMaxFloat3.z = dynamic_cast<CBounding_AABB*>(_pCollider->Get_Bounding())->Get_Bounding()->Center.z +
		dynamic_cast<CBounding_AABB*>(_pCollider->Get_Bounding())->Get_Bounding()->Extents.z;
	_pTargetMinMaxFloat3Desc.vMinFloat3.z = dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding())->Get_Bounding()->Center.z -
		dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding())->Get_Bounding()->Extents.z;
	_pTargetMinMaxFloat3Desc.vMaxFloat3.z = dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding())->Get_Bounding()->Center.z +
		dynamic_cast<CBounding_AABB*>(_pTargetCollider->Get_Bounding())->Get_Bounding()->Extents.z;
}

void CCollision_Manager::Overlap_Calculater(COL_OVERLAP_OFFSET& _pOverlapOffSetDesc, COL_MINMAX_FLOAT3& _pMinMaxFloat3Desc, COL_TARGET_MINMAX_FLOAT3& _pTargetMinMaxFloat3Desc)
{
	_pOverlapOffSetDesc.vMinFloat3.x = max(_pMinMaxFloat3Desc.vMinFloat3.x, _pTargetMinMaxFloat3Desc.vMinFloat3.x);
	_pOverlapOffSetDesc.vMaxFloat3.x = min(_pMinMaxFloat3Desc.vMaxFloat3.x, _pTargetMinMaxFloat3Desc.vMaxFloat3.x);
	_pOverlapOffSetDesc.vMinFloat3.y = max(_pMinMaxFloat3Desc.vMinFloat3.y, _pTargetMinMaxFloat3Desc.vMinFloat3.y);
	_pOverlapOffSetDesc.vMaxFloat3.y = min(_pMinMaxFloat3Desc.vMaxFloat3.y, _pTargetMinMaxFloat3Desc.vMaxFloat3.y);
	_pOverlapOffSetDesc.vMinFloat3.z = max(_pMinMaxFloat3Desc.vMinFloat3.z, _pTargetMinMaxFloat3Desc.vMinFloat3.z);
	_pOverlapOffSetDesc.vMaxFloat3.z = min(_pMinMaxFloat3Desc.vMaxFloat3.z, _pTargetMinMaxFloat3Desc.vMaxFloat3.z);
}

void CCollision_Manager::Free()
{
	__super::Free();
}
