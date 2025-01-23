#include "framework.h"
#include "StatePlayer_RunFowardL.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayer_RunFowardL::CStatePlayer_RunFowardL()
{
}

HRESULT CStatePlayer_RunFowardL::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform)
{
	__super::Initialize(_pPlayer, _pPlayerTransform);

	return S_OK;
}

void CStatePlayer_RunFowardL::Update(_float _fTimeDelta)
{
	m_pPlayerTransform->Go_Foward(_fTimeDelta);

	Key_Input(_fTimeDelta);
}

void CStatePlayer_RunFowardL::Late_Update()
{
}

void CStatePlayer_RunFowardL::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyUp('W'))
	{
		dynamic_cast<CPlayer*>(m_pPlayer)->Set_State(ONEHAND_RUN_L);
	}

	if (pGameInstance->Get_DIKeyUp('A'))
	{
		dynamic_cast<CPlayer*>(m_pPlayer)->Set_State(ONEHAND_RUN_F);
	}

	Safe_Release(pGameInstance);
}

CStatePlayer_RunFowardL* CStatePlayer_RunFowardL::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform)
{
	CStatePlayer_RunFowardL* pInstance = new CStatePlayer_RunFowardL();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform)))
	{
		MSG_BOX("Fail Create : CStatePlayer_RunFowardL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayer_RunFowardL::Free()
{
}
