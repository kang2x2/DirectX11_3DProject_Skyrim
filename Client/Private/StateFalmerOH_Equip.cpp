#include "framework.h"
#include "StateFalmerOH_Equip.h"

#include "GameInstance.h"

#include "Falmer_OneHand.h"
#include "Player.h"

CStateFalmerOH_Equip::CStateFalmerOH_Equip()
{
}

HRESULT CStateFalmerOH_Equip::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateFalmerOH_Equip::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	if (m_pMonster->Get_CurFrameIndex() >= 15 &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "1hmequip"))
	{
		m_pMonster->Set_WeaponSocket("WEAPON");
	}
}

void CStateFalmerOH_Equip::Late_Update(_float _fTimeDelta)
{
	if (m_pMonster->Get_IsAnimationFin())
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);
		
		pGameInstance->PlaySoundFile(TEXT("npc_falmer_aggrowarning_03.wav"), CHANNEL_MONSTER2, 1.f);

		Safe_Release(pGameInstance);

		m_pMonster->Set_State(CFalmer_OneHand::FALMEROH_WARNING);
		m_pMonster->Play_Animation(false, "1hm_aggrowarning1");
	}

}

CStateFalmerOH_Equip* CStateFalmerOH_Equip::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateFalmerOH_Equip* pInstance = new CStateFalmerOH_Equip();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateFalmerOH_Equip");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateFalmerOH_Equip::Free()
{
}
