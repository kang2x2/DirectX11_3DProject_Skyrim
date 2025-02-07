#include "framework.h"
#include "TorsoF_Merchant01.h"

#include "GameInstance.h"

CTorsoF_Merchant01::CTorsoF_Merchant01(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrimArmor(_pDevice, _pContext)
{
}

CTorsoF_Merchant01::CTorsoF_Merchant01(const CTorsoF_Merchant01& rhs)
	: CSkyrimArmor(rhs)
{
}

HRESULT CTorsoF_Merchant01::Initialize_ProtoType()
{
	__super::Initialize_ProtoType();

	return S_OK;
}

HRESULT CTorsoF_Merchant01::Initialize_Clone(void* _pArg)
{
	__super::Initialize_Clone(_pArg);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(_float3(0.01f, 0.01f, 0.01f));
	// m_pTransformCom->Fix_Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-90.0f));
	// matInitialize = XMMatrixRotationY(XMConvertToRadians(-90.f));
	m_strName = TEXT("TorsoF_Merchant01");

	return S_OK;
}

void CTorsoF_Merchant01::Tick(_float _fTimeDelta)
{

}

void CTorsoF_Merchant01::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CTorsoF_Merchant01::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CTorsoF_Merchant01::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_TorsoF_Merchant01"),
		TEXT("Com_3stModel"), (CComponent**)&m_pModelComAry[VIEW_3ST])))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_TorsoF_Merchant01"),
		TEXT("Com_1stModel"), (CComponent**)&m_pModelComAry[VIEW_1ST])))
		return E_FAIL;

	__super::Ready_Component();

	return S_OK;
}

CTorsoF_Merchant01* CTorsoF_Merchant01::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CTorsoF_Merchant01* pInstance = new CTorsoF_Merchant01(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CTorsoF_Merchant01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTorsoF_Merchant01::Clone(void* _pArg)
{
	CTorsoF_Merchant01* pInstance = new CTorsoF_Merchant01(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CTorsoF_Merchant01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTorsoF_Merchant01::Free()
{
	__super::Free();
}

