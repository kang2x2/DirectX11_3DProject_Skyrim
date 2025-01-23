#include "framework.h"
#include "StateGuardCaptain_UnEquipWalk.h"

#include "GameInstance.h"

#include "NPC_GuardCaptain.h"
#include "Player.h"

CStateGuardCaptain_UnEquipWalk::CStateGuardCaptain_UnEquipWalk()
{
}

HRESULT CStateGuardCaptain_UnEquipWalk::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateGuardCaptain_UnEquipWalk::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_pNpcTransform->Go_Foward(_fTimeDelta, m_pNpcNavigation);
	m_pNpcTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* Talk 범위에 들어왔을 때 */
	if (pGameInstance->Collision_Enter(m_pVecCollider[CNPC_GuardCaptain::GUARDCAPTAIN_COL_DIALOG], m_pPlayerBodyCollider))
	{
		m_pNpc->Start_Talk();

		m_pNpc->Set_State(CNPC_GuardCaptain::GUARDCAPTAIN_TALK);
		m_pNpc->Play_Animation(true, "dialogueresponsepositivea");
	}

	Safe_Release(pGameInstance);
}

void CStateGuardCaptain_UnEquipWalk::Late_Update(_float _fTimeDelta)
{
}

CStateGuardCaptain_UnEquipWalk* CStateGuardCaptain_UnEquipWalk::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateGuardCaptain_UnEquipWalk* pInstance = new CStateGuardCaptain_UnEquipWalk();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateGuardCaptain_UnEquipWalk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateGuardCaptain_UnEquipWalk::Free()
{
}
