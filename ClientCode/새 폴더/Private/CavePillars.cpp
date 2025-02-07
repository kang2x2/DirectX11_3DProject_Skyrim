#include "framework.h"
#include "CavePillars.h"

#include "GameInstance.h"

CCavePillars::CCavePillars(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlaceableObject(_pDevice, _pContext)
{
}

CCavePillars::CCavePillars(const CCavePillars& rhs)
	: CPlaceableObject(rhs)
{
}

HRESULT CCavePillars::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CCavePillars::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CCavePillars::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	_matrix* pMatPivot = (_matrix*)pArg;

	m_strModelComTag = _strModelComTag;

	if (FAILED(Ready_Component(_iLevel)))
		return E_FAIL;

	m_pTransformCom->Set_WorldMatrix(*pMatPivot);

	m_bHasMesh = true;
	m_strName = TEXT("FalmerHouse");

	return S_OK;

}

void CCavePillars::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CCavePillars::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CCavePillars::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CCavePillars::Ready_Component(_uint _iLevel)
{
	__super::Ready_Component(_iLevel);

	/* 추후 aabb 추가. */

	return S_OK;

}

CCavePillars* CCavePillars::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CCavePillars* pInstance = new CCavePillars(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CCavePillars");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCavePillars::Clone(void* _pArg)
{
	CCavePillars* pInstance = new CCavePillars(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CCavePillars");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCavePillars::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CCavePillars* pInstance = new CCavePillars(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CCavePillars");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCavePillars::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
