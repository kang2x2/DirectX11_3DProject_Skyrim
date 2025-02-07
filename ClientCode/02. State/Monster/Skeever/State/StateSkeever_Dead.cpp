#include "framework.h"
#include "StateSkeever_Dead.h"

#include "GameInstance.h"

#include "Skeever.h"
#include "Player.h"

CStateSkeever_Dead::CStateSkeever_Dead()
{
}

HRESULT CStateSkeever_Dead::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSkeever_Dead::Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 동작이 끝났을 때 */
	if (m_pMonster->Get_CurFrameIndex() >= 20 &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "aggrowarning1"))
	{
		m_pMonster->Set_AnimationStop(true);
		m_pMonster->Set_IsDissloving(true);
	}

	Safe_Release(pGameInstance);
}

void CStateSkeever_Dead::Late_Update(_float _fTimeDelta)
{

}

CStateSkeever_Dead* CStateSkeever_Dead::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSkeever_Dead* pInstance = new CStateSkeever_Dead();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSkeever_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSkeever_Dead::Free()
{
}
