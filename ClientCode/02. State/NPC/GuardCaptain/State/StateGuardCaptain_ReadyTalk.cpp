#include "framework.h"
#include "StateGuardCaptain_ReadyWarning.h"

#include "GameInstance.h"

#include "NPC_GuardCaptain.h"
#include "Player.h"

CStateGuardCaptain_ReadyWarning::CStateGuardCaptain_ReadyWarning()
{
}

HRESULT CStateGuardCaptain_ReadyWarning::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateGuardCaptain_ReadyWarning::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Collision_Enter(m_pVecCollider[CNPC_GuardCaptain::GUARDCAPTAIN_COL_DETECTION], m_pPlayerBodyCollider))
	{
		m_pNpcTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

		m_pNpc->Play_Animation(false, "1hm_equip");
		m_pNpc->Set_State(CNPC_GuardCaptain::GUARDCAPTAIN_EQUIP);
	}

	Safe_Release(pGameInstance);
}

void CStateGuardCaptain_ReadyWarning::Late_Update(_float _fTimeDelta)
{

}

CStateGuardCaptain_ReadyWarning* CStateGuardCaptain_ReadyWarning::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateGuardCaptain_ReadyWarning* pInstance = new CStateGuardCaptain_ReadyWarning();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateGuardCaptain_ReadyWarning");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateGuardCaptain_ReadyWarning::Free()
{
}

