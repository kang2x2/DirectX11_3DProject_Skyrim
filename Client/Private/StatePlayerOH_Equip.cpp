#include "framework.h"
#include "StatePlayerOH_Equip.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayerOH_Equip::CStatePlayerOH_Equip()
{
}

HRESULT CStatePlayerOH_Equip::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_Equip::Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_CurFrameIndex() >= 10 &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_equip") &&
		strcmp(m_pPlayer->Get_CurSocketBonName(), "WEAPON"))
	{
		m_pPlayer->Set_SoketBone("WEAPON");
	}

	__super::Key_Input(_fTimeDelta);
}

void CStatePlayerOH_Equip::Late_Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_IsAnimationFin() && 
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_equip"))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation_All(true, "1hm_idle");
	}
}

CStatePlayerOH_Equip* CStatePlayerOH_Equip::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_Equip* pInstance = new CStatePlayerOH_Equip();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_Equip");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayerOH_Equip::Free()
{
}
