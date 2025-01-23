#include "framework.h"
#include "Building.h"

#include "GameInstance.h"

CBuilding::CBuilding(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlaceableObject(_pDevice, _pContext)
{
}

CBuilding::CBuilding(const CBuilding& rhs)
	: CPlaceableObject(rhs)
{
}

HRESULT CBuilding::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CBuilding::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CBuilding::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	_matrix* pMatPivot = (_matrix*)pArg;

	m_strModelComTag = _strModelComTag;

	if (FAILED(Ready_Component(_iLevel)))
		return E_FAIL;

	// 받아온 행렬의 정보를 저장 후 세팅
	//_float4x4 _matInit;
	//XMStoreFloat4x4(&_matInit, (*pMatPivot));
	//_float3 vScale = { _matInit._11, _matInit._22, _matInit._33 };
	//
	// _vector vPos = pMatPivot->r[3];
	//
	//m_pTransformCom->Set_Scaling(vScale);
	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->Set_WorldMatrix(*pMatPivot);
	// 메시의 정점 정보도 같이 업데이트 해준다.
	m_pModelCom->Update_VI(*pMatPivot);

	m_bHasMesh = true;
	m_strName = TEXT("SkyrimBuilding");

	m_fRadius = 10.f;

	return S_OK;
}

void CBuilding::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CBuilding::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CBuilding::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CBuilding::Ready_Component(_uint _iLevel)
{
	__super::Ready_Component(_iLevel);

	return S_OK;
}

CBuilding* CBuilding::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CBuilding* pInstance = new CBuilding(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CBuilding ProtoType");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBuilding::Clone(void* _pArg)
{
	CBuilding* pInstance = new CBuilding(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CBuilding Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBuilding::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CBuilding* pInstance = new CBuilding(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CBuilding Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuilding::Free()
{
	__super::Free();
}
