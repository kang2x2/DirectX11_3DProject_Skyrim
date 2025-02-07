#pragma once

#include "Client_Defines.h"
#include "GuardPart_Base.h"

BEGIN(Engine)

class CBone;
class CRenderer;
class CTransform;
class CCollider;

END

BEGIN(Client)

class CGuard_Weapon final : public CGuardPart_Base
{
public:
	typedef struct tagPartDesc : public CGuardPart_Base::PART_DESC
	{
		CBone* pSocketBone = nullptr;
		_float4x4	matSocketPivot;
	}WEAPON_DESC;

private:
	CGuard_Weapon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CGuard_Weapon(const CGuard_Weapon& rhs);
	virtual ~CGuard_Weapon() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* _pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void		Set_PivotMatrix(_float4x4 _matPivot) { m_matSocketPivot = _matPivot; }
	const char* Get_SoketBoneName();
	void		Set_SoketBone(CBone* _pSocketBone);

public:
	HRESULT Ready_Component();

private:
	CCollider* m_pColliderCom = nullptr;

	CBone* m_pSocketBone = nullptr;
	_float4x4		m_matSocketPivot;

	_float		m_fCurDissloveTime = 0.f;
	_float		m_fDissloveTime = 0.f;

public:
	static CGuard_Weapon* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END


