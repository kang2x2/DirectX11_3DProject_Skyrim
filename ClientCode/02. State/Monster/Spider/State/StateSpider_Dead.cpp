#include "framework.h"
#include "StateSpider_Dead.h"

#include "GameInstance.h"

#include "Spider.h"
#include "Player.h"

CStateSpider_Dead::CStateSpider_Dead()
{
}

HRESULT CStateSpider_Dead::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSpider_Dead::Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 동작이 끝났을 때 */
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "death"))
	{
		m_pMonster->Set_IsDissloving(true);
	}

	Safe_Release(pGameInstance);
}

void CStateSpider_Dead::Late_Update(_float _fTimeDelta)
{
}

CStateSpider_Dead* CStateSpider_Dead::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSpider_Dead* pInstance = new CStateSpider_Dead();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSpider_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSpider_Dead::Free()
{
}
