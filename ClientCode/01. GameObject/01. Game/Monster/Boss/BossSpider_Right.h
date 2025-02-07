#pragma once

#include "Client_Defines.h"
#include "BossSpiderPart_Base.h"

BEGIN(Engine)

class CBone;
class CRenderer;
class CTransform;
class CCollider;

END

BEGIN(Client)

class CBossSpider_Right final : public CBossSpiderPart_Base
{
public:
	typedef struct tagRightPartDesc : public CBossSpiderPart_Base::PART_DESC
	{
		CBone* pSocketBone = nullptr;
		_float4x4	matSocketPivot;
	}RIGHT_DESC;

private:
	CBossSpider_Right(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CBossSpider_Right(const CBossSpider_Right& rhs);
	virtual ~CBossSpider_Right() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* _pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void		Set_PivotMatrix(_float4x4 _matPivot) { m_matSocketPivot = _matPivot; }
	const char* Get_SoketBoneName();

public:
	HRESULT Ready_Component();

private:
	CCollider* m_pColliderCom = nullptr;

	CBone* m_pSocketBone = nullptr;
	_float4x4		m_matSocketPivot;

public:
	static CBossSpider_Right* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END


