#include "framework.h"
#include "StateGuardSupply_Idle.h"

#include "GameInstance.h"

#include "NPC_GuardSupply.h"
#include "Player.h"

CStateGuardSupply_Idle::CStateGuardSupply_Idle()
{
}

HRESULT CStateGuardSupply_Idle::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateGuardSupply_Idle::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Collision_Enter(m_pVecCollider[CNPC_GuardSupply::GUARDSUPPLY_COL_DIALOG], m_pPlayerBodyCollider) ||
		pGameInstance->Collision_Stay(m_pVecCollider[CNPC_GuardSupply::GUARDSUPPLY_COL_DIALOG], m_pPlayerBodyCollider))
	{
		m_pNpc->Set_VisibleTalkIcon(true);

		if (pGameInstance->Get_DIKeyDown('E'))
		{
			m_pNpc->Start_Talk();

			m_pNpc->Play_Animation(false, "dialogueresponsepositivea");
			m_pNpc->Set_State(CNPC_GuardSupply::GUARDSUPPLY_TALK);
		}
	}
	else
	{
		m_pNpc->Set_VisibleTalkIcon(false);
	}

	Safe_Release(pGameInstance);
}

void CStateGuardSupply_Idle::Late_Update(_float _fTimeDelta)
{

}

CStateGuardSupply_Idle* CStateGuardSupply_Idle::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateGuardSupply_Idle* pInstance = new CStateGuardSupply_Idle();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateGuardSupply_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateGuardSupply_Idle::Free()
{
}

