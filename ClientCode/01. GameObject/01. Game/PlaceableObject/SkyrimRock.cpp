#include "framework.h"
#include "SkyrimRock.h"

#include "GameInstance.h"

CSkyrimRock::CSkyrimRock(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlaceableObject(_pDevice, _pContext)
{
}

CSkyrimRock::CSkyrimRock(const CSkyrimRock& rhs)
	: CPlaceableObject(rhs)
{
}

HRESULT CSkyrimRock::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSkyrimRock::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CSkyrimRock::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	_matrix* pMatPivot = (_matrix*)pArg;

	m_strModelComTag = _strModelComTag;

	if (FAILED(Ready_Component(_iLevel)))
		return E_FAIL;

	m_pTransformCom->Set_WorldMatrix(*pMatPivot);

	m_bHasMesh = true;
	m_strName = TEXT("SkyrimRock");

	return S_OK;

}

void CSkyrimRock::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CSkyrimRock::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CSkyrimRock::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CSkyrimRock::Ready_Component(_uint _iLevel)
{
	__super::Ready_Component(_iLevel);

	return S_OK;

}

CSkyrimRock* CSkyrimRock::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CSkyrimRock* pInstance = new CSkyrimRock(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CSkyrimRock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkyrimRock::Clone(void* _pArg)
{
	CSkyrimRock* pInstance = new CSkyrimRock(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CSkyrimRock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkyrimRock::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CSkyrimRock* pInstance = new CSkyrimRock(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CSkyrimRock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyrimRock::Free()
{
	__super::Free();
}
