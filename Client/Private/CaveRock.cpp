#include "framework.h"
#include "CaveRock.h"

#include "GameInstance.h"

CCaveRock::CCaveRock(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlaceableObject(_pDevice, _pContext)
{
}

CCaveRock::CCaveRock(const CCaveRock& rhs)
	: CPlaceableObject(rhs)
{
}

HRESULT CCaveRock::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CCaveRock::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CCaveRock::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	_matrix* pMatPivot = (_matrix*)pArg;

	m_strModelComTag = _strModelComTag;

	if (FAILED(Ready_Component(_iLevel)))
		return E_FAIL;

	m_pTransformCom->Set_WorldMatrix(*pMatPivot);

	m_bHasMesh = true;
	m_strName = TEXT("CaveRock");

	return S_OK;

}

void CCaveRock::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CCaveRock::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CCaveRock::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CCaveRock::Ready_Component(_uint _iLevel)
{
	__super::Ready_Component(_iLevel);

	/* 추후 aabb 추가. */

	return S_OK;

}

CCaveRock* CCaveRock::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CCaveRock* pInstance = new CCaveRock(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CCaveRock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCaveRock::Clone(void* _pArg)
{
	CCaveRock* pInstance = new CCaveRock(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CCaveRock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCaveRock::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CCaveRock* pInstance = new CCaveRock(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CCaveRock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCaveRock::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
