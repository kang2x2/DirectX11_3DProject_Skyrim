#include "framework.h"
#include "StateBossSpider_BerserkChop.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_BerserkChop::CStateBossSpider_BerserkChop()
{
}

HRESULT CStateBossSpider_BerserkChop::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_BerserkChop::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pMonster->Get_CurFrameIndex() == 15)
	{
		pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbitegiant_attack_b_01.wav"), CHANNEL_MONSTER2_ATK, 0.5f);
	}

	if (!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "attack_combochop") &&
		m_pMonster->Get_CurFrameIndex() >= 18 && m_pMonster->Get_CurFrameIndex() <= 19)
	{
		if (pGameInstance->Collision_Enter(m_pRightCollider, m_pPlayerBodyCollider))
		{
			m_pPlayer->SetHp(-m_pMonster->Get_BossDesc().fChopDamage);
			m_pPlayer->Set_IsHit(true);
		}
	}

	if (!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "attack_combochop") &&
		m_pMonster->Get_CurFrameIndex() >= 30 && m_pMonster->Get_CurFrameIndex() <= 32)
	{
		if (pGameInstance->Collision_Enter(m_pLeftCollider, m_pPlayerBodyCollider))
		{
			m_pPlayer->SetHp(-m_pMonster->Get_BossDesc().fChopDamage);
			m_pPlayer->Set_IsHit(true);
		}
	}

	Safe_Release(pGameInstance);
}

void CStateBossSpider_BerserkChop::Late_Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 동작이 끝났을 때 */
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "attack_combochop"))
	{
		m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

		m_pMonster->Set_State(CBossSpider::BOSSSPIDER_BACKWARD);
		m_pMonster->Play_Animation(true, "backward_run");
	}

	Safe_Release(pGameInstance);
}

CStateBossSpider_BerserkChop* CStateBossSpider_BerserkChop::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_BerserkChop* pInstance = new CStateBossSpider_BerserkChop();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_BerserkChop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_BerserkChop::Free()
{
}
