#include "framework.h"
#include "StateSpider_Backward.h"

#include "GameInstance.h"

#include "Spider.h"
#include "Player.h"

CStateSpider_Backward::CStateSpider_Backward()
{
}

HRESULT CStateSpider_Backward::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSpider_Backward::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_fBackTime += _fTimeDelta;

	m_pMonsterTransform->Go_Backward(_fTimeDelta, m_pMonsterNavigation);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->CheckPlaySoundFile(TEXT("npc_spiderfrostbite_foot_01.wav"), CHANNEL_MONSTER4, 1.f);

	/* 일정 거리 뒤로 이동했을 때 */
	if (m_fBackTime > 2.f)
	{
		pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbite_attack_bite_01.wav"), CHANNEL_MONSTER4, 1.f);

		m_pMonster->Set_State(CSpider::SPIDER_SPIT);
		m_pMonster->Play_Animation(false, "attack_castinga");

		m_fBackTime = 2.f - m_fBackTime;
	}

	Safe_Release(pGameInstance);

}

void CStateSpider_Backward::Late_Update(_float _fTimeDelta)
{

}

CStateSpider_Backward* CStateSpider_Backward::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSpider_Backward* pInstance = new CStateSpider_Backward();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSpider_Backward");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSpider_Backward::Free()
{
}

