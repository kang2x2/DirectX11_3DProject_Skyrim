#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CNavigation;
class CCollider;

END

BEGIN(Client)

class CState_Monster abstract : public CBase
{
protected:
	CState_Monster();
	virtual ~CState_Monster() = default;

public:
	virtual HRESULT Initialize(CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);

public:
	virtual void Update(_float _fTimeDelta);
	virtual void Late_Update(_float _fTimeDelta);

protected:
	CTransform* m_pMonsterTransform = nullptr;
	CNavigation* m_pMonsterNavigation = nullptr;

	vector<CCollider*>  m_pVecCollider;
	CCollider* m_pWeaponCollider = nullptr;

	class CPlayer* m_pPlayer = nullptr;
	CTransform* m_pPlayerTransform = nullptr;
	CCollider*  m_pPlayerBodyCollider = nullptr;
	CCollider*  m_pPlayerWeaponCollider = nullptr;

	_bool		m_bIsWating = false;
	_float		m_fWaitingTime = 0.f;
public:
	virtual void Free() override;

};

END

