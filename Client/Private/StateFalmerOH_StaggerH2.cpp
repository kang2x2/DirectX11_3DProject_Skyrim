#include "framework.h"
#include "StateFalmerOH_StaggerH2.h"

#include "GameInstance.h"

#include "Falmer_OneHand.h"
#include "Player.h"

CStateFalmerOH_StaggerH2::CStateFalmerOH_StaggerH2()
{
}

HRESULT CStateFalmerOH_StaggerH2::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateFalmerOH_StaggerH2::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	if (m_pMonster->Get_CurFrameIndex() < 40)
		m_pMonsterTransform->Go_Backward(_fTimeDelta, m_pMonsterNavigation);
}

void CStateFalmerOH_StaggerH2::Late_Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "1hmstaggerbackmid"))
	{
		if (pGameInstance->Collision_Enter(m_pVecCollider[CFalmer_OneHand::FALMEROH_COL_ATKROUND], m_pPlayerBodyCollider) || 
			pGameInstance->Collision_Stay(m_pVecCollider[CFalmer_OneHand::FALMEROH_COL_ATKROUND], m_pPlayerBodyCollider))
		{
			m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());
			pGameInstance->PlaySoundFile(TEXT("npc_falmer_attack_02.wav"), CHANNEL_MONSTER2, 1.f);

			m_pMonster->Set_State(CFalmer_OneHand::FALMEROH_ATK);
			m_pMonster->Play_Animation(false, "1hm_attack2");
		}
		else
		{
			m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

			m_pMonster->Set_State(CFalmer_OneHand::FALMEROH_CHASE);
			m_pMonster->Play_Animation(true, "mtrunforward");
		}
	}

	Safe_Release(pGameInstance);
}

CStateFalmerOH_StaggerH2* CStateFalmerOH_StaggerH2::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateFalmerOH_StaggerH2* pInstance = new CStateFalmerOH_StaggerH2();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateFalmerOH_StaggerH2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateFalmerOH_StaggerH2::Free()
{
	__super::Free();
}
