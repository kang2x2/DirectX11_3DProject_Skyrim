#include "framework.h"
#include "StateSkeever_Idle.h"

#include "GameInstance.h"

#include "Skeever.h"
#include "Player.h"


CStateSkeever_Idle::CStateSkeever_Idle()
{
}

HRESULT CStateSkeever_Idle::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSkeever_Idle::Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Collision_Enter(m_pVecCollider[CSkeever::SKEEVER_COL_DETECTION], m_pPlayerBodyCollider))
	{
		pGameInstance->Collision_Enter(m_pVecCollider[CSkeever::SKEEVER_COL_MISSDETECTION], m_pPlayerBodyCollider);
		pGameInstance->PlaySoundFile(TEXT("npc_skeever_breathe_02_lp.wav"),CHANNEL_MONSTER3, 1.f);

		m_pMonster->Play_Animation(false, "aggrowarning1");
		m_pMonster->Set_State(CSkeever::SKEEVER_WARNING);
	}

	Safe_Release(pGameInstance);
}

void CStateSkeever_Idle::Late_Update(_float _fTimeDelta)
{
}

CStateSkeever_Idle* CStateSkeever_Idle::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSkeever_Idle* pInstance = new CStateSkeever_Idle();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSkeever_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSkeever_Idle::Free()
{
}
