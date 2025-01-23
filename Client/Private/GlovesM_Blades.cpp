#include "framework.h"
#include "GlovesM_Blades.h"

#include "GameInstance.h"

CGlovesM_Blades::CGlovesM_Blades(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrimArmor(_pDevice, _pContext)
{
}

CGlovesM_Blades::CGlovesM_Blades(const CGlovesM_Blades& rhs)
	: CSkyrimArmor(rhs)
{
}

HRESULT CGlovesM_Blades::Initialize_ProtoType()
{
	__super::Initialize_ProtoType();

	return S_OK;
}

HRESULT CGlovesM_Blades::Initialize_Clone(void* _pArg)
{
	__super::Initialize_Clone(_pArg);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(_float3(0.01f, 0.01f, 0.01f));
	// m_pTransformCom->Fix_Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-90.0f));
	// matInitialize = XMMatrixRotationY(XMConvertToRadians(-90.f));
	m_strName = TEXT("블레이즈 건틀릿(남)");
	m_ePartType = ARMOR_GLOVE;

	return S_OK;
}

void CGlovesM_Blades::Tick(_float _fTimeDelta)
{
}

void CGlovesM_Blades::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CGlovesM_Blades::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CGlovesM_Blades::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_GlovesM_Blades"),
		TEXT("Com_3stModel"), (CComponent**)&m_pModelComAry[VIEW_3ST])))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_GlovesM_Blades"),
		TEXT("Com_1stModel"), (CComponent**)&m_pModelComAry[VIEW_1ST])))
		return E_FAIL;

	__super::Ready_Component();

	return S_OK;
}

CGlovesM_Blades* CGlovesM_Blades::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CGlovesM_Blades* pInstance = new CGlovesM_Blades(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CGlovesM_Blades");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGlovesM_Blades::Clone(void* _pArg)
{
	CGlovesM_Blades* pInstance = new CGlovesM_Blades(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CGlovesM_Blades");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlovesM_Blades::Free()
{
	__super::Free();
}

