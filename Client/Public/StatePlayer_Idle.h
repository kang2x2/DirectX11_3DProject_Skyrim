#pragma once

#include "Client_Defines.h"
#include "State_Player.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CNavigation;

END

BEGIN(Client)

class CStateManager_Player;
class CStatePlayer_Idle final : public CState_Player
{
private:
	CStatePlayer_Idle();
	virtual ~CStatePlayer_Idle() = default;

public:
	virtual		 HRESULT Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation);
	virtual void Update(_float _fTimeDelta);
	virtual void Late_Update(_float _fTimeDelta);
	virtual void Key_Input(_float _fTimeDelta);

public:
	static CStatePlayer_Idle* Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation);
	virtual void Free() override;
};

END

