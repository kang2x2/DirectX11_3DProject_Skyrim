#pragma once

#include "Client_Defines.h"
#include "PlayerPart_Base.h"
#include "SkyrimWeapon.h"

BEGIN(Engine)

class CBone;
class CRenderer;
class CTransform;
class CCollider;

END

BEGIN(Client)

class CPlayer_Weapon final : public CPlayerPart_Base
{
public:
	typedef struct tagPartDesc : public CPlayerPart_Base::PART_DESC
	{
		CBone*		pSocketBone = nullptr;
		_float4x4	matSocketPivot;
	}WEAPON_DESC;

private:
	CPlayer_Weapon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CPlayer_Weapon(const CPlayer_Weapon& rhs);
	virtual ~CPlayer_Weapon() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* _pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void		Set_PivotMatrix(_float4x4 _matPivot) { m_matSocketPivot = _matPivot; }
	void		Set_SoketBone(CBone* _pSocketBone);
	const char* Get_SoketBoneName();

	virtual void Set_ViewType(CSkyrimWeapon::WEAPON_VIEWTYPE _eType);

	void		CheckHit_Onehand(_uint _iSourFrame, _uint _iDestFrame);

	void		Create_Spark();

public:
	HRESULT Ready_Component();

private:
	CCollider*		m_pColliderCom = nullptr;

	CBone*			m_pSocketBone = nullptr;
	_float4x4		m_matSocketPivot;

	CGameObject*	m_pWeapon = nullptr;
	class CEffect_CombatSpark* m_pSpark;
public:
	static CPlayer_Weapon* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;

};

END
