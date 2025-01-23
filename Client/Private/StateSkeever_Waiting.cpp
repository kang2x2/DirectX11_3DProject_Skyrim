#include "framework.h"
#include "StateSkeever_Waiting.h"

#include "GameInstance.h"

#include "Skeever.h"
#include "Player.h"

CStateSkeever_Waiting::CStateSkeever_Waiting()
{
}

HRESULT CStateSkeever_Waiting::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSkeever_Waiting::Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 동작이 끝났을 때 */
	if (m_pMonster->Get_CurFrameIndex() >= 5 &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "staggerbacksmall"))
	{
		m_pMonster->Set_AnimationStop(true);
		m_pMonster->Set_IsDissloving(true);
	}

	Safe_Release(pGameInstance);
}

void CStateSkeever_Waiting::Late_Update(_float _fTimeDelta)
{

}

CStateSkeever_Waiting* CStateSkeever_Waiting::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSkeever_Waiting* pInstance = new CStateSkeever_Waiting();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSkeever_Waiting");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSkeever_Waiting::Free()
{
}
