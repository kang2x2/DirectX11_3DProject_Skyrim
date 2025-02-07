#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "State_BossSpider.h"
#include "BossSpider.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CNavigation;

END

BEGIN(Client)

class CStateManager_BossSpider final : public CBase
{
private:
	CStateManager_BossSpider();
	virtual ~CStateManager_BossSpider() = default;

public:
	CState_BossSpider* Get_State(CBossSpider::BOSSSPIDER_STATE _eState);

public:
	HRESULT Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);
	HRESULT Set_State(CBossSpider::BOSSSPIDER_STATE _eState);

public:
	void Update(_float _fTimeDelta);
	void Late_Update(_float _fTimeDelta);

private:
	CState_BossSpider* m_pCurState = nullptr;

	vector<CState_BossSpider*> m_vecMonsterState; /* »óÅÂµé */

public:
	static CStateManager_BossSpider* Create(class CGameObject* _pMonster, CGameObject* _pPlayer, class CTransform* _pMonsterTransform, class CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom);
	virtual void Free() override;
};

END