#include "framework.h"
#include "StateManager_GuardSupply.h"

#include "State_GuardSupply.h"

#include "StateGuardSupply_Idle.h"

#include "StateGuardSupply_Talk.h"

CStateManager_GuardSupply::CStateManager_GuardSupply()
{
}

HRESULT CStateManager_GuardSupply::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	if (_pMonster == nullptr)
		return E_FAIL;

	/* Idle */
	CState_GuardSupply* pState = CStateGuardSupply_Idle::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecNpcState.push_back(pState);
	/* Talk */
	pState = CStateGuardSupply_Talk::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecNpcState.push_back(pState);

	m_pCurState = m_vecNpcState.front();

	return S_OK;

}

CState_GuardSupply* CStateManager_GuardSupply::Get_State(CNPC_GuardSupply::GUARDSUPPLY_STATE _eState)
{
	return m_vecNpcState[_eState];
}

HRESULT CStateManager_GuardSupply::Set_State(CNPC_GuardSupply::GUARDSUPPLY_STATE _eState)
{
	m_pCurState = nullptr;
	m_pCurState = m_vecNpcState[_eState];

	return S_OK;
}

void CStateManager_GuardSupply::Update(_float _fTimeDelta)
{
	m_pCurState->Update(_fTimeDelta);
}

void CStateManager_GuardSupply::Late_Update(_float _fTimeDelta)
{
	m_pCurState->Late_Update(_fTimeDelta);
}

CStateManager_GuardSupply* CStateManager_GuardSupply::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateManager_GuardSupply* pInstance = new CStateManager_GuardSupply();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateManager_GuardSupply");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateManager_GuardSupply::Free()
{
	__super::Free();

	for (auto& iter : m_vecNpcState)
	{
		Safe_Release(iter);
	}
}

