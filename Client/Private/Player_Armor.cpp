#include "framework.h"
#include "Player_Armor.h"

#include "GameInstance.h" 
#include "Bone.h"

#include "Armor_Glass.h"

CPlayer_Armor::CPlayer_Armor(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlayerPart_Base(_pDevice, _pContext)
{
}

CPlayer_Armor::CPlayer_Armor(const CPlayer_Armor& rhs)
	: CPlayerPart_Base(rhs)
{
}

HRESULT CPlayer_Armor::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CPlayer_Armor::Initialize_Clone(void* _pArg)
{
	if (nullptr != _pArg)
	{
		ARMOR_DESC* pArmorDesc = (ARMOR_DESC*)_pArg;

		m_pSocketBone = pArmorDesc->pSocketBone;
		Safe_AddRef(m_pSocketBone);

		m_matSocketPivot = pArmorDesc->matSocketPivot;

		if (FAILED(__super::Initialize_Clone(_pArg)))
			return E_FAIL;
	}

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(_float3(0.013f, 0.013f, 0.013f));
	m_pTransformCom->Fix_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.0f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-0.1f, -1.f, 0.f, 1.f));
	m_strName = TEXT("PlayerArmor");

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// m_pArmor = pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Equip"), TEXT("GlassArmor"));

	Safe_Release(pGameInstance);

	return S_OK;
}

void CPlayer_Armor::Tick(_float _fTimeDelta)
{
	/* 내 행렬 * (소캣 뼈의 컴바인드 행렬 * 소캣의 행렬 * 페어런트의 월드 행렬) */
	_float4x4 matSocketCombined = m_pSocketBone->Get_CombinedTransformationMatrix();

	_matrix		WorldMatrix = XMLoadFloat4x4(&matSocketCombined) *
		XMLoadFloat4x4(&m_matSocketPivot);

	WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
	WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
	WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix() * WorldMatrix);
	
	if(m_pArmor != nullptr)
		dynamic_cast<CTransform*>(m_pArmor->Get_Component(TEXT("Com_Transform")))->Set_WorldMatrix(XMLoadFloat4x4(&m_matWorld));
}

void CPlayer_Armor::LateTick(_float _fTimeDelta)
{
	if (m_pArmor != nullptr)
		m_pArmor->LateTick(_fTimeDelta);
}

HRESULT CPlayer_Armor::Render()
{
	if (m_pArmor != nullptr)
		m_pArmor->Render();

	return S_OK;
}

void CPlayer_Armor::Set_SoketBone(CBone* _pBone)
{
	Safe_Release(m_pSocketBone);
	m_pSocketBone = _pBone;
	Safe_AddRef(_pBone);
}

const char* CPlayer_Armor::Get_SoketBoneName()
{
	return m_pSocketBone->Get_BoneName();
}

HRESULT CPlayer_Armor::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Armor::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_matWorld)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	// 뷰, 투영 행렬과 카메라의 위치를 던져준다.
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::D3DTS_VIEW)))
		return E_FAIL;
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
		return E_FAIL;

	_float4 vCamPosition = pGameInstance->Get_CamPosition_Float4();
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (pLightDesc == nullptr)
		return E_FAIL;

	_uint		iPassIndex = 0;

	if (pLightDesc->eLightType == LIGHT_DESC::LIGHT_DIRECTIONAL)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vLightDir, sizeof(_float4))))
			return E_FAIL;
		iPassIndex = 0;
	}
	else if (pLightDesc->eLightType == LIGHT_DESC::LIGHT_POINT)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightPos", &pLightDesc->vLightPos, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fLightRange", &pLightDesc->fLightRange, sizeof(_float))))
			return E_FAIL;
		iPassIndex = 1;
	}

	// 나머지 조명 연산에 필요한 데이터를 던져 줌
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;

}

CPlayer_Armor* CPlayer_Armor::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CPlayer_Armor* pInstance = new CPlayer_Armor(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CPlayer_Armor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Armor::Clone(void* _pArg)
{
	CPlayer_Armor* pInstance = new CPlayer_Armor(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CPlayer_Armor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Armor::Free()
{
	__super::Free();

	Safe_Release(m_pSocketBone);
}
