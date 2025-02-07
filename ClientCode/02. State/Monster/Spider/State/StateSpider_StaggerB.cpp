#include "framework.h"
#include "StateSpider_StaggerB.h"

#include "GameInstance.h"

#include "Spider.h"
#include "Player.h"

CStateSpider_StaggerB::CStateSpider_StaggerB()
{
}

HRESULT CStateSpider_StaggerB::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSpider_StaggerB::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CStateSpider_StaggerB::Late_Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "recoil_bite"))
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
				m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());
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

CStateSpider_StaggerB* CStateSpider_StaggerB::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSpider_StaggerB* pInstance = new CStateSpider_StaggerB();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSpider_StaggerB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSpider_StaggerB::Free()
{
}
