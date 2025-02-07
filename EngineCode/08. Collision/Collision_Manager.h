#pragma once

#include "Base.h"

BEGIN(Engine)

class CCollision_Manager final : public CBase
{
	DECLARE_SINGLETON(CCollision_Manager)

	typedef struct tagMinMaxFloat3Desc
	{
		_float3 vMinFloat3 = {};
		_float3 vMaxFloat3 = {};
	}COL_MINMAX_FLOAT3;

	typedef struct tagTargetMinMaxFloat3Desc
	{
		_float3 vMinFloat3 = {};
		_float3 vMaxFloat3 = {};
	}COL_TARGET_MINMAX_FLOAT3;

	typedef struct tagOverlapMinMaxFloat3Desc
	{
		_float3 vMinFloat3 = {};
		_float3 vMaxFloat3 = {};
	}COL_OVERLAP_OFFSET;

private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	void Collision_AABBTransition(class CCollider* _pCollider, class CCollider* _pTargetCollider);
	
	_bool Collision_Enter(class CCollider* _pCollider, class CCollider* _pTargetCollider);
	_bool Collision_Stay(class CCollider* _pCollider, class CCollider* _pTargetCollider);
	_bool Collision_Out(class CCollider* _pCollider, class CCollider* _pTargetCollider);

private:
	_bool Is_Collsion(class CCollider* _pCollider, class CCollider* _pTargetCollider);
	void MinMax_Calculator(class CCollider* _pCollider, class CCollider* _pTargetCollider, COL_MINMAX_FLOAT3& _pMinMaxFloat3Desc, COL_TARGET_MINMAX_FLOAT3& _pTargetMinMaxFloat3Desc);
	void Overlap_Calculater(COL_OVERLAP_OFFSET& _pOverlapOffSetDesc, COL_MINMAX_FLOAT3& _pMinMaxFloat3Desc, COL_TARGET_MINMAX_FLOAT3& _pTargetMinMaxFloat3Desc);

private:


public:
	virtual void Free() override;
};

END
