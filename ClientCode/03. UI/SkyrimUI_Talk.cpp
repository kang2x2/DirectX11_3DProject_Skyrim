#include "framework.h"
#include "SkyrimUI_Talk.h"

#include "GameInstance.h"

#include "Skyrim_NPC.h"

CSkyrimUI_Talk::CSkyrimUI_Talk(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrimUI(_pDevice, _pContext)
{
}

CSkyrimUI_Talk::CSkyrimUI_Talk(const CSkyrimUI_Talk& rhs)
	: CSkyrimUI(rhs)
{
}

HRESULT CSkyrimUI_Talk::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSkyrimUI_Talk::Initialize_Clone(void* _pArg)
{
	m_tTalkDesc = *(TALK_DESC*)_pArg;

	__super::Initialize_Clone(_pArg);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_vBlackPos = {300.f, 0.f};
	m_vBlackSize = {300.f, 200.f};
	m_pTransformBlack->Set_Scaling(_float3(m_vBlackSize.x, m_vBlackSize.y, 1.f));
	m_pTransformBlack->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_vBlackPos.x, m_vBlackPos.y, 0.f, 1.f));

	m_vLinePos = {150.f, 0.f };
	m_vLineSize = {5.f, 200.f };
	m_pTransformLine->Set_Scaling(_float3(m_vLineSize.x, m_vLineSize.y, 1.f));
	m_pTransformLine->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_vLinePos.x, m_vLinePos.y, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_Rc = { 800, (g_iWinSizeY / 2) + 40, 950, (g_iWinSizeY / 2) - 40 };
	m_vPlayerTextColor = { 0.4f, 0.4f, 0.4f, 1.f };

	m_strName = TEXT("UI_Talk");

	return S_OK;
}

void CSkyrimUI_Talk::PriorityTick(_float _fTimeDelta)
{
}

void CSkyrimUI_Talk::Tick(_float _fTimeDelta)
{
	if (m_bIsTalk)
	{
		POINT MousePos = {};
		GetCursorPos(&MousePos);
		ScreenToClient(g_hWnd, &MousePos);

		if (m_tTalkDesc.m_iShowDialogIndex == m_iCurTextIndex)
		{
			if (m_fAlpha < 0.3f)
				m_fAlpha += 0.3f * _fTimeDelta;
		}

		/* 플레이어 텍스트 띄우기 */	
		if (m_iCurTextIndex == m_tTalkDesc.m_vecPlayerTalkDesc[m_iCurPlayerTextIndex].m_iShowTextID)
		{
			m_strPlayerTalk = m_tTalkDesc.m_vecPlayerTalkDesc[m_iCurPlayerTextIndex].m_strPlayerTalk;
		}
		

		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		// 플레이어의 대사를 눌러야 되는 상황이라면
		if (m_iCurTextIndex == m_tTalkDesc.m_vecPlayerTalkDesc[m_iCurPlayerTextIndex].m_iShowTextID)
		{
			// 플레이어 텍스트 범위 내에서 클릭해야만 .
			if (MousePos.x >= m_Rc.left && MousePos.x <= m_Rc.right &&
				MousePos.y <= m_Rc.top && MousePos.y >= m_Rc.bottom)
			{
				m_vPlayerTextColor = { 1.f, 1.f, 1.f, 1.f };

				if (pGameInstance->Get_DIKeyDown(VK_LBUTTON))
				{
					if (m_iCurPlayerTextIndex < m_tTalkDesc.m_vecPlayerTalkDesc.size() - 1)
					{
						m_iCurPlayerTextIndex += 1;
					}
					m_iCurTextIndex += 1;
					m_vPlayerTextColor = { 0.4f, 0.4f, 0.4f, 1.f };
				}
			}
			else
				m_vPlayerTextColor = { 0.4f, 0.4f, 0.4f, 1.f };
		}
		// 그게 아니라면 
		else
		{
			if (pGameInstance->Get_DIKeyDown(VK_LBUTTON))
				m_iCurTextIndex += 1;
		}

		auto iter = m_tTalkDesc.m_mapNpcTalkData.find(0);

		// 만약 모든 대사가 끝났다면
		if (m_iCurTextIndex >= iter->second.size())
		{
			g_iCurTalkID += 1;
			dynamic_cast<CSkyrim_NPC*>(m_tTalkDesc.pOwner)->Set_Idle();
			g_bIsTalk = false;
			m_bIsTalk = false;
		}

		Safe_Release(pGameInstance);
	}
	else
	{
		if (m_fAlpha > 0.f)
			m_fAlpha -= 0.3f * _fTimeDelta;
	}
}

void CSkyrimUI_Talk::LateTick(_float _fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI_0, this);
}

HRESULT CSkyrimUI_Talk::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyrimUI_Talk::Ready_Component()
{
	// Renderer
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// Shader
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// VIBuffer
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBufferBlack"), (CComponent**)&m_pVIBufferBlack)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBufferLine"), (CComponent**)&m_pVIBufferLine)))
		return E_FAIL;

	// Texture
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Black"),
		TEXT("Com_TextureBlack"), (CComponent**)&m_pTextureBlack)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WhiteEm"),
		TEXT("Com_TextureLine"), (CComponent**)&m_pTextureLine)))
		return E_FAIL;

	// Transform
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_TransformBlack"), (CComponent**)&m_pTransformBlack)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_TransformLine"), (CComponent**)&m_pTransformLine)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyrimUI_Talk::Bind_ShaderResources()
{
	if (m_bIsTalk)
	{
		if (m_iCurTextIndex >= m_tTalkDesc.m_iShowDialogIndex)
		{
			if (FAILED(m_pTransformBlack->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
				return E_FAIL;

			if (FAILED(m_pTextureBlack->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
				return E_FAIL;

			m_pShaderCom->Begin(1);
			m_pVIBufferBlack->Render();

			if (FAILED(m_pTransformLine->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
				return E_FAIL;

			if (FAILED(m_pTextureLine->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
				return E_FAIL;

			m_pShaderCom->Begin(1);
			m_pVIBufferLine->Render();
		}

		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		auto iter = m_tTalkDesc.m_mapNpcTalkData.find(0);
		if(g_iCurTalkID == 2)
			auto iter = m_tTalkDesc.m_mapNpcTalkData.find(1);

		if (&iter)
		{
			// Player 대사
			pGameInstance->Render_Font(TEXT("Font_Bold"), m_strPlayerTalk,
				_float2(800.f, ((_float)g_iWinSizeY / 2.f) - 10.f), m_vPlayerTextColor
			, 0.f, _float2(0.f, 0.f), 0.5f);


			// NPC 이름
			pGameInstance->Render_Font(TEXT("Font_Bold"), m_tTalkDesc.m_strOwnerName,
				_float2(((_float)g_iWinSizeX / 2.f) - 80.f, 500.f), { 1.f, 1.f, 1.f, 1.f }
			, 0.f, _float2(0.f, 0.f), 0.7f);

			// NPC 대사
			pGameInstance->Render_Font(TEXT("Font_Bold"), iter->second[m_iCurTextIndex],
				_float2(
					(((_float)g_iWinSizeX/ 2.f) - 80.f) - ((iter->second[m_iCurTextIndex].size() * 8.f) / 2.f), 600.f),
					{ 1.f, 1.f, 1.f, 1.f }
					, 0.f, _float2(0.f, 0.f), 0.5f);
		}

		Safe_Release(pGameInstance);
	}

	return S_OK;
}

CSkyrimUI_Talk* CSkyrimUI_Talk::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CSkyrimUI_Talk* pInstance = new CSkyrimUI_Talk(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CSkyrimUI_Talk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSkyrimUI* CSkyrimUI_Talk::Clone(void* _pArg)
{
	CSkyrimUI_Talk* pInstance = new CSkyrimUI_Talk(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CSkyrimUI_Talk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyrimUI_Talk::Free()
{
	__super::Free();

	Safe_Release(m_pTransformBlack);
	Safe_Release(m_pTextureBlack);
	Safe_Release(m_pVIBufferBlack);

	Safe_Release(m_pTransformLine);
	Safe_Release(m_pTextureLine);
	Safe_Release(m_pVIBufferLine);
}
