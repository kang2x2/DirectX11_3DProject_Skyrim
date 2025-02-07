#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "State_Player.h"
#include "Player.h"

BEGIN(Client)

class CStateManager_Player final : public CBase
{
private:
	CStateManager_Player();
	virtual ~CStateManager_Player() = default;

public:
	CState_Player* Get_State(CPlayer::PLAYERSTATE _eState);

public:
	HRESULT Initialize(class CGameObject* _pPlayer, class CTransform* _pPlayerTransform, class CNavigation* _pPlayerNavigation);
	HRESULT Set_State(CPlayer::PLAYERSTATE _eState);

public:
	void Update(_float _fTimeDelta);
	void Late_Update(_float _fTimeDelta);

private:
	CState_Player* m_pCurState = nullptr;

	vector<class CState_Player*> m_vecPlayerState; /* »óÅÂµé */

public:
	static CStateManager_Player* Create(class CGameObject* _pPlayer, class CTransform* _pPlayerTransform, class CNavigation* _pPlayerNavigation);
	virtual void Free() override;
};

END