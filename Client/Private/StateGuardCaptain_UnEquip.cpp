#include "framework.h"
#include "StateGuardCaptain_UnEquip.h"

#include "GameInstance.h"

#include "NPC_GuardCaptain.h"
#include "Player.h"

CStateGuardCaptain_UnEquip::CStateGuardCaptain_UnEquip()
{
}

HRESULT CStateGuardCaptain_UnEquip::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateGuardCaptain_UnEquip::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_pNpcTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	if (m_pNpc->Get_CurFrameIndex() >= 30 &&
		!strcmp(m_pNpc->Get_CurAnimationName().c_str(), "1hm_unequip"))
	{
		m_pNpc->Set_WeaponSocket("WeaponSword");
	}
}

void CStateGuardCaptain_UnEquip::Late_Update(_float _fTimeDelta)
{
	if (m_pNpc->Get_IsAnimationFin())
	{
		m_pNpc->Set_State(CNPC_GuardCaptain::GUARDCAPTAIN_TALK);
		m_pNpc->Play_Animation(true, "dialogueresponsepositivea");
	}
}

CStateGuardCaptain_UnEquip* CStateGuardCaptain_UnEquip::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateGuardCaptain_UnEquip* pInstance = new CStateGuardCaptain_UnEquip();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateGuardCaptain_UnEquip");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateGuardCaptain_UnEquip::Free()
{
}
