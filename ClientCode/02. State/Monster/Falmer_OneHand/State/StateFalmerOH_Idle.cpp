#include "framework.h"
#include "StateFalmerOH_Idle.h"

#include "GameInstance.h"

#include "Falmer_OneHand.h"
#include "Player.h"

CStateFalmerOH_Idle::CStateFalmerOH_Idle()
{
}

HRESULT CStateFalmerOH_Idle::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateFalmerOH_Idle::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Collision_Enter(m_pVecCollider[CFalmer_OneHand::FALMEROH_COL_DETECTION], m_pPlayerBodyCollider))
	{
		pGameInstance->Collision_Enter(m_pVecCollider[CFalmer_OneHand::FALMEROH_COL_MISSDETECTION], m_pPlayerBodyCollider);
		pGameInstance->PlaySoundFile(TEXT("wpn_axe1hand_draw_02.wav"), CHANNEL_MONSTER2, 1.f);

		m_pMonster->Play_Animation(false, "1hmequip");
		m_pMonster->Set_State(CFalmer_OneHand::FALMEROH_EQUIP);
	}

	Safe_Release(pGameInstance);
}

void CStateFalmerOH_Idle::Late_Update(_float _fTimeDelta)
{

}

CStateFalmerOH_Idle* CStateFalmerOH_Idle::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateFalmerOH_Idle* pInstance = new CStateFalmerOH_Idle();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateFalmerOH_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateFalmerOH_Idle::Free()
{
}

