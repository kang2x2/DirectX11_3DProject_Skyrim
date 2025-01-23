#pragma once

#include "State_Monster.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CNavigation;
class CCollider;

END

BEGIN(Client)

class CState_FalmerUE abstract : public CState_Monster
{
protected:
	CState_FalmerUE();
	virtual ~CState_FalmerUE() = default;

public:
	virtual HRESULT Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);

public:
	virtual void Update(_float _fTimeDelta);
	virtual void Late_Update(_float _fTimeDelta);

protected:
	class CFalmer_UnEquip*  m_pMonster = nullptr;
	CCollider*				m_pWeaponCollider = nullptr;

public:
	virtual void Free() override;

};

END

