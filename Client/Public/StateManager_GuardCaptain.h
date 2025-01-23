#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "State_GuardCaptain.h"
#include "NPC_GuardCaptain.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CNavigation;

END

BEGIN(Client)

class CStateManager_GuardCaptain final : public CBase
{
private:
	CStateManager_GuardCaptain();
	virtual ~CStateManager_GuardCaptain() = default;

public:
	CState_GuardCaptain* Get_State(CNPC_GuardCaptain::GUARDCAPTAIN_STATE _eState);

public:
	HRESULT Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);
	HRESULT Set_State(CNPC_GuardCaptain::GUARDCAPTAIN_STATE _eState);

public:
	void Update(_float _fTimeDelta);
	void Late_Update(_float _fTimeDelta);

private:
	CState_GuardCaptain* m_pCurState = nullptr;

	vector<class CState_GuardCaptain*> m_vecNpcState; /* »óÅÂµé */

public:
	static CStateManager_GuardCaptain* Create(class CGameObject* _pMonster, CGameObject* _pPlayer, class CTransform* _pMonsterTransform, class CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);
	virtual void Free() override;
};

END