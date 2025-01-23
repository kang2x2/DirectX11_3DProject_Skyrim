#include "framework.h"
#include "StateFalmerUE_StaggerL2.h"

#include "GameInstance.h"

#include "Falmer_UnEquip.h"
#include "Player.h"

CStateFalmerUE_StaggerL2::CStateFalmerUE_StaggerL2()
{
}

HRESULT CStateFalmerUE_StaggerL2::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateFalmerUE_StaggerL2::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CStateFalmerUE_StaggerL2::Late_Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "1hmrecoil3"))
	{
		if (pGameInstance->Collision_Stay(m_pVecCollider[CFalmer_UnEquip::FALMERUE_COL_ATKROUND], m_pPlayerBodyCollider))
		{
			m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());
			pGameInstance->PlaySoundFile(TEXT("npc_falmer_attack_01.wav"), CHANNEL_MONSTER1, 1.f);

			m_pMonster->Set_State(CFalmer_UnEquip::FALMERUE_ATK);
			m_pMonster->Play_Animation(false, "1hm_attack1");
		}
		else
		{
			m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

			m_pMonster->Set_State(CFalmer_UnEquip::FALMERUE_CHASE);
			m_pMonster->Play_Animation(true, "mtrunforward");
		}
	}

	Safe_Release(pGameInstance);

}

CStateFalmerUE_StaggerL2* CStateFalmerUE_StaggerL2::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateFalmerUE_StaggerL2* pInstance = new CStateFalmerUE_StaggerL2();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateFalmerUE_StaggerL2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateFalmerUE_StaggerL2::Free()
{
}
