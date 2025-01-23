#pragma once

#include "Client_Defines.h"
#include "State_GuardCaptain.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CNavigation;

END

BEGIN(Client)

class CStateManager_GuardCaptain;
class CStateGuardCaptain_ReadyWarning final : public CState_GuardCaptain
{
private:
	CStateGuardCaptain_ReadyWarning();
	virtual ~CStateGuardCaptain_ReadyWarning() = default;

public:
	virtual		 HRESULT Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);
	virtual void Update(_float _fTimeDelta);
	virtual void Late_Update(_float _fTimeDelta);

public:
	static CStateGuardCaptain_ReadyWarning* Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);
	virtual void Free() override;
};

END

