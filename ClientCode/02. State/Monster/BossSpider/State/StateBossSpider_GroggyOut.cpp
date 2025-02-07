#include "framework.h"
#include "StateBossSpider_GroggyOut.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_GroggyOut::CStateBossSpider_GroggyOut()
{
}

HRESULT CStateBossSpider_GroggyOut::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_GroggyOut::Update(_float _fTimeDelta)
{
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "getup"))
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbite_injured_03.wav"), CHANNEL_MONSTER1, 0.5f);

		Safe_Release(pGameInstance);

		m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

		m_pMonster->Set_AnimationSpeed(1.f);

		m_pMonster->Set_State(CBossSpider::BOSSSPIDER_CHASE);
		m_pMonster->Play_Animation(true, "forward_run");
	}
}

void CStateBossSpider_GroggyOut::Late_Update(_float _fTimeDelta)
{
}

CStateBossSpider_GroggyOut* CStateBossSpider_GroggyOut::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_GroggyOut* pInstance = new CStateBossSpider_GroggyOut();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_GroggyOut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_GroggyOut::Free()
{
}
