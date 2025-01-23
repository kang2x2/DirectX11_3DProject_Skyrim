#pragma once

#include "Component.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum COLLIDER_TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

private:
	CCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	COLLIDER_TYPE		Get_ColliderType() { return m_eColliderType; }
	class CBounding*	Get_Bounding() { return m_pBounding; }

public:
	virtual HRESULT Initialize_ProtoType(COLLIDER_TYPE _eType);
	virtual HRESULT Initialize_Clone(void* pArg);

public:
	void Update(_fmatrix _TransformMat);
	void Late_Update(_float _fTimeDelta);

	/* 충돌 체크 */
	_bool IsCollision(CCollider* _pTragetCollider);
	/* 정보 변경 */
	void Set_ColliderDesc(_float _fExtents);
	/* 중점 반환 */
	_vector Get_WorldCenter();

#ifdef _DEBUG
public:
	HRESULT Render();
#endif

public:
	/* 충돌체를 가지고 있는 주인 객체 관련. */
	virtual CGameObject* Get_OwnerObj() { return m_pOwnerObj; }
	virtual void		 Set_OwnerObj(class CGameObject* _pObj);

	/* 충돌체가 가지는 ID 반환. */
	virtual _uint		 Get_ColliderID() { return m_iColliderID; }

	/* 충돌했던 충돌체들을 보관하는 vector 관련. */
	virtual map<_uint, CCollider*>* Get_MapHadCol() { return &m_mapHadCol; }
	virtual void Add_MapHadCol(_uint _iColID, CCollider* _pCollider)
	{
		m_mapHadCol.emplace(_iColID, _pCollider);
	}
	
	/* Render를 위한 bool 세팅 함수. */
	virtual void Set_bISCol(_bool _bIsCol);

private:
	CGameObject* m_pOwnerObj = nullptr;

	class CBounding*	m_pBounding = nullptr;
	COLLIDER_TYPE		m_eColliderType = TYPE_END;

	/* 모든 콜라이더가 가지고 있을 ID */
	static _uint				m_iIDIndex;
	_uint						m_iColliderID = 0;

	map<_uint, CCollider*> 		m_mapHadCol;

#ifdef _DEBUG
private:
	/* Vertex, Index Buffer를 생성하고 그리는 기능을 제공하는 객체.  */
	PrimitiveBatch<DirectX::VertexPositionColor>* m_pBatch =  nullptr;
	BasicEffect*								  m_pEffect =  nullptr;
	ID3D11InputLayout*							  m_pInputLayout = nullptr;
	ID3D11DepthStencilState*					  m_pDSState = nullptr;

#endif

public:
	static CCollider* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, COLLIDER_TYPE _eType);
	virtual CComponent* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

