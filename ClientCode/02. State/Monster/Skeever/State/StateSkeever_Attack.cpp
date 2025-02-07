#include "framework.h"
#include "StateSkeever_Attack.h"

#include "GameInstance.h"

#include "Skeever.h"
#include "Player.h"

CStateSkeever_Attack::CStateSkeever_Attack()
{
}

HRESULT CStateSkeever_Attack::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSkeever_Attack::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	if (m_pMonster->Get_CurFrameIndex() < 20 && !m_bIsWating)
		m_pMonsterTransform->Go_Foward(_fTimeDelta, m_pMonsterNavigation);


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTransform* pTragetTransform = dynamic_cast<CTransform*>(m_pPlayer->Get_Component(TEXT("Com_Transform")));

	/* 공격 중 서로의 콜라이더가 충돌하였으면. (피격) */
	if (!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_blockidle") ||
		m_pPlayer->Get_CurState() == CPlayer::ONEHAND_ANTICIPATE)
	{
		if (pGameInstance->Collision_Enter(m_pWeaponCollider, m_pPlayerWeaponCollider))
		{
			pGameInstance->PlaySoundFile(TEXT("fx_melee_sword_other_02.wav"), CHANNEL_GUARD, 1.f);

			if (m_pPlayer->Get_IsReadyCounter())
				m_pPlayer->Set_IsCounter(true);

			m_pMonster->Play_Animation(false, "recoil");
			m_pMonster->Set_State(CSkeever::SKEEVER_STAGGER);
			m_pPlayer->Set_State(CPlayer::ONEHAND_ANTICIPATE);
			m_pPlayer->Play_Animation_All(false, "1hm_blockanticipate");
		}
	}
	else
	{
		if (!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "attack2") &&
			m_pMonster->Get_CurFrameIndex() >= 12 && m_pMonster->Get_CurFrameIndex() <= 14)
		{
			if (pGameInstance->Collision_Enter(m_pWeaponCollider, m_pPlayerBodyCollider))
			{
				pGameInstance->PlaySoundFile(TEXT("wpn_impact_blunt_dirt_01.wav"), CHANNEL_MONSTER3_ATK, 1.f);

				m_pPlayer->SetHp(-m_pMonster->GetAtk());
				m_pPlayer->Set_IsHit(true);
			}
		}
	}

	Safe_Release(pGameInstance);
}

void CStateSkeever_Attack::Late_Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 동작이 끝났을 때 */
	if (m_pMonster->Get_IsAnimationFin() && !m_bIsWating &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "attack2"))
	{
		m_bIsWating = true;
		m_pMonster->Play_Animation(true, "combatidle");
	}

	if (m_bIsWating)
	{
		if (State_Waiting(2.f, true, _fTimeDelta))
		{
			if (pGameInstance->Collision_Stay(m_pVecCollider[CSkeever::SKEEVER_COL_ATKROUND], m_pPlayerBodyCollider))
			{
				m_pMonsterTransform->Set_Speed(m_pMonster->GetWalkSpeed());
				m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
				pGameInstance->PlaySoundFile(TEXT("npc_skeever_attack_01.wav"), CHANNEL_MONSTER3, 1.f);

				m_pMonster->Set_State(CSkeever::SKEEVER_ATK);
				m_pMonster->Play_Animation(false, "attack2", 0, true);
			}
			else
			{
				m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

				m_pMonster->Set_State(CSkeever::SKEEVER_CHASE);
				m_pMonster->Play_Animation(true, "runforward");
			}

			m_bIsWating = false;
		}
	}

	Safe_Release(pGameInstance);
}

CStateSkeever_Attack* CStateSkeever_Attack::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSkeever_Attack* pInstance = new CStateSkeever_Attack();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSkeever_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSkeever_Attack::Free()
{
}
