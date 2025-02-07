#include "framework.h"
#include "StatePlayerOH_UnEquip.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayerOH_UnEquip::CStatePlayerOH_UnEquip()
{
}

HRESULT CStatePlayerOH_UnEquip::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_UnEquip::Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_CurFrameIndex() >= 30 &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_unequip") &&
		strcmp(m_pPlayer->Get_CurSocketBonName(), "WeaponSword"))
	{
		m_pPlayer->Set_SoketBone("WeaponSword");
	}

	__super::Key_Input(_fTimeDelta);
}

void CStatePlayerOH_UnEquip::Late_Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_IsAnimationFin() &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_unequip"))
	{
		m_pPlayer->Play_Animation_All(true, "mt_idle");
		m_pPlayer->Set_State(CPlayer::UNEQUIP_IDLE);
	}
}

CStatePlayerOH_UnEquip* CStatePlayerOH_UnEquip::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_UnEquip* pInstance = new CStatePlayerOH_UnEquip();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_UnEquip");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayerOH_UnEquip::Free()
{
}
