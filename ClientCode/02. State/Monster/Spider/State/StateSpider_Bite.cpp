#include "framework.h"
#include "StateSpider_Bite.h"

#include "GameInstance.h"

#include "Spider.h"
#include "Player.h"

CStateSpider_Bite::CStateSpider_Bite()
{
}

HRESULT CStateSpider_Bite::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSpider_Bite::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 공격 중 서로의 콜라이더가 충돌하였으면. (피격) */
	if (m_pPlayer->Get_CurState() == CPlayer::ONEHAND_BLOCK ||
		m_pPlayer->Get_CurState() == CPlayer::ONEHAND_ANTICIPATE)
	{
		if (pGameInstance->Collision_Enter(m_pMouthCollider, m_pPlayerWeaponCollider))
		{
			pGameInstance->PlaySoundFile(TEXT("fx_melee_sword_other_02.wav"), CHANNEL_GUARD, 1.f);

			if (m_pPlayer->Get_IsReadyCounter())
				m_pPlayer->Set_IsCounter(true);

			m_pMonster->Play_Animation(false, "recoil_bite");
			m_pMonster->Set_State(CSpider::SPIDER_STAGGER_B);
			m_pPlayer->Set_State(CPlayer::ONEHAND_ANTICIPATE);
			m_pPlayer->Play_Animation_All(false, "1hm_blockanticipate");
		}
	}

	else
	{
		if (!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "attack_bitelow") &&
			m_pMonster->Get_CurFrameIndex() >= 24 && m_pMonster->Get_CurFrameIndex() <= 26)
		{
			if (pGameInstance->Collision_Enter(m_pMouthCollider, m_pPlayerBodyCollider))
			{
				pGameInstance->PlaySoundFile(TEXT("wpn_impact_blunt_dirt_01.wav"), CHANNEL_MONSTER4_ATK, 1.f);

				m_pPlayer->SetHp(-5.f);
				m_pPlayer->Set_IsHit(true);
			}
		}
	}

	Safe_Release(pGameInstance);
}

void CStateSpider_Bite::Late_Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 동작이 끝났을 때 */
	if (m_pMonster->Get_IsAnimationFin() && !m_bIsWating &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "attack_bitelow"))
	{
		m_bIsWating = true;
		m_pMonster->Play_Animation(true, "combatidle");
	}

	if (m_bIsWating)
	{
		if (State_Waiting(2.f, true, _fTimeDelta))
		{
			if (pGameInstance->Collision_Stay(m_pVecCollider[CSpider::SPIDER_COL_ATKROUND], m_pPlayerBodyCollider))
			{
				m_pMonsterTransform->Set_Speed(m_pMonster->GetWalkSpeed());
				m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
				pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbite_attack_bite_01.wav"), CHANNEL_MONSTER4, 1.f);

				m_pMonster->Set_State(CSpider::SPIDER_BITE);
				m_pMonster->Play_Animation(false, "attack_bitelow", 0, true);
			}
			else
			{
				m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

				m_pMonster->Set_State(CSpider::SPIDER_CHASE);
				m_pMonster->Play_Animation(true, "forward_run");
			}

			m_bIsWating = false;
		}
	}

	Safe_Release(pGameInstance);

}

CStateSpider_Bite* CStateSpider_Bite::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSpider_Bite* pInstance = new CStateSpider_Bite();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSpider_Bite");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSpider_Bite::Free()
{
}
