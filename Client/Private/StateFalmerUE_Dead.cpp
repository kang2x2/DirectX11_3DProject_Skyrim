#include "framework.h"
#include "StateFalmerUE_Dead.h"

#include "GameInstance.h"

#include "Falmer_UnEquip.h"
#include "Player.h"

CStateFalmerUE_Dead::CStateFalmerUE_Dead()
{
}

HRESULT CStateFalmerUE_Dead::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateFalmerUE_Dead::Update(_float _fTimeDelta)
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

void CStateFalmerUE_Dead::Late_Update(_float _fTimeDelta)
{
}

CStateFalmerUE_Dead* CStateFalmerUE_Dead::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateFalmerUE_Dead* pInstance = new CStateFalmerUE_Dead();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateFalmerUE_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateFalmerUE_Dead::Free()
{
}

