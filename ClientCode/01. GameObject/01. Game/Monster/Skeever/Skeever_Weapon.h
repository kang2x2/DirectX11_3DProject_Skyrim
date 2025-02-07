#pragma once

#include "Client_Defines.h"
#include "SkeeverPart_Base.h"

BEGIN(Engine)

class CBone;
class CRenderer;
class CTransform;
class CCollider;

END

BEGIN(Client)

class CSkeever_Weapon final : public CSkeeverPart_Base
{
public:
	typedef struct tagPartDesc : public CSkeeverPart_Base::PART_DESC
	{
		CBone* pSocketBone = nullptr;
		_float4x4	matSocketPivot;
	}WEAPON_DESC;

private:
	CSkeever_Weapon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkeever_Weapon(const CSkeever_Weapon& rhs);
	virtual ~CSkeever_Weapon() = default;

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
	static CSkeever_Weapon* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

