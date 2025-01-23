#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_AABB final : public CBounding
{
public:
	typedef struct tagBoundingAABBDesc : public CBounding::BOUNDING_DESC
	{
		_float3 vExtents;
	}BOUNDING_AABB_DESC;

private:
	CBounding_AABB();
	virtual ~CBounding_AABB() = default;

public:
	const BoundingBox* Get_Bounding() const { return m_pAABB; }
	virtual _vector Get_WorldCenter();

public:
	virtual HRESULT Initialize(const BOUNDING_DESC* _pDesc) override;
	virtual void Update(_fmatrix _TransformMat) override;

public:
	virtual _bool IsCollision(CCollider::COLLIDER_TYPE _eType, CBounding* _pBounding) override;
	virtual void Set_ColliderDesc(_float _fExtents);

#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* _pBatch) override;
#endif


private:
	BoundingBox* m_pAABB_Original = nullptr;
	BoundingBox* m_pAABB = nullptr;

public:
	static CBounding_AABB* Create(const BOUNDING_DESC* _pDesc);
	virtual void Free() override;
};

END