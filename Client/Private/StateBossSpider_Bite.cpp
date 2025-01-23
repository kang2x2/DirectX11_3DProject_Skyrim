#include "framework.h"
#include "StateBossSpider_Bite.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_Bite::CStateBossSpider_Bite()
{
}

HRESULT CStateBossSpider_Bite::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_Bite::Update(_float _fTimeDelta)
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
			pGameInstance->PlaySoundFile(TEXT("fx_melee_sword_other_02.wav"), CHANNEL_GUARD, 0.35f);

			if (m_pPlayer->Get_IsReadyCounter())
			{
				m_pPlayer->Set_IsCounter(true);
				m_pPlayer->Set_IsSuccesCounter(true);
				m_pMonster->Play_Animation(false, "recoil_bite");
				m_pMonster->Set_State(CBossSpider::BOSSSPIDER_STAGGER_BITE);
				m_pMonster->Add_ParryingCount();
			}

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
				m_pPlayer->SetHp(-m_pMonster->Get_BossDesc().fBiteDamage);
				m_pPlayer->Set_IsHit(true);
			}
		}
	}

	Safe_Release(pGameInstance);
}

void CStateBossSpider_Bite::Late_Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 동작이 끝났을 때 */
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "attack_bitelow"))
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
	}

	Safe_Release(pGameInstance);
}

CStateBossSpider_Bite* CStateBossSpider_Bite::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_Bite* pInstance = new CStateBossSpider_Bite();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_Bite");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_Bite::Free()
{
}
