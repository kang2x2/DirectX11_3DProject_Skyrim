#include "framework.h"
#include "StateManager_FalmerUE.h"

#include "State_FalmerUE.h"

#include "StateFalmerUE_Squat.h"
#include "StateFalmerUE_SquatOutro.h"
#include "StateFalmerUE_SquatIntro.h"

#include "StateFalmerUE_Warning.h"
#include "StateFalmerUE_Detection.h"
#include "StateFalmerUE_Return.h"

#include "StateFalmerUE_Chase.h"

#include "StateFalmerUE_Attack.h"
#include "StateFalmerUE_Attack2.h"
#include "StateFalmerUE_Charge.h"

#include "StateFalmerUE_StaggerL.h"
#include "StateFalmerUE_StaggerL2.h"
#include "StateFalmerUE_StaggerH.h"

#include "StateFalmerUE_Dead.h"

CStateManager_FalmerUE::CStateManager_FalmerUE()
{
}

HRESULT CStateManager_FalmerUE::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	if (_pMonster == nullptr)
		return E_FAIL;

	/* Idle */
	CState_FalmerUE* pState = CStateFalmerUE_Squat::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	pState = CStateFalmerUE_SquatOutro::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	pState = CStateFalmerUE_SquatIntro::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Warning */
	pState = CStateFalmerUE_Warning::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Detection */
	pState = CStateFalmerUE_Detection::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Return */
	pState = CStateFalmerUE_Return::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Chase */
	pState = CStateFalmerUE_Chase::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Attack */
	pState = CStateFalmerUE_Attack::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	pState = CStateFalmerUE_Attack2::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Charge */
	pState = CStateFalmerUE_Charge::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Stagger */
	pState = CStateFalmerUE_StaggerL::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	pState = CStateFalmerUE_StaggerL2::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	pState = CStateFalmerUE_StaggerH::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Dead */
	pState = CStateFalmerUE_Dead::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	m_pCurState = m_vecMonsterState.front();

	return S_OK;

}

CState_FalmerUE* CStateManager_FalmerUE::Get_State(CFalmer_UnEquip::CFalmer_UnEquip::FALMERUE_STATE _eState)
{
	return m_vecMonsterState[_eState];
}

HRESULT CStateManager_FalmerUE::Set_State(CFalmer_UnEquip::FALMERUE_STATE _eState)
{
	m_pCurState = nullptr;
	m_pCurState = m_vecMonsterState[_eState];

	return S_OK;
}

void CStateManager_FalmerUE::Update(_float _fTimeDelta)
{
	m_pCurState->Update(_fTimeDelta);
}

void CStateManager_FalmerUE::Late_Update(_float _fTimeDelta)
{
	m_pCurState->Late_Update(_fTimeDelta);
}

CStateManager_FalmerUE* CStateManager_FalmerUE::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateManager_FalmerUE* pInstance = new CStateManager_FalmerUE();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateManager_FalmerUE");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateManager_FalmerUE::Free()
{
	__super::Free();

	for (auto& iter : m_vecMonsterState)
	{
		Safe_Release(iter);
	}
}

