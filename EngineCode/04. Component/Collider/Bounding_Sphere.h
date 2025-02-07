#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_Sphere final : public CBounding
{
public:
	typedef struct tagBoundingSphereDesc : public CBounding::BOUNDING_DESC
	{
		_float		fRadius;
	}BOUNDING_SPHERE_DESC;

private:
	CBounding_Sphere();
	virtual ~CBounding_Sphere() = default;

public:
	const BoundingSphere* Get_Bounding() const { return m_pSphere; }
	virtual _vector Get_WorldCenter();

public:
	virtual HRESULT Initialize(const BOUNDING_DESC* _pDesc) override;
	virtual void Update(_fmatrix _TransformMat) override;

public:
	virtual _bool IsCollision(CCollider::COLLIDER_TYPE _eType, CBounding* _pBounding) override;

#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* _pBatch) override;
#endif


private:
	BoundingSphere* m_pSphere_Original = nullptr;
	BoundingSphere* m_pSphere = nullptr;

public:
	static CBounding_Sphere* Create(const BOUNDING_DESC* _pDesc);
	virtual void Free() override;
};

END

