#include "framework.h"
#include "Effect_IntroFade.h"

#include "GameInstance.h"

CEffect_IntroFade::CEffect_IntroFade(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CEffect_IntroFade::CEffect_IntroFade(const CEffect_IntroFade& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_IntroFade::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CEffect_IntroFade::Initialize_Clone(void* _pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;

	m_pTransformBG->Set_Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformBG->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	m_fSizeX = g_iWinSizeX / 2;
	m_fSizeY = g_iWinSizeY / 2;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;

	m_pTransformTitle->Set_Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformTitle->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_bHasMesh = false;
	m_strName = TEXT("Effect_BloodFlare");

	return S_OK;
}

void CEffect_IntroFade::PriorityTick(_float _fTimeDelta)
{
}

void CEffect_IntroFade::Tick(_float _fTimeDelta)
{
	m_fWaitTime += _fTimeDelta;
	if (m_fWaitTime > 2.f)
	{
		switch (m_iEventLevel)
		{
		case 0:
			if (m_fAlphaTitle < 1.f)			
				m_fAlphaTitle += 0.25f * _fTimeDelta;
			
			if(m_fAlphaTitle >= 1.f)
				m_iEventLevel += 1;

			break;

		case 1:
			m_fWaitTimeTitle += _fTimeDelta;

			if (m_fWaitTimeTitle > 3.f)
				m_iEventLevel += 1;

			break;

		case 2:
			m_fAlphaTitle -= 0.25f * _fTimeDelta;

			if (m_fAlphaTitle <= 0.f)
				m_iEventLevel += 1;
			break;

		case 3:
			m_fAlphaBG -= 0.1f * _fTimeDelta;

			if (m_fAlphaBG <= 0.f)
			{
				CGameInstance* pGameInstance = CGameInstance::GetInstance();
				Safe_AddRef(pGameInstance);
				
				pGameInstance->Add_CloneObject(g_curLevel, TEXT("Layer_UI"), TEXT("ProtoType_GameObject_Letter"));
				
				Safe_Release(pGameInstance);

				m_bReadyDead = true;
			}
			break;
		}
	}
}

void CEffect_IntroFade::LateTick(_float _fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI_2, this);
}

HRESULT CEffect_IntroFade::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_IntroFade::Ready_Component()
{
	// Renderer
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	// Shader
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Shader_Effect"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

#pragma region Background
	// VIBuffer
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBufferBG"), (CComponent**)&m_pVIBufferBG)))
		return E_FAIL;
	// Texture
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_FadeBlack"),
		TEXT("Com_TextureBG"), (CComponent**)&m_pTextureBG)))
		return E_FAIL;
	// Transform
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_TransformBG"), (CComponent**)&m_pTransformBG)))
		return E_FAIL;
#pragma endregion

#pragma region Title
	// VIBuffer
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBufferTitle"), (CComponent**)&m_pVIBufferTitle)))
		return E_FAIL;
	// Texture
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Title"),
		TEXT("Com_TextureTitle"), (CComponent**)&m_pTextureTitle)))
		return E_FAIL;
	// Transform
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_TransformTitle"), (CComponent**)&m_pTransformTitle)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CEffect_IntroFade::Bind_ShaderResource()
{
	/* BackGround */
	if (FAILED(m_pTransformBG->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureBG->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlphaBG, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferBG->Render();

	/* Title */
	if (FAILED(m_pTransformTitle->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureTitle->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlphaTitle, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferTitle->Render();

	return S_OK;
}

CEffect_IntroFade* CEffect_IntroFade::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CEffect_IntroFade* pInstance = new CEffect_IntroFade(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CEffect_IntroFade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_IntroFade::Clone(void* _pArg)
{
	CEffect_IntroFade* pInstance = new CEffect_IntroFade(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CEffect_IntroFade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_IntroFade::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);

	Safe_Release(m_pTransformBG);
	Safe_Release(m_pTextureBG);
	Safe_Release(m_pVIBufferBG);

	Safe_Release(m_pTransformTitle);
	Safe_Release(m_pTextureTitle);
	Safe_Release(m_pVIBufferTitle);
}
