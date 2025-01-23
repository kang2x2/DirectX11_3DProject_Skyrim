#include "framework.h"
#include "Effect_BloodSpot.h"

#include "GameInstance.h"

_uint CEffect_BloodSpot::m_iBloodSpotIdx = 0;

CEffect_BloodSpot::CEffect_BloodSpot(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CEffect_BloodSpot::CEffect_BloodSpot(const CEffect_BloodSpot& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_BloodSpot::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CEffect_BloodSpot::Initialize_Clone(void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	switch (m_iBloodSpotIdx)
	{
	case 0:
		m_fSizeX = 200.f;
		m_fSizeY = 200.f;
		m_fX = g_iWinSizeX * 0.75f;
		m_fY = g_iWinSizeY * 0.7f;

		m_pTransformCom->Set_Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

		m_fSizeX = 100.f;
		m_fSizeY = 100.f;
		m_fX = g_iWinSizeX * 0.3f;
		m_fY = g_iWinSizeY * 0.2f;

		m_pTransformCom2->Set_Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom2->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

		m_iBloodSpotIdx++;
		break;
	case 1:
		m_fSizeX = 200.f;
		m_fSizeY = 200.f;
		m_fX = g_iWinSizeX * 0.6f;
		m_fY = g_iWinSizeY * 0.5f;

		m_pTransformCom->Set_Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

		m_fSizeX = 100.f;
		m_fSizeY = 100.f;
		m_fX = g_iWinSizeX * 0.1f;
		m_fY = g_iWinSizeY * 0.5f;

		m_pTransformCom2->Set_Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom2->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

		m_iBloodSpotIdx++;
		break;
	case 2:
		m_fSizeX = 200.f;
		m_fSizeY = 200.f;
		m_fX = g_iWinSizeX * 0.85f;
		m_fY = g_iWinSizeY * 0.2f;

		m_pTransformCom->Set_Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

		m_fSizeX = 100.f;
		m_fSizeY = 100.f;
		m_fX = g_iWinSizeX * 0.25f;
		m_fY = g_iWinSizeY * 0.8f;

		m_pTransformCom2->Set_Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom2->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

		m_iBloodSpotIdx = 0;
		break;
	}

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_bHasMesh = false;
	m_strName = TEXT("Effect_BloodSpot");

	return S_OK;
}

void CEffect_BloodSpot::PriorityTick(_float _fTimeDelta)
{
}

void CEffect_BloodSpot::Tick(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* Spot1 */
	if (!m_bIsRelease && m_fAlpha == 1.f)
		m_fReleaseTime += _fTimeDelta;

	if (!m_bIsRelease && m_fReleaseTime > 2.f)
	{
		m_bIsRelease = true;
		m_fReleaseTime = 2.f - m_fReleaseTime;
	}

	if (m_bIsRelease && m_fAlpha >= 0.f)
		m_fAlpha -= 0.25f * _fTimeDelta;

	else if (m_bIsRelease && m_fAlpha == 0.f)
	{
		m_bIsRelease = false;
		m_bIsDelete2 = true;
	}

	/* Spot2 */
	if (!m_bIsRelease2 && m_fAlpha2 == 1.f)
		m_fReleaseTime2 += _fTimeDelta;

	if (!m_bIsRelease2 && m_fReleaseTime2 > 2.f)
	{
		m_bIsRelease2 = true;
		m_fReleaseTime2 = 2.f - m_fReleaseTime2;
	}

	if (m_bIsRelease2 && m_fAlpha2 >= 0.f)
		m_fAlpha2 -= 0.25f * _fTimeDelta;

	else if (m_bIsRelease2 && m_fAlpha2 == 0.f)
	{
		m_bIsRelease2 = false;
		m_bIsDelete2 = true;
	}


	/* Áö¿ìÀÚ */
	if (m_bIsDelete && m_bIsDelete2)
		m_bReadyDead = true;

	Safe_Release(pGameInstance);
}

void CEffect_BloodSpot::LateTick(_float _fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CEffect_BloodSpot::Render()
{
	if (m_fAlpha != 0.f)
	{
		if (FAILED(Bind_ShaderResource()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_BloodSpot::Ready_Component()
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
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer2"), (CComponent**)&m_pVIBufferCom2)))
		return E_FAIL;

	// Texture
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_Texture_BloodSpot1"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_Texture_BloodSpot2"),
		TEXT("Com_Texture2"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;

	// Transform
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform2"), (CComponent**)&m_pTransformCom2)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_BloodSpot::Bind_ShaderResource()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(1);
	m_pVIBufferCom->Render();

	if (FAILED(m_pTransformCom2->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom2->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha2, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(1);
	m_pVIBufferCom2->Render();

	return S_OK;
}

CEffect_BloodSpot* CEffect_BloodSpot::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CEffect_BloodSpot* pInstance = new CEffect_BloodSpot(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CEffect_BloodSpot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_BloodSpot::Clone(void* _pArg)
{
	CEffect_BloodSpot* pInstance = new CEffect_BloodSpot(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CEffect_BloodSpot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_BloodSpot::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pVIBufferCom2);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTransformCom2);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom2);

}
