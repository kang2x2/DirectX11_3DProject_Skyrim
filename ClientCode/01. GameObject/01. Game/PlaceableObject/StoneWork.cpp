#include "framework.h"
#include "StoneWork.h"

#include "GameInstance.h"

CStoneWork::CStoneWork(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlaceableObject(_pDevice, _pContext)
{
}

CStoneWork::CStoneWork(const CStoneWork& rhs)
	: CPlaceableObject(rhs)
{
}

HRESULT CStoneWork::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CStoneWork::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CStoneWork::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	_matrix* pMatPivot = (_matrix*)pArg;

	m_strModelComTag = _strModelComTag;

	if (FAILED(Ready_Component(_iLevel)))
		return E_FAIL;

	m_pTransformCom->Set_WorldMatrix(*pMatPivot);

	m_bHasMesh = true;
	m_strName = TEXT("SkyrimStoneWork");

	return S_OK;

}

void CStoneWork::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CStoneWork::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CStoneWork::Render()
{
	__super::Render();

	return S_OK;

}

HRESULT CStoneWork::Ready_Component(_uint _iLevel)
{
	__super::Ready_Component(_iLevel);
	return S_OK;

}

CStoneWork* CStoneWork::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CStoneWork* pInstance = new CStoneWork(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CStoneWork");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStoneWork::Clone(void* _pArg)
{
	CStoneWork* pInstance = new CStoneWork(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CStoneWork");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStoneWork::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CStoneWork* pInstance = new CStoneWork(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CStoneWork");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStoneWork::Free()
{
	__super::Free();
}
