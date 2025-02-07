#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "State_FalmerOH.h"
#include "Falmer_OneHand.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CNavigation;

END

BEGIN(Client)

class CStateManager_FalmerOH final : public CBase
{
private:
	CStateManager_FalmerOH();
	virtual ~CStateManager_FalmerOH() = default;

public:
	CState_FalmerOH* Get_State(CFalmer_OneHand::FALMEROH_STATE _eState);

public:
	HRESULT Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);
	HRESULT Set_State(CFalmer_OneHand::FALMEROH_STATE _eState);

public:
	void Update(_float _fTimeDelta);
	void Late_Update(_float _fTimeDelta);

private:
	CState_FalmerOH* m_pCurState = nullptr;

	vector<class CState_FalmerOH*> m_vecMonsterState; /* »óÅÂµé */

public:
	static CStateManager_FalmerOH* Create(class CGameObject* _pMonster, CGameObject* _pPlayer, class CTransform* _pMonsterTransform, class CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);
	virtual void Free() override;
};

END