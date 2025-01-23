#include "framework.h"
#include "Level_Castle.h"

#include "GameInstance.h"

CLevel_Castle::CLevel_Castle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CLevel(_pDevice, _pContext)
{
}

HRESULT CLevel_Castle::Initialize()
{
	return S_OK;
}

HRESULT CLevel_Castle::Tick(_float _fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_Castle::LateTick(_float _fTimeDelta)
{
	return S_OK;
}

void CLevel_Castle::AfterRender()
{
}

CLevel_Castle* CLevel_Castle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLevel_Castle* pInstance = new CLevel_Castle(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CLevel_Castle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Castle::Free()
{
	__super::Free();
}
