#include "framework.h"
#include "SpiderEgg.h"

#include "GameInstance.h"

CSpiderEgg::CSpiderEgg(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlaceableObject(_pDevice, _pContext)
{
}

CSpiderEgg::CSpiderEgg(const CSpiderEgg& rhs)
	: CPlaceableObject(rhs)
{
}

HRESULT CSpiderEgg::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSpiderEgg::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CSpiderEgg::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	_matrix* pMatPivot = (_matrix*)pArg;

	m_strModelComTag = _strModelComTag;

	if (FAILED(Ready_Component(_iLevel)))
		return E_FAIL;

	m_pTransformCom->Set_WorldMatrix(*pMatPivot);

	m_bHasMesh = true;
	m_strName = TEXT("SpiderEgg");

	return S_OK;

}

void CSpiderEgg::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CSpiderEgg::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CSpiderEgg::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CSpiderEgg::Ready_Component(_uint _iLevel)
{
	__super::Ready_Component(_iLevel);
	/* 추후 aabb 추가. */

	return S_OK;

}

CSpiderEgg* CSpiderEgg::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CSpiderEgg* pInstance = new CSpiderEgg(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CSpiderEgg");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderEgg::Clone(void* _pArg)
{
	CSpiderEgg* pInstance = new CSpiderEgg(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CSpiderEgg");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderEgg::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CSpiderEgg* pInstance = new CSpiderEgg(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CSpiderEgg");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpiderEgg::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
