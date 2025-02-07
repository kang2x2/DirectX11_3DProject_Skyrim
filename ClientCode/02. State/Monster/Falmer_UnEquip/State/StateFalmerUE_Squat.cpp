#include "framework.h"
#include "StateFalmerUE_Squat.h"

#include "GameInstance.h"

#include "Falmer_UnEquip.h"
#include "Player.h"

CStateFalmerUE_Squat::CStateFalmerUE_Squat()
{
}

HRESULT CStateFalmerUE_Squat::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateFalmerUE_Squat::Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Collision_Enter(m_pVecCollider[CFalmer_UnEquip::FALMERUE_COL_DETECTION], m_pPlayerBodyCollider))
	{
		pGameInstance->Collision_Enter(m_pVecCollider[CFalmer_UnEquip::FALMERUE_COL_MISSDETECTION], m_pPlayerBodyCollider);
		
		m_pMonster->Play_Animation(false, "idlesquatoutro");
		m_pMonster->Set_State(CFalmer_UnEquip::FALMERUE_SQUAT_OUTRO);
	}

	Safe_Release(pGameInstance);
}

void CStateFalmerUE_Squat::Late_Update(_float _fTimeDelta)
{

}

CStateFalmerUE_Squat* CStateFalmerUE_Squat::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateFalmerUE_Squat* pInstance = new CStateFalmerUE_Squat();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateFalmerUE_Squat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateFalmerUE_Squat::Free()
{
}

