#include "Light_Fire.h"

#include "GameInstance.h"

CLight_Fire::CLight_Fire(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrim_Light(_pDevice, _pContext)
{
}

CLight_Fire::CLight_Fire(const CLight_Fire& rhs)
	: CSkyrim_Light(rhs)
{
}

HRESULT CLight_Fire::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CLight_Fire::Initialize_Clone(void* _pArg)
{
	__super::Initialize_Clone(_pArg);

	m_strName = TEXT("Light_Fire");

	Ready_Light();

	return S_OK;
}

HRESULT CLight_Fire::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	__super::Initialize_Clone(_iLevel, _strModelComTag, _pArg);

	m_strName = TEXT("Light_Fire");

	Ready_Light();

	return S_OK;
}

void CLight_Fire::PriorityTick(_float _fTimeDelta)
{
}

void CLight_Fire::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CLight_Fire::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CLight_Fire::Render()
{
	return S_OK;
}

HRESULT CLight_Fire::Ready_Light()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHT_DESC LightDesc;

	/* Á¡ ±¤¿ø Ãß°¡ */
	ZeroMemory(&LightDesc, sizeof(LightDesc));
	LightDesc.eLightType = LIGHT_DESC::LIGHT_POINT;
	LightDesc.vLightPos = _float4(1.f, -1.3f, 12.f, 1.f);
	LightDesc.fLightRange = 50.f;
	LightDesc.vDiffuse = _float4(1.f, 0.4f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	XMStoreFloat4(&LightDesc.vLightPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (FAILED(pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

CLight_Fire* CLight_Fire::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLight_Fire* pInstance = new CLight_Fire(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CLight_Fire");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLight_Fire::Clone(void* _pArg)
{
	CLight_Fire* pInstance = new CLight_Fire(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CLight_Fire");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight_Fire::Free()
{
	__super::Free();
}
