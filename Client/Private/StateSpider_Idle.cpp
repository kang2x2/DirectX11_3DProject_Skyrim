#include "framework.h"
#include "StateSpider_Idle.h"

#include "GameInstance.h"

#include "Spider.h"
#include "Player.h"

CStateSpider_Idle::CStateSpider_Idle()
{
}

HRESULT CStateSpider_Idle::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSpider_Idle::Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Collision_Enter(m_pVecCollider[CSpider::SPIDER_COL_DETECTION], m_pPlayerBodyCollider))
	{
		pGameInstance->Collision_Enter(m_pVecCollider[CSpider::SPIDER_COL_MISSDETECTION], m_pPlayerBodyCollider);

		m_pMonster->Play_Animation(false, "getup");
		m_pMonster->Set_State(CSpider::SPIDER_GETUP);
	}

	Safe_Release(pGameInstance);
}

void CStateSpider_Idle::Late_Update(_float _fTimeDelta)
{

}

CStateSpider_Idle* CStateSpider_Idle::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSpider_Idle* pInstance = new CStateSpider_Idle();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSpider_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSpider_Idle::Free()
{
}

