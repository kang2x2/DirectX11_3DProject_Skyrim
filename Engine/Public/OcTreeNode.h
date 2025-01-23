#pragma once
#include "Base.h"

BEGIN(Engine)

class COcTreeNode : public CBase
{
public:
	COcTreeNode();
	virtual ~COcTreeNode() = default;

public:
	// 중점, 반지름,  
	HRESULT Initialize(_float3 _vCenter, _float3 _vRange);

	HRESULT AddChild(COcTreeNode* _pChild);
	vector<COcTreeNode*> GetChilds() { return m_vChilds; }

	HRESULT AddObject(class CGameObject* _pObject);

	void SetCull(_bool _bIsCull) { m_bIsCull = _bIsCull; }
	_bool GetCull() { return m_bIsCull; }

	HRESULT Update();
#ifdef _DEBUG
public:
	HRESULT Render(PrimitiveBatch<VertexPositionColor>* _pBatch);
#endif

public:
	HRESULT Ready_Component();

private:
	class CTransform* m_pTransformCom;

	// 바운딩 박스, 중점, 반지름, 자식 배열, 포함한 객체 배열
	class BoundingBox* m_pBoundingBox = nullptr;
	_float3 m_vCenter;
	_float3 m_vRange;
	_bool m_bIsCull;
	vector<COcTreeNode*> m_vChilds;
	vector<class CGameObject*> m_vObjects;

public:
	virtual void Free();
};

END

