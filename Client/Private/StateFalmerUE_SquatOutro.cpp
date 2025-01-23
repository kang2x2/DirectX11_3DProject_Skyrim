#include "framework.h"
#include "StateFalmerUE_SquatOutro.h"

#include "GameInstance.h"

#include "Falmer_UnEquip.h"

CStateFalmerUE_SquatOutro::CStateFalmerUE_SquatOutro()
{
}

HRESULT CStateFalmerUE_SquatOutro::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateFalmerUE_SquatOutro::Update(_float _fTimeDelta)
{
}

void CStateFalmerUE_SquatOutro::Late_Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pMonster->Get_IsAnimationFin())
	{
		if (pGameInstance->Collision_Stay(m_pVecCollider[CFalmer_UnEquip::FALMERUE_COL_MISSDETECTION], m_pPlayerBodyCollider))
		{
			pGameInstance->PlaySoundFile(TEXT("npc_falmer_aggrowarning_03.wav"), CHANNEL_MONSTER1, 1.f);

			m_pMonster->Set_State(CFalmer_UnEquip::FALMERUE_WARNING);
			m_pMonster->Play_Animation(false, "1hm_aggrowarning1");
		}
		else
		{
			m_pMonster->Set_State(CFalmer_UnEquip::FALMERUE_SQUAT_INTRO);
			m_pMonster->Play_Animation(false, "idlesquatintro");
		}
	}

	Safe_Release(pGameInstance);
}

CStateFalmerUE_SquatOutro* CStateFalmerUE_SquatOutro::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateFalmerUE_SquatOutro* pInstance = new CStateFalmerUE_SquatOutro();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateFalmerUE_SquatOutro");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateFalmerUE_SquatOutro::Free()
{
}

