#include "framework.h"
#include "Effect_BloodFlare.h"

#include "GameInstance.h"

CEffect_BloodFlare::CEffect_BloodFlare(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CEffect_BloodFlare::CEffect_BloodFlare(const CEffect_BloodFlare& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_BloodFlare::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CEffect_BloodFlare::Initialize_Clone(void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;

	m_pTransformCom->Set_Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_bHasMesh = false;
	m_strName = TEXT("Effect_BloodFlare");
	m_strLayerTag = TEXT("Layer_Effect");
	return S_OK;
}

void CEffect_BloodFlare::PriorityTick(_float _fTimeDelta)
{
}

void CEffect_BloodFlare::Tick(_float _fTimeDelta)
{
	if (m_fAlpha >= 0.f)
		m_fAlpha -= 1.f * _fTimeDelta;
}

void CEffect_BloodFlare::LateTick(_float _fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CEffect_BloodFlare::Render()
{
	if (m_fAlpha != 0.f)
	{
		if (FAILED(Bind_ShaderResource()))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CEffect_BloodFlare::Ready_Component()
{
	// Renderer
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	// Shader
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Shader_Effect"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	// VIBuffer
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	// Texture
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_Texture_BloodFlare"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	// Transform
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_BloodFlare::Bind_ShaderResource()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_Textures")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CEffect_BloodFlare* CEffect_BloodFlare::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CEffect_BloodFlare* pInstance = new CEffect_BloodFlare(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CEffect_BloodFlare");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_BloodFlare::Clone(void* _pArg)
{
	CEffect_BloodFlare* pInstance = new CEffect_BloodFlare(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CEffect_BloodFlare");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_BloodFlare::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
