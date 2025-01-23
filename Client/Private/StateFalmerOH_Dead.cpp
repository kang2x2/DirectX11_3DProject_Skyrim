#include "framework.h"
#include "StateFalmerOH_Dead.h"

#include "GameInstance.h"

#include "Falmer_OneHand.h"
#include "Player.h"

CStateFalmerOH_Dead::CStateFalmerOH_Dead()
{
}

HRESULT CStateFalmerOH_Dead::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateFalmerOH_Dead::Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 동작이 끝났을 때 */
	if (m_pMonster->Get_CurFrameIndex() >= 50 &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "bleedoutintro"))
	{
		m_pMonster->Set_AnimationStop(true);
		m_pMonster->Set_IsDissloving(true);
	}

	Safe_Release(pGameInstance);
}

void CStateFalmerOH_Dead::Late_Update(_float _fTimeDelta)
{
}

CStateFalmerOH_Dead* CStateFalmerOH_Dead::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateFalmerOH_Dead* pInstance = new CStateFalmerOH_Dead();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateFalmerOH_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateFalmerOH_Dead::Free()
{
}

