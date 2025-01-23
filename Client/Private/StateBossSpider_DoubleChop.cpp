#include "framework.h"
#include "StateBossSpider_DoubleChop.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_DoubleChop::CStateBossSpider_DoubleChop()
{
}

HRESULT CStateBossSpider_DoubleChop::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_DoubleChop::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pMonster->Get_CurFrameIndex() == 15)
	{
		pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbitegiant_attack_b_01.wav"), CHANNEL_MONSTER2_ATK, 0.35f);
	}

	/* 공격 중 서로의 콜라이더가 충돌하였으면. (피격) */
	if (m_pPlayer->Get_CurState() == CPlayer::ONEHAND_BLOCK ||
		m_pPlayer->Get_CurState() == CPlayer::ONEHAND_ANTICIPATE)
	{
		if (pGameInstance->Collision_Enter(m_pRightCollider, m_pPlayerWeaponCollider))
		{
			pGameInstance->PlaySoundFile(TEXT("fx_melee_sword_other_02.wav"), CHANNEL_GUARD, 0.35f);

			if (m_pPlayer->Get_IsReadyCounter())
			{
				m_pPlayer->Set_IsCounter(true);
				m_pPlayer->Set_IsSuccesCounter(true);
				m_pMonster->Play_Animation(false, "recoilrchop");
				m_pMonster->Set_State(CBossSpider::BOSSSPIDER_STAGGER_ONECHOP);
				m_pMonster->Add_ParryingCount();
			}

			m_pPlayer->Set_State(CPlayer::ONEHAND_ANTICIPATE);
			m_pPlayer->Play_Animation_All(false, "1hm_blockanticipate");
		}
		else if (pGameInstance->Collision_Enter(m_pLeftCollider, m_pPlayerWeaponCollider))
		{
			if (m_pPlayer->Get_IsReadyCounter())
			{
				m_pPlayer->Set_IsCounter(true);
				m_pPlayer->Set_IsSuccesCounter(true);
				m_pMonster->Play_Animation(false, "recoillchop");
				m_pMonster->Set_State(CBossSpider::BOSSSPIDER_STAGGER_DOUBLECHOP);
				m_pMonster->Add_ParryingCount();
			}

			m_pPlayer->Set_State(CPlayer::ONEHAND_ANTICIPATE);
			m_pPlayer->Play_Animation_All(false, "1hm_blockanticipate");
		}
	}

	else
	{
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
	}

	Safe_Release(pGameInstance);
}

void CStateBossSpider_DoubleChop::Late_Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 동작이 끝났을 때 */
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "attack_combochop"))
	{
		if (pGameInstance->Collision_Stay(m_pVecCollider[CBossSpider::BOSSSPIDER_COL_ATKROUND], m_pPlayerBodyCollider))
		{
			m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
			m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

			m_pMonster->Set_State(CBossSpider::BOSSSPIDER_BITE);
			m_pMonster->Play_Animation(false, "attack_bitelow");
		}
		else
		{
			m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

			m_pMonster->Set_State(CBossSpider::BOSSSPIDER_CHASE);
			m_pMonster->Play_Animation(true, "forward_run");
		}
	}

	Safe_Release(pGameInstance);
}

CStateBossSpider_DoubleChop* CStateBossSpider_DoubleChop::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_DoubleChop* pInstance = new CStateBossSpider_DoubleChop();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_DoubleChop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_DoubleChop::Free()
{
}
