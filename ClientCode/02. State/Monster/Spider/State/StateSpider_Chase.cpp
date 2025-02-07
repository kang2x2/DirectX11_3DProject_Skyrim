#include "framework.h"
#include "StateSpider_Chase.h"

#include "GameInstance.h"

#include "Spider.h"
#include "Player.h"

CStateSpider_Chase::CStateSpider_Chase()
{
}

HRESULT CStateSpider_Chase::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSpider_Chase::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_pMonsterTransform->Go_Foward(_fTimeDelta, m_pMonsterNavigation);
	m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->CheckPlaySoundFile(TEXT("npc_spiderfrostbite_foot_01.wav"), CHANNEL_MONSTER4, 1.f);

	/* RunPowerAtk 범위에 들어왔을 때 */
	if (pGameInstance->Collision_Enter(m_pVecCollider[CSpider::SPIDER_COL_ATKROUND], m_pPlayerBodyCollider))
	{
		pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbite_attack_bite_01.wav"), CHANNEL_MONSTER4, 1.f);

		m_pMonster->Set_State(CSpider::SPIDER_CHARGE);
		m_pMonster->Play_Animation(false, "attack_forwardjump");
	}

	Safe_Release(pGameInstance);
}

void CStateSpider_Chase::Late_Update(_float _fTimeDelta)
{

}

CStateSpider_Chase* CStateSpider_Chase::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSpider_Chase* pInstance = new CStateSpider_Chase();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSpider_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSpider_Chase::Free()
{
}

