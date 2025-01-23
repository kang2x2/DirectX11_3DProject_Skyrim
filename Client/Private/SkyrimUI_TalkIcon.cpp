#include "framework.h"
#include "SkyrimUI_TalkIcon.h"

#include "GameInstance.h"
#include "Player.h"

CSkyrimUI_TalkIcon::CSkyrimUI_TalkIcon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrimUI(_pDevice, _pContext)
{
}

CSkyrimUI_TalkIcon::CSkyrimUI_TalkIcon(const CSkyrimUI_TalkIcon& rhs)
	: CSkyrimUI(rhs)
{
}

HRESULT CSkyrimUI_TalkIcon::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSkyrimUI_TalkIcon::Initialize_Clone(void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_tIconDesc = *(TALKICON_DESC*)pArg;

	/* UI */
	m_pTransformCom->Set_Scaling(_float3(25.f, 25.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(0.f, 0.f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_strName = TEXT("UI_TalkIcon");

	return S_OK;
}

void CSkyrimUI_TalkIcon::PriorityTick(_float _fTimeDelta)
{
}

void CSkyrimUI_TalkIcon::Tick(_float _fTimeDelta)
{
	_matrix matWorld = m_pTransformCom->Get_WorldMatrix();
}

void CSkyrimUI_TalkIcon::LateTick(_float _fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI_0, this);
}

HRESULT CSkyrimUI_TalkIcon::Render()
{
	if (m_bIsVisible)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (g_curLevel == LEVEL_GAMEPLAY)
		{
			if (FAILED(Bind_ShaderResources()))
				return E_FAIL;

			pGameInstance->Render_Font(TEXT("Font_Bold"), TEXT("대화하기"),
				_float2((g_iWinSizeX / 2) + 12, (g_iWinSizeY / 2) - 11), XMVectorSet(1.f, 1.f, 1.f, 1.f)
				, 0.f, _float2(0.f, 0.f), 0.5f);
			pGameInstance->Render_Font(TEXT("Font_Bold"), m_tIconDesc.strTargetName,
				_float2((g_iWinSizeX / 2), (g_iWinSizeY / 2) + 12), XMVectorSet(1.f, 1.f, 1.f, 1.f)
				, 0.f, _float2(0.f, 0.f), 0.5f);
		}
		Safe_Release(pGameInstance);
	}

	return E_FAIL;
}

HRESULT CSkyrimUI_TalkIcon::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_Texture_E"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyrimUI_TalkIcon::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 전달. */
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

CSkyrimUI_TalkIcon* CSkyrimUI_TalkIcon::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CSkyrimUI_TalkIcon* pInstance = new CSkyrimUI_TalkIcon(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CSkyrimUI_TalkIcon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSkyrimUI* CSkyrimUI_TalkIcon::Clone(void* _pArg)
{
	CSkyrimUI_TalkIcon* pInstance = new CSkyrimUI_TalkIcon(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CSkyrimUI_TalkIcon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyrimUI_TalkIcon::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
