#include "framework.h"
#include "StateManager_GuardCaptain.h"

#include "State_GuardCaptain.h"

#include "StateGuardCaptain_Idle.h"

#include "StateGuardCaptain_Equip.h"
#include "StateGuardCaptain_UnEquip.h"

#include "StateGuardCaptain_Walk.h"
#include "StateGuardCaptain_UnEquipWalk.h"

#include "StateGuardCaptain_Warning.h"
#include "StateGuardCaptain_Talk.h"

CStateManager_GuardCaptain::CStateManager_GuardCaptain()
{
}

HRESULT CStateManager_GuardCaptain::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	if (_pMonster == nullptr)
		return E_FAIL;

	/* Idle */
	CState_GuardCaptain* pState = CStateGuardCaptain_Idle::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecNpcState.push_back(pState);

	/* Equip */
	pState = CStateGuardCaptain_Equip::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecNpcState.push_back(pState);
	/* UnEquip */
	pState = CStateGuardCaptain_UnEquip::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecNpcState.push_back(pState);

	/* Walk */
	pState = CStateGuardCaptain_Walk::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecNpcState.push_back(pState);
	pState = CStateGuardCaptain_UnEquipWalk::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecNpcState.push_back(pState);

	/* Warning */
	pState = CStateGuardCaptain_Warning::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecNpcState.push_back(pState);
	/* Talk */
	pState = CStateGuardCaptain_Talk::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecNpcState.push_back(pState);

	m_pCurState = m_vecNpcState.front();

	return S_OK;

}

CState_GuardCaptain* CStateManager_GuardCaptain::Get_State(CNPC_GuardCaptain::GUARDCAPTAIN_STATE _eState)
{
	return m_vecNpcState[_eState];
}

HRESULT CStateManager_GuardCaptain::Set_State(CNPC_GuardCaptain::GUARDCAPTAIN_STATE _eState)
{
	m_pCurState = nullptr;
	m_pCurState = m_vecNpcState[_eState];

	return S_OK;
}

void CStateManager_GuardCaptain::Update(_float _fTimeDelta)
{
	m_pCurState->Update(_fTimeDelta);
}

void CStateManager_GuardCaptain::Late_Update(_float _fTimeDelta)
{
	m_pCurState->Late_Update(_fTimeDelta);
}

CStateManager_GuardCaptain* CStateManager_GuardCaptain::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateManager_GuardCaptain* pInstance = new CStateManager_GuardCaptain();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateManager_GuardCaptain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateManager_GuardCaptain::Free()
{
	__super::Free();

	for (auto& iter : m_vecNpcState)
	{
		Safe_Release(iter);
	}
}

