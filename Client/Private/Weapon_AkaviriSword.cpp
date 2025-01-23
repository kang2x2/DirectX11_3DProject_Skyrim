#include "framework.h"
#include "Weapon_AkaviriSword.h"

#include "GameInstance.h"
#include "Bone.h"

CWeapon_AkaviriSword::CWeapon_AkaviriSword(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrimWeapon(_pDevice, _pContext)
{
}

CWeapon_AkaviriSword::CWeapon_AkaviriSword(const CWeapon_AkaviriSword& rhs)
	: CSkyrimWeapon(rhs)
{
}

HRESULT CWeapon_AkaviriSword::Initialize_ProtoType()
{
	__super::Initialize_ProtoType();

	return S_OK;
}

HRESULT CWeapon_AkaviriSword::Initialize_Clone(void* _pArg)
{
	__super::Initialize_Clone(_pArg);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Fix_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.0f));
	// matInitialize = XMMatrixRotationY(XMConvertToRadians(-90.f));
	m_strName = TEXT("아카비르 블레이드");

	return S_OK;
}

void CWeapon_AkaviriSword::Tick(_float _fTimeDelta)
{
}

void CWeapon_AkaviriSword::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CWeapon_AkaviriSword::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CWeapon_AkaviriSword::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_Weapon_AkaviriSword"),
		TEXT("Com_3stModel"), (CComponent**)&m_pModelComAry[VIEW_3ST])))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_Weapon_AkaviriSword1st"),
		TEXT("Com_1stModel"), (CComponent**)&m_pModelComAry[VIEW_1ST])))
		return E_FAIL;

	// 1, 3인칭 모델 만들고 어떻게 바꿀 건지 함수 구상해보기

	__super::Ready_Component();

	return S_OK;
}

CWeapon_AkaviriSword* CWeapon_AkaviriSword::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CWeapon_AkaviriSword* pInstance = new CWeapon_AkaviriSword(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CWeapon_AkaviriSword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_AkaviriSword::Clone(void* _pArg)
{
	CWeapon_AkaviriSword* pInstance = new CWeapon_AkaviriSword(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CWeapon_AkaviriSword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_AkaviriSword::Free()
{
	__super::Free();
}

