#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "State_Spider.h"
#include "Spider.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CNavigation;

END

BEGIN(Client)

class CStateManager_Spider final : public CBase
{
private:
	CStateManager_Spider();
	virtual ~CStateManager_Spider() = default;

public:
	CState_Spider* Get_State(CSpider::SPIDER_STATE _eState);

public:
	HRESULT Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);
	HRESULT Set_State(CSpider::SPIDER_STATE _eState);

public:
	void Update(_float _fTimeDelta);
	void Late_Update(_float _fTimeDelta);

private:
	CState_Spider* m_pCurState = nullptr;

	vector<class CState_Spider*> m_vecMonsterState; /* »óÅÂµé */

public:
	static CStateManager_Spider* Create(class CGameObject* _pMonster, CGameObject* _pPlayer, class CTransform* _pMonsterTransform, class CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);
	virtual void Free() override;
};

END