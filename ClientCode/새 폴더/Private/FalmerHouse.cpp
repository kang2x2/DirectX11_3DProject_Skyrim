#include "framework.h"
#include "FalmerHouse.h"

#include "GameInstance.h"

CFalmerHouse::CFalmerHouse(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlaceableObject(_pDevice, _pContext)
{
}

CFalmerHouse::CFalmerHouse(const CFalmerHouse& rhs)
	: CPlaceableObject(rhs)
{
}

HRESULT CFalmerHouse::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CFalmerHouse::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CFalmerHouse::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
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

void CFalmerHouse::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CFalmerHouse::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CFalmerHouse::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CFalmerHouse::Ready_Component(_uint _iLevel)
{
	__super::Ready_Component(_iLevel);

	/* 추후 aabb 추가. */

	return S_OK;
}

CFalmerHouse* CFalmerHouse::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CFalmerHouse* pInstance = new CFalmerHouse(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CFalmerHouse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFalmerHouse::Clone(void* _pArg)
{
	CFalmerHouse* pInstance = new CFalmerHouse(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CFalmerHouse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFalmerHouse::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CFalmerHouse* pInstance = new CFalmerHouse(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CFalmerHouse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFalmerHouse::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
