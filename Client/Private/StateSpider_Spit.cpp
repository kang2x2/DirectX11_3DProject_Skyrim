#include "framework.h"
#include "StateSpider_Spit.h"

#include "GameInstance.h"

#include "Spider.h"
#include "Player.h"

CStateSpider_Spit::CStateSpider_Spit()
{
}

HRESULT CStateSpider_Spit::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSpider_Spit::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	if (m_pMonster->Get_CurFrameIndex() == 8 &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "attack_castinga"))
	{
		_vector vSpitPos = m_pMonsterTransform->Get_State(CTransform::STATE_POSITION);

		// 거미줄 발사(각도도 추가해야 할 듯 하다.)
		pGameInstance->Add_CloneObject(g_curLevel,
			TEXT("Projectile"), TEXT("ProtoType_GameObject_CProjectile_Web"), &vSpitPos);
	
		// 막으면 퍼펙트 가드
	}

	Safe_Release(pGameInstance);
}

void CStateSpider_Spit::Late_Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 동작이 끝났을 때 */
	if (m_pMonster->Get_IsAnimationFin() && !m_bIsWating &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "attack_castinga"))
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
				m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
				m_pMonsterTransform->Set_Speed(m_pMonster->GetWalkSpeed());
				pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbite_attack_bite_01.wav"), CHANNEL_MONSTER4, 1.f);

				m_pMonster->Set_State(CSpider::SPIDER_BITE);
				m_pMonster->Play_Animation(false, "attack_bitelow");
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

CStateSpider_Spit* CStateSpider_Spit::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSpider_Spit* pInstance = new CStateSpider_Spit();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSpider_Spit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSpider_Spit::Free()
{
}

