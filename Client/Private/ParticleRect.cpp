#include "framework.h"
#include "ParticleRect.h"

#include "GameInstance.h"

CParticleRect::CParticleRect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CParticleRect::CParticleRect(const CParticleRect& rhs)
	: CGameObject(rhs)
{
}

HRESULT CParticleRect::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CParticleRect::Initialize_Clone(void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(_float3(10.f, 10.f, 10.f));

	return S_OK;
}

HRESULT CParticleRect::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CParticleRect::PriorityTick(_float _fTimeDelta)
{

}

void CParticleRect::Tick(_float _fTimeDelta)
{
	m_pVIBufferCom->Update(_fTimeDelta);
}

void CParticleRect::LateTick(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTransform* m_pTargetTransform = 
		dynamic_cast<CTransform*>
		(pGameInstance->Find_CloneObject(g_curLevel, TEXT("Layer_Player"), TEXT("Player"))
		->Get_Component(TEXT("Com_Transform")));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTargetTransform->Get_State(CTransform::STATE_POSITION));

	Safe_Release(pGameInstance);

	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CParticleRect::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	// m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CParticleRect::Ready_Component()
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
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_Texture_Snow"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CParticleRect::Bind_ShaderResource()
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

CParticleRect* CParticleRect::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CParticleRect* pInstance = new CParticleRect(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CParticleRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParticleRect::Clone(void* _pArg)
{
	CParticleRect* pInstance = new CParticleRect(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CParticleRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleRect::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
