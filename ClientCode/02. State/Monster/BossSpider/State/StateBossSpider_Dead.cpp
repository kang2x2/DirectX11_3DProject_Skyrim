#include "framework.h"
#include "StateBossSpider_Dead.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_Dead::CStateBossSpider_Dead()
{
}

HRESULT CStateBossSpider_Dead::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_Dead::Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 동작이 끝났을 때 */
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "death"))
	{
		m_pMonster->Set_State(CBossSpider::BOSSSPIDER_RELEASE);
		m_pMonster->Play_Animation(false, "bleedout_idle");
	}

	Safe_Release(pGameInstance);
}

void CStateBossSpider_Dead::Late_Update(_float _fTimeDelta)
{
}

CStateBossSpider_Dead* CStateBossSpider_Dead::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_Dead* pInstance = new CStateBossSpider_Dead();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_Dead::Free()
{
}
