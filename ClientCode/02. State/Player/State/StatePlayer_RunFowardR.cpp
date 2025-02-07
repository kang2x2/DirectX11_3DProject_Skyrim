#include "framework.h"
#include "StatePlayer_RunFowardR.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayer_RunFowardR::CStatePlayer_RunFowardR()
{
}

HRESULT CStatePlayer_RunFowardR::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform)
{
	__super::Initialize(_pPlayer, _pPlayerTransform);

	return S_OK;
}

void CStatePlayer_RunFowardR::Update(_float _fTimeDelta)
{
	m_pPlayerTransform->SetLook(dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerCamLook());
	m_pPlayerTransform->Go_Foward(_fTimeDelta);

	Key_Input(_fTimeDelta);
}

void CStatePlayer_RunFowardR::Late_Update()
{
}

void CStatePlayer_RunFowardR::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// 여기에 runfoward에 있는거 추가.

	Safe_Release(pGameInstance);
}

CStatePlayer_RunFowardR* CStatePlayer_RunFowardR::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform)
{
	CStatePlayer_RunFowardR* pInstance = new CStatePlayer_RunFowardR();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform)))
	{
		MSG_BOX("Fail Create : CStatePlayer_RunFowardR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayer_RunFowardR::Free()
{
}
