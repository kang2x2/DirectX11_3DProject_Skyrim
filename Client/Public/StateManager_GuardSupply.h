#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "State_GuardSupply.h"
#include "NPC_GuardSupply.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CNavigation;

END

BEGIN(Client)

class CStateManager_GuardSupply final : public CBase
{
private:
	CStateManager_GuardSupply();
	virtual ~CStateManager_GuardSupply() = default;

public:
	CState_GuardSupply* Get_State(CNPC_GuardSupply::GUARDSUPPLY_STATE _eState);

public:
	HRESULT Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);
	HRESULT Set_State(CNPC_GuardSupply::GUARDSUPPLY_STATE _eState);

public:
	void Update(_float _fTimeDelta);
	void Late_Update(_float _fTimeDelta);

private:
	CState_GuardSupply* m_pCurState = nullptr;

	vector<class CState_GuardSupply*> m_vecNpcState; /* »óÅÂµé */

public:
	static CStateManager_GuardSupply* Create(class CGameObject* _pMonster, CGameObject* _pPlayer, class CTransform* _pMonsterTransform, class CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);
	virtual void Free() override;
};

END