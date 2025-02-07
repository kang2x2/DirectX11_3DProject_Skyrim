#include "framework.h"
#include "SkyrimUI_SceneChange.h"

#include "GameInstance.h"
#include "Level_Loading.h"

CSkyrimUI_SceneChange::CSkyrimUI_SceneChange(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrimUI(_pDevice, _pContext)
{
}

CSkyrimUI_SceneChange::CSkyrimUI_SceneChange(const CSkyrimUI_SceneChange& rhs)
	: CSkyrimUI(rhs)
{
}

HRESULT CSkyrimUI_SceneChange::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSkyrimUI_SceneChange::Initialize_Clone(void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	UI_TEXT_DESC* textDesc = (UI_TEXT_DESC*)(pArg);

	m_strText = textDesc->strText;
	m_strFontTag = textDesc->strFontTag;
	m_fSizeX = textDesc->fSizeX;
	m_fSizeY = textDesc->fSizeY;
	m_fX = textDesc->fX;
	m_fY = textDesc->fY;
	m_vColor = textDesc->vColor;
	m_fFontSize = textDesc->fFontSize;
	m_iLevelID = textDesc->iLevelID;
	m_iStageID = textDesc->iStageID;
	
	m_Rc = {(_long)(m_fX - (m_fSizeX / 2)),
			(_long)(m_fY - (m_fSizeY / 2)), 
			(_long)(m_fX + (m_fSizeX / 2)), 
			(_long)(m_fY + (m_fSizeY / 2)), };
	

	m_pTransformCom->Set_Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fX - m_fSizeX * 0.5f, -m_fY + m_fSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(m_fSizeX, m_fSizeY, 0.f, 1.f));

	m_bHasMesh = false;
	m_strName = TEXT("UI_Text");

	return S_OK;
}

void CSkyrimUI_SceneChange::Tick(_float _fTimeDelta)
{
	if (!m_bIsChangeScene)
	{
		POINT MousePos = {};
		GetCursorPos(&MousePos);
		ScreenToClient(g_hWnd, &MousePos);

		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (MousePos.x >= m_Rc.left && MousePos.x <= m_Rc.right &&
			MousePos.y >= m_Rc.top && MousePos.y <= m_Rc.bottom)
		{
			m_vColor = { 1.f, 1.f, 1.f };

			if (pGameInstance->Get_DIKeyDown(MK_LBUTTON))
			{
				pGameInstance->PlaySoundFile(TEXT("ui_menu_ok.wav"), CHANNEL_NATURAL, 1.f);
				m_bIsChangeScene = true;
			}
		}
		else
		{
			m_vColor = { 0.7f, 0.7f, 0.7f };
		}

		Safe_Release(pGameInstance);
	}
}

void CSkyrimUI_SceneChange::LateTick(_float _fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI_0, this);

	if (m_bIsChangeScene)
	{
		m_fChangeWaitTime += _fTimeDelta;
		if (m_fChangeWaitTime > 1.f)
			Change_Event();
	}
}

HRESULT CSkyrimUI_SceneChange::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Render_Font(m_strFontTag, m_strText,
		_float2(m_fX, m_fY), m_vColor
		, 0.f, _float2(m_fSizeX, m_fSizeY), m_fFontSize);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CSkyrimUI_SceneChange::Change_Event()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Set_IsChange(true, m_iLevelID, m_iStageID);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CSkyrimUI_SceneChange::Ready_Component()
{
	// Renderer
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	// Transform
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// VIBuffer
	if (FAILED(__super::Add_CloneComponent(LEVEL_LOADING, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	// Shader
	if (FAILED(__super::Add_CloneComponent(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyrimUI_SceneChange::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 전달. */
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	return S_OK;
}

CSkyrimUI_SceneChange* CSkyrimUI_SceneChange::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CSkyrimUI_SceneChange* pInstance = new CSkyrimUI_SceneChange(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CSkyrimUI_SceneChange");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSkyrimUI* CSkyrimUI_SceneChange::Clone(void* pArg)
{
	CSkyrimUI_SceneChange* pInstance = new CSkyrimUI_SceneChange(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Fail Clone : CSkyrimUI_SceneChange");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyrimUI_SceneChange::Free()
{
	__super::Free();
}
