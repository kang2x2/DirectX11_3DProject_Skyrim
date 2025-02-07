#include "framework.h"
#include "DGPlaceableObj.h"

#include "GameInstance.h"

CDGPlaceableObj::CDGPlaceableObj(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlaceableObject(_pDevice, _pContext)
{
}

CDGPlaceableObj::CDGPlaceableObj(const CDGPlaceableObj& rhs)
	: CPlaceableObject(rhs)
{
}

HRESULT CDGPlaceableObj::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CDGPlaceableObj::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CDGPlaceableObj::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	_matrix* pMatPivot = (_matrix*)pArg;

	m_strModelComTag = _strModelComTag;

	if (FAILED(Ready_Component(_iLevel)))
		return E_FAIL;

	m_pTransformCom->Set_WorldMatrix(*pMatPivot);

	m_bHasMesh = true;
	m_strName = TEXT("DGPlaceableObj");

	return S_OK;

}

void CDGPlaceableObj::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CDGPlaceableObj::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CDGPlaceableObj::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CDGPlaceableObj::Ready_Component(_uint _iLevel)
{
	__super::Ready_Component(_iLevel);

	return S_OK;
}

CDGPlaceableObj* CDGPlaceableObj::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CDGPlaceableObj* pInstance = new CDGPlaceableObj(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CDGPlaceableObj");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDGPlaceableObj::Clone(void* _pArg)
{
	CDGPlaceableObj* pInstance = new CDGPlaceableObj(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CDGPlaceableObj");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDGPlaceableObj::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CDGPlaceableObj* pInstance = new CDGPlaceableObj(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CDGPlaceableObj");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDGPlaceableObj::Free()
{
	__super::Free();
}
