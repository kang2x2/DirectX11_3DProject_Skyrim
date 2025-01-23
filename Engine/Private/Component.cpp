#include "Component.h"

CComponent::CComponent(ID3D11Device* _pDeivce, ID3D11DeviceContext* _pContext)
	: m_pDevice(_pDeivce)
	, m_pContext(_pContext)
	, m_isCloned(false)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CComponent::CComponent(const CComponent& rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
	, m_isCloned(true)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CComponent::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CComponent::Initialize_Clone(void* pArg)
{
	return S_OK;
}

void CComponent::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
