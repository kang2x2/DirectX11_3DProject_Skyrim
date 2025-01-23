#include "framework.h"
#include "SceneChangeCol.h"

#include "GameInstance.h"
#include "Player.h"

#include "Effect_FadeBlack.h"

CSceneChangeCol::CSceneChangeCol(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CSceneChangeCol::CSceneChangeCol(const CSceneChangeCol& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSceneChangeCol::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSceneChangeCol::Initialize_Clone(void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	SCENECHANGECOL_DESC tempDesc = *(SCENECHANGECOL_DESC*)pArg;
	m_eDestStage = tempDesc.eDestStage;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, tempDesc.vPos);
	m_strDestText = tempDesc.strDestText;

	/* UI */
	m_pTransformE->Set_Scaling(_float3(25.f, 25.f, 1.f));
	m_pTransformE->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(0.f, 0.f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));



	m_strName = TEXT("SceneChangeCol_Dungeon");

	return S_OK;
}

void CSceneChangeCol::PriorityTick(_float _fTimeDelta)
{
}

void CSceneChangeCol::Tick(_float _fTimeDelta)
{
	_matrix matWorld = m_pTransformCom->Get_WorldMatrix();
	
	m_pColliderCom->Update(matWorld);
	
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>
		(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Player")));
	
	if (m_pColliderCom->IsCollision(dynamic_cast<CCollider*>(pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB")))))
	{
		if (pGameInstance->Get_DIKeyDown('E'))
		{
			CEffect_FadeBlack::FADE_DESC FadeDesc;
			// Gara
			FadeDesc.bIsFadeIn = false;
			FadeDesc.fFadeTime = 0.5f;
			FadeDesc.eDestStage = m_eDestStage;

			pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("ProtoType_GameObject_FadeBlack"), &FadeDesc);

			m_bIsReadyChange = true;
		}
	}
	
	Safe_Release(pGameInstance);
}

void CSceneChangeCol::LateTick(_float _fTimeDelta)
{
#ifdef _DEBUG
	m_pRendererCom->Add_Debug(m_pColliderCom);
#endif
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI_0, this);
}

HRESULT CSceneChangeCol::Render()
{
	if (!m_bIsReadyChange)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);
	
		CPlayer* pPlayer = dynamic_cast<CPlayer*>
			(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Player")));
	
		if (g_curLevel == LEVEL_GAMEPLAY)
		{
			if (m_pColliderCom->IsCollision(dynamic_cast<CCollider*>(pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB")))))
			{
				if (FAILED(Bind_ShaderResources()))
					return E_FAIL;
	
				pGameInstance->Render_Font(TEXT("Font_Bold"), TEXT("이동하기"),
					_float2((g_iWinSizeX / 2) + 12, (g_iWinSizeY / 2) - 11), XMVectorSet(1.f, 1.f, 1.f, 1.f)
					, 0.f, _float2(0.f, 0.f), 0.5f);
				pGameInstance->Render_Font(TEXT("Font_Bold"), m_strDestText,
					_float2((g_iWinSizeX / 2), (g_iWinSizeY / 2) + 12), XMVectorSet(1.f, 1.f, 1.f, 1.f)
					, 0.f, _float2(0.f, 0.f), 0.5f);
			}
		}
		Safe_Release(pGameInstance);
	}

	return E_FAIL;
}

HRESULT CSceneChangeCol::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom), &TransformDesc))
		return E_FAIL;


	/* UI */
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_TransformE"), (CComponent**)&m_pTransformE), &TransformDesc))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_ShaderE"), (CComponent**)&m_pShaderE)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBufferE"), (CComponent**)&m_pVIBufferE)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_Texture_E"),
		TEXT("Com_TextureE"), (CComponent**)&m_pTextureE)))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC AABBDesc = {};

	AABBDesc.vExtents = _float3(1.f, 1.f, 1.f);
	AABBDesc.vCenter = _float3(0.f, AABBDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_AABB"),
		TEXT("Com_Collider_AABB"), (CComponent**)&m_pColliderCom, &AABBDesc)))
		return E_FAIL;

	m_pColliderCom->Set_OwnerObj(this);

	return S_OK;
}

HRESULT CSceneChangeCol::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 전달. */
	if (FAILED(m_pTransformE->Bind_ShaderResources(m_pShaderE, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderE->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderE->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureE->Bind_ShaderResource(m_pShaderE, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderE->Begin(0);
	m_pVIBufferE->Render();

	return S_OK;
}

CSceneChangeCol* CSceneChangeCol::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CSceneChangeCol* pInstance = new CSceneChangeCol(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CSceneChangeCol");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSceneChangeCol::Clone(void* _pArg)
{
	CSceneChangeCol* pInstance = new CSceneChangeCol(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CSceneChangeCol");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSceneChangeCol::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);

	Safe_Release(m_pShaderE);
	Safe_Release(m_pTransformE);
	Safe_Release(m_pTextureE);
	Safe_Release(m_pVIBufferE);
}
