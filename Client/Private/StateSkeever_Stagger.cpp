#include "framework.h"
#include "StateSkeever_Stagger.h"

#include "GameInstance.h"

#include "Skeever.h"
#include "Player.h"

CStateSkeever_Stagger::CStateSkeever_Stagger()
{
}

HRESULT CStateSkeever_Stagger::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSkeever_Stagger::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	if (!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "recoil") &&
		m_pMonster->Get_CurFrameIndex() <= 10)
	{
		m_pMonsterTransform->Go_Backward(_fTimeDelta, m_pMonsterNavigation);
	}

}

void CStateSkeever_Stagger::Late_Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pMonster->Get_IsAnimationFin() && !m_bIsWating &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "recoil"))
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
				pGameInstance->PlaySoundFile(TEXT("npc_skeever_attack_01.wav"), CHANNEL_MONSTER3, 1.f);

				m_pMonster->Set_State(CSkeever::SKEEVER_ATK);
				m_pMonster->Play_Animation(false, "attack2");
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

CStateSkeever_Stagger* CStateSkeever_Stagger::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSkeever_Stagger* pInstance = new CStateSkeever_Stagger();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSkeever_Stagger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSkeever_Stagger::Free()
{
}
