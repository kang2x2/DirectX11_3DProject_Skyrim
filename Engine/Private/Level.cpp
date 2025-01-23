#include "Level.h"

CLevel::CLevel(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice(_pDevice)
	, m_pContext(_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CLevel::Initialize()
{
	return S_OK;
}

HRESULT CLevel::Tick(_float _fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel::LateTick(_float _fTimeDelta)
{
	return S_OK;
}

void CLevel::AfterRender()
{
}

void CLevel::Free()
{
 	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
