#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Skeever.h"
#include "State_Skeever.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CNavigation;

END

BEGIN(Client)

class CStateManager_Skeever final : public CBase
{
private:
	CStateManager_Skeever();
	virtual ~CStateManager_Skeever() = default;

public:
	CState_Skeever* Get_State(CSkeever::SKEEVER_STATE _eState);

public:
	HRESULT Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);
	HRESULT Set_State(CSkeever::SKEEVER_STATE _eState);

public:
	void Update(_float _fTimeDelta);
	void Late_Update(_float _fTimeDelta);

private:
	CState_Skeever* m_pCurState = nullptr;

	vector<class CState_Skeever*> m_vecMonsterState; /* »óÅÂµé */

public:
	static CStateManager_Skeever* Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);
	virtual void Free() override;
};

END