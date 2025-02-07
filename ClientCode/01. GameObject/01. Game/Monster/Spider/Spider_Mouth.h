#pragma once

#include "Client_Defines.h"
#include "SpiderPart_Base.h"

BEGIN(Engine)

class CBone;
class CRenderer;
class CTransform;
class CCollider;

END

BEGIN(Client)

class CSpider_Mouth final : public CSpiderPart_Base
{
public:
	typedef struct tagMouthPartDesc : public CSpiderPart_Base::PART_DESC
	{
		CBone* pSocketBone = nullptr;
		_float4x4	matSocketPivot;
	}MOUTH_DESC;

private:
	CSpider_Mouth(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSpider_Mouth(const CSpider_Mouth& rhs);
	virtual ~CSpider_Mouth() = default;

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
	static CSpider_Mouth* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END


