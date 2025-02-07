#pragma once

#include "State_Monster.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CNavigation;
class CCollider;

END

BEGIN(Client)

class CState_Spider abstract : public CState_Monster
{
protected:
	CState_Spider();
	virtual ~CState_Spider() = default;

public:
	virtual HRESULT Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);

public:
	virtual void Update(_float _fTimeDelta);
	virtual void Late_Update(_float _fTimeDelta);

	virtual _bool State_Waiting(_float _fWaitingTime, _bool _bIsLookAt, _float _fTimeDelta);

protected:
	class CSpider* m_pMonster = nullptr;
	CCollider*  m_pMouthCollider = nullptr;

	_float		m_fSpitCoolTime = 0.f;

public:
	virtual void Free() override;

};

END

