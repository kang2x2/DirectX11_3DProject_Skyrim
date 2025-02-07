#include "framework.h"
#include "SkyrimUI_BossHpBar.h"

#include "GameInstance.h"

CSkyrimUI_BossHpBar::CSkyrimUI_BossHpBar(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrimUI(_pDevice, _pContext)
{
}

CSkyrimUI_BossHpBar::CSkyrimUI_BossHpBar(const CSkyrimUI_BossHpBar& rhs)
	: CSkyrimUI(rhs)
{
}

HRESULT CSkyrimUI_BossHpBar::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSkyrimUI_BossHpBar::Initialize_Clone(void* _pArg)
{
	m_TextName = TEXT("King Spider");

	__super::Initialize_Clone(_pArg);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	UI_InitPos((_float)(g_iWinSizeX / 2.f), 100.f);

	m_fSizeX = 450.f;
	m_fSizeY = 20.f;

	m_pTransformEmpty->Set_Scaling(_float3(m_fSizeX + 100.f, m_fSizeY + 10.f, 1.f));
	m_pTransformEmpty->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fX, m_fY, 0.f, 1.f));

	m_pTransformFill->Set_Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformFill->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_fX, m_fY, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_strName = TEXT("UI_BossHpbar");

	return S_OK;
}

void CSkyrimUI_BossHpBar::PriorityTick(_float _fTimeDelta)
{
}

void CSkyrimUI_BossHpBar::Tick(_float _fHp, _float _fTimeDelta)
{
	if (m_bIsShow)
		m_fAlpha = 1.f;
	else
		m_fAlpha = 0.f;

	// 몬스터 HP에 대한 비율 계산
	//_float hpRatio = _fHp / 1000.f;
	_float hpRatio = _fHp / 450.f;

	// 목표 크기를 비율에 따라 조정
	_float m_fMonsterHpSize = hpRatio * 450.f;

	_float fTempSizeX = m_fSizeX;

	// 선형 보간을 사용하여 부드럽게 크기를 조절
	m_fSizeX = lerp(fTempSizeX, m_fMonsterHpSize, 0.1f);

	if (m_fSizeX != fTempSizeX)
	{
		if (m_fSizeX < 0.1f)
			m_fSizeX = 0.1f;

		m_pTransformFill->Set_Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformFill->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_fX, m_fY, 0.f, 1.f));
	}
}

void CSkyrimUI_BossHpBar::LateTick(_float _fTimeDelta)
{
	if (m_fAlpha > 0.1f)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI_0, this);
	}
}

HRESULT CSkyrimUI_BossHpBar::Render()
{
	if (m_fAlpha > 0.1f)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;
	}

	if (m_bIsShow)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		pGameInstance->Render_Font(TEXT("Font_Bold"), m_TextName,
			_float2(
				((_float)g_iWinSizeX / 2.f) - m_TextName.size() * 8.f, 50.f), { 1.f, 0.f, 0.f, 1.f }
		, 0.f, _float2(0.f, 0.f), 0.5f);

		Safe_Release(pGameInstance);
	}

	return S_OK;
}

HRESULT CSkyrimUI_BossHpBar::Ready_Component()
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
		TEXT("Com_VIBufferEmpty"), (CComponent**)&m_pVIBufferEmpty)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBufferFill"), (CComponent**)&m_pVIBufferFill)))
		return E_FAIL;

	// Texture
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EmptyBar"),
		TEXT("Com_TextureEmpty"), (CComponent**)&m_pTextureEmpty)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HpBar"),
		TEXT("Com_TextureFill"), (CComponent**)&m_pTextureFill)))
		return E_FAIL;

	// Transform
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_TransformEmpty"), (CComponent**)&m_pTransformEmpty)))
		return E_FAIL;
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_TransformFill"), (CComponent**)&m_pTransformFill)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyrimUI_BossHpBar::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 전달. */
	if (FAILED(m_pTransformEmpty->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureEmpty->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(1);
	m_pVIBufferEmpty->Render();

	if (FAILED(m_pTransformFill->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureFill->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(1);
	m_pVIBufferFill->Render();

	return S_OK;
}

CSkyrimUI_BossHpBar* CSkyrimUI_BossHpBar::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CSkyrimUI_BossHpBar* pInstance = new CSkyrimUI_BossHpBar(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CSkyrimUI_BossHpBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSkyrimUI* CSkyrimUI_BossHpBar::Clone(void* _pArg)
{
	CSkyrimUI_BossHpBar* pInstance = new CSkyrimUI_BossHpBar(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CSkyrimUI_BossHpBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyrimUI_BossHpBar::Free()
{
	__super::Free();

	Safe_Release(m_pTransformEmpty);
	Safe_Release(m_pTextureEmpty);
	Safe_Release(m_pVIBufferEmpty);

	Safe_Release(m_pTransformFill);
	Safe_Release(m_pTextureFill);
	Safe_Release(m_pVIBufferFill);
}
