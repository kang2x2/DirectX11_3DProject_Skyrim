#include "framework.h"
#include "StateGuardCaptain_Warning.h"

#include "GameInstance.h"

#include "NPC_GuardCaptain.h"
#include "Player.h"

CStateGuardCaptain_Warning::CStateGuardCaptain_Warning()
{
}

HRESULT CStateGuardCaptain_Warning::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateGuardCaptain_Warning::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	if (m_pNpc->Get_CurTalkID() == 0 && m_pNpc->Get_CurTextID() == 2)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		pGameInstance->PlaySoundFile(TEXT("wpn_blade1hand_sheathe.wav"), CHANNEL_PLAYER, 1.f);

		Safe_Release(pGameInstance);

		m_pNpc->Set_State(CNPC_GuardCaptain::GUARDCAPTAIN_UNEQUIP);
		m_pNpc->Play_Animation(false, "1hm_unequip");
	}

	_float4 vPos;
	XMStoreFloat4(&vPos, m_pNpcTransform->Get_State(CTransform::STATE_POSITION));
	vPos.y -= 0.5f;
	g_bIsTalk = true;
	m_pPlayerTransform->LookAt_Fade(XMLoadFloat4(&vPos), _fTimeDelta, XMConvertToRadians(45.f));

	m_pNpcTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
}

void CStateGuardCaptain_Warning::Late_Update(_float _fTimeDelta)
{
}

CStateGuardCaptain_Warning* CStateGuardCaptain_Warning::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateGuardCaptain_Warning* pInstance = new CStateGuardCaptain_Warning();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateGuardCaptain_Warning");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateGuardCaptain_Warning::Free()
{
}
