#include "framework.h"
#include "StateSpider_GetUp.h"

#include "GameInstance.h"

#include "Spider.h"
#include "Player.h"

CStateSpider_GetUp::CStateSpider_GetUp()
{
}

HRESULT CStateSpider_GetUp::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSpider_GetUp::Update(_float _fTimeDelta)
{

}

void CStateSpider_GetUp::Late_Update(_float _fTimeDelta)
{
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "getup"))
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbitegiant_breathe_lp.wav"), CHANNEL_MONSTER4, 1.f);

		Safe_Release(pGameInstance);

		m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

		m_pMonster->Set_State(CSpider::SPIDER_SPIT);
		m_pMonster->Play_Animation(false, "attack_castinga");
	}
}

CStateSpider_GetUp* CStateSpider_GetUp::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSpider_GetUp* pInstance = new CStateSpider_GetUp();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSpider_GetUp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSpider_GetUp::Free()
{
}

