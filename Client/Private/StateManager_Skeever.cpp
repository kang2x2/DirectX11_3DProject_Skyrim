#include "framework.h"
#include "StateManager_Skeever.h"

#include "State_Skeever.h"

#include "StateSkeever_Idle.h"
#include "StateSkeever_Chase.h"
#include "StateSkeever_Detection.h"
#include "StateSkeever_Warning.h"
#include "StateSkeever_Return.h"

#include "StateSkeever_Attack.h"
#include "StateSkeever_Charge.h"

#include "StateSkeever_Stagger.h"

#include "StateSkeever_Dead.h"

CStateManager_Skeever::CStateManager_Skeever()
{
}

HRESULT CStateManager_Skeever::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	if (_pMonster == nullptr)
		return E_FAIL;

	/* Idle */
	CState_Skeever* pState = CStateSkeever_Idle::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Warning */
	pState = CStateSkeever_Warning::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Detection */
	pState = CStateSkeever_Detection::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Return */
	pState = CStateSkeever_Return::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Chase */
	pState = CStateSkeever_Chase::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Attack */
	pState = CStateSkeever_Attack::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Charge */
	pState = CStateSkeever_Charge::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Stagger */
	pState = CStateSkeever_Stagger::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Dead */
	pState = CStateSkeever_Dead::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	m_pCurState = m_vecMonsterState.front();

	return S_OK;

}

CState_Skeever* CStateManager_Skeever::Get_State(CSkeever::SKEEVER_STATE _eState)
{
	return m_vecMonsterState[_eState];
}

HRESULT CStateManager_Skeever::Set_State(CSkeever::SKEEVER_STATE _eState)
{
	// m_pCurState = nullptr;
	m_pCurState = m_vecMonsterState[_eState];

	return S_OK;
}

void CStateManager_Skeever::Update(_float _fTimeDelta)
{
	m_pCurState->Update(_fTimeDelta);
}

void CStateManager_Skeever::Late_Update(_float _fTimeDelta)
{
	m_pCurState->Late_Update(_fTimeDelta);
}

CStateManager_Skeever* CStateManager_Skeever::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateManager_Skeever* pInstance = new CStateManager_Skeever();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateManager_Skeever");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateManager_Skeever::Free()
{
	__super::Free();

	for (auto& iter : m_vecMonsterState)
	{
		Safe_Release(iter);
	}
}
