#include "framework.h"
#include "StateBossSpider_Charge.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_Charge::CStateBossSpider_Charge()
{
}

HRESULT CStateBossSpider_Charge::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_Charge::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	if (m_pMonster->Get_CurFrameIndex() <= 25 && !m_bIsWating)
		m_pMonsterTransform->Go_Foward(_fTimeDelta, m_pMonsterNavigation);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 공격 중 서로의 콜라이더가 충돌하였으면. (피격) */
	if (m_pPlayer->Get_CurState() == CPlayer::ONEHAND_BLOCK ||
		m_pPlayer->Get_CurState() == CPlayer::ONEHAND_ANTICIPATE)
	{
		if (pGameInstance->Collision_Enter(m_pMouthCollider, m_pPlayerWeaponCollider))
		{
			if (m_pPlayer->Get_IsReadyCounter())
			{
				pGameInstance->PlaySoundFile(TEXT("fx_melee_sword_other_02.wav"), CHANNEL_GUARD, 0.5f);

				m_pPlayer->Set_IsCounter(true);
				m_pPlayer->Set_IsSuccesCounter(true);
				m_pMonster->Play_Animation(false, "recoil_jump");
				m_pMonster->Set_State(CBossSpider::BOSSSPIDER_STAGGER_CHARGE);
				m_pPlayer->Set_State(CPlayer::ONEHAND_ANTICIPATE);
				m_pPlayer->Play_Animation_All(false, "1hm_blockanticipate");
			}
			/* 플레이어 크게 비틀거리게 */
			else
			{
				pGameInstance->PlaySoundFile(TEXT("wpn_impact_axelarge_flesh_01.wav"), CHANNEL_GUARD, 0.5f);

				if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
				{
					m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());
					m_pPlayer->Set_State(CPlayer::PLAYER_1STSTAGGER);
					m_pPlayer->Play_Animation_All(false, "staggercamera");
				}
				else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
				{
					m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());
					m_pPlayer->Set_State(CPlayer::PLAYER_3STSTAGGER);
					m_pPlayer->Play_Animation_All(false, "1hm_staggerbacklargest");
				}
			}
		}
	}

	else
	{
		if (!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "attack_forwardjump"))
		{
			if (pGameInstance->Collision_Enter(m_pMouthCollider, m_pPlayerBodyCollider))
			{
				pGameInstance->PlaySoundFile(TEXT("wpn_impact_axelarge_flesh_01.wav"), CHANNEL_GUARD, 0.5f);

				/* 플레이어 크게 비틀거리게 */
				if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
				{
					m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());
					m_pPlayer->Set_State(CPlayer::PLAYER_1STSTAGGER);
					m_pPlayer->Play_Animation_All(false, "staggercamera");
				}
				else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
				{
					m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());
					m_pPlayer->Set_State(CPlayer::PLAYER_3STSTAGGER);
					m_pPlayer->Play_Animation_All(false, "1hm_staggerbacklargest");
				}

				m_pPlayer->SetHp(-m_pMonster->Get_BossDesc().fChargeDamage);
				m_pPlayer->Set_IsHit(true);
			}
		}
	}

	Safe_Release(pGameInstance);
}

void CStateBossSpider_Charge::Late_Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 동작이 끝났을 때 */
	if (m_pMonster->Get_IsAnimationFin() && !m_bIsWating &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "attack_forwardjump"))
	{
		m_bIsWating = true;
		m_pMonster->Play_Animation(true, "combatidle");
	}

	if (m_bIsWating)
	{
		if (State_Waiting(1.3f, true, _fTimeDelta))
		{
			if (pGameInstance->Collision_Stay(m_pVecCollider[CBossSpider::BOSSSPIDER_COL_ATKROUND], m_pPlayerBodyCollider))
			{
				m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
				m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

				m_pMonster->Set_State(CBossSpider::BOSSSPIDER_ONECHOP);
				m_pMonster->Play_Animation(false, "attack_rightchop");
			}
			else
			{
				m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

				m_pMonster->Set_State(CBossSpider::BOSSSPIDER_CHASE);
				m_pMonster->Play_Animation(true, "forward_run");
			}

			m_bIsWating = false;
		}
	}

	Safe_Release(pGameInstance);
}

CStateBossSpider_Charge* CStateBossSpider_Charge::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_Charge* pInstance = new CStateBossSpider_Charge();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_Charge::Free()
{
}
