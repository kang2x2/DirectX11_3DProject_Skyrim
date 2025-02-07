#include "framework.h"
#include "Weapon_FalmerAxe.h"

#include "GameInstance.h"
#include "Bone.h"

CWeapon_FalmerAxe::CWeapon_FalmerAxe(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrimWeapon(_pDevice, _pContext)
{
}

CWeapon_FalmerAxe::CWeapon_FalmerAxe(const CWeapon_FalmerAxe& rhs)
	: CSkyrimWeapon(rhs)
{
}

HRESULT CWeapon_FalmerAxe::Initialize_ProtoType()
{
	__super::Initialize_ProtoType();

	return S_OK;
}

HRESULT CWeapon_FalmerAxe::Initialize_Clone(void* pArg)
{
	__super::Initialize_Clone(pArg);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Fix_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.0f));
	// matInitialize = XMMatrixRotationY(XMConvertToRadians(-90.f));
	m_strName = TEXT("FalmerAxe");

	return S_OK;
}

void CWeapon_FalmerAxe::Tick(_float _fTimeDelta)
{
}

void CWeapon_FalmerAxe::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CWeapon_FalmerAxe::Render()
{
	__super::Render();
	return S_OK;
}

HRESULT CWeapon_FalmerAxe::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_Weapon_FalmerAxe"),
		TEXT("Com_3stModel"), (CComponent**)&m_pModelComAry[CSkyrimWeapon::VIEW_3ST])))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_Weapon_FalmerAxe"),
		TEXT("Com_1stModel"), (CComponent**)&m_pModelComAry[CSkyrimWeapon::VIEW_1ST])))
		return E_FAIL;

	__super::Ready_Component();

	return S_OK;
}

CWeapon_FalmerAxe* CWeapon_FalmerAxe::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CWeapon_FalmerAxe* pInstance = new CWeapon_FalmerAxe(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CWeapon_FalmerAxe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_FalmerAxe::Clone(void* _pArg)
{
	CWeapon_FalmerAxe* pInstance = new CWeapon_FalmerAxe(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CWeapon_FalmerAxe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_FalmerAxe::Free()
{
	__super::Free();
}
