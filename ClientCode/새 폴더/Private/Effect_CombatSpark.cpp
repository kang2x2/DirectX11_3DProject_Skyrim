#include "framework.h"
#include "Effect_CombatSpark.h"

#include "GameInstance.h"

#include "Player.h"

CEffect_CombatSpark::CEffect_CombatSpark(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CEffect_CombatSpark::CEffect_CombatSpark(const CEffect_CombatSpark& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_CombatSpark::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CEffect_CombatSpark::Initialize_Clone(void* pArg)
{
	m_pOwnerTransform = (CTransform*)pArg;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_Light()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_CombatSpark::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	m_pOwnerTransform = (CTransform*)pArg;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_Light()))
		return E_FAIL;
	return S_OK;
}

void CEffect_CombatSpark::PriorityTick(_float _fTimeDelta)
{

}

void CEffect_CombatSpark::Tick(_vector _vWeaponPos, _float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHT_DESC* LightDesc = pGameInstance->Get_LightDesc(m_iSelfIndex);
	XMStoreFloat4(&LightDesc->vLightPos, _vWeaponPos);

	if (LightDesc->fLightRange < 0.f)
		LightDesc->fLightRange = 0.f;

	if (m_bIsSpark)
	{
		m_fRange += 30.f * _fTimeDelta;
		LightDesc->fLightRange = m_fRange;

		if (m_fRange > 1.f)
			m_bIsSpark = false;
	}
	else
	{
		if (m_fRange > 0.f)
		{
			m_fRange -= 30.f * _fTimeDelta;
			if(m_fRange > 0)
				LightDesc->fLightRange = m_fRange;
		}
	}

	Safe_Release(pGameInstance);
}

void CEffect_CombatSpark::LateTick(_float _fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CEffect_CombatSpark::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	// m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_CombatSpark::Ready_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Shader_Rect_Instance"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_VIBuffer_Rect_Instance"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_Texture_SparkEffect"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_CombatSpark::Ready_Light()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHT_DESC LightDesc;

	/* Á¡ ±¤¿ø Ãß°¡ */
	ZeroMemory(&LightDesc, sizeof(LightDesc));
	LightDesc.eLightType = LIGHT_DESC::LIGHT_POINT;
	LightDesc.fLightRange = 0.f;
	LightDesc.vDiffuse = _float4(1.f, 0.4f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);

	//XMStoreFloat4(&LightDesc.vLightPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (FAILED(pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	m_tLightFileDesc.lightDesc = LightDesc;
	m_iSelfIndex = pGameInstance->Get_CurLightIndex() - 1;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CEffect_CombatSpark::Bind_ShaderResource()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::D3DTS_VIEW)))
		return E_FAIL;
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	return S_OK;
}

LIGHT_DESC* CEffect_CombatSpark::Get_LightDesc()
{
	return nullptr;
}

void CEffect_CombatSpark::Set_LightDesc(LIGHT_DESC _lightDesc)
{
}

CEffect_CombatSpark* CEffect_CombatSpark::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CEffect_CombatSpark* pInstance = new CEffect_CombatSpark(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CEffect_CombatSpark");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_CombatSpark::Clone(void* _pArg)
{
	CEffect_CombatSpark* pInstance = new CEffect_CombatSpark(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CEffect_CombatSpark");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_CombatSpark::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
