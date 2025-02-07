#include "framework.h"
#include "LoadingObj_Skeever.h"

#include "GameInstance.h"

CLoadingObj_Skeever::CLoadingObj_Skeever(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CLoadingObj_Skeever::CLoadingObj_Skeever(const CLoadingObj_Skeever& rhs)
	: CGameObject(rhs)
{
}

HRESULT CLoadingObj_Skeever::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CLoadingObj_Skeever::Initialize_Clone(void* pArg)
{
	if (FAILED(Ready_Component(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-0.5f, -0.5f, 0.f, 1.f));
	m_strName = TEXT("LoadingObj_Skeever");

	m_strText = TEXT("이 영약한 쥐는 수렁이나 하수도 뿐 만이 아니라 \n어디서나 서식하며 질병을 옮기고 다닙니다.");

	return S_OK;
}

void CLoadingObj_Skeever::PriorityTick(_float _fTimeDelta)
{
}

void CLoadingObj_Skeever::Tick(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_long mouseMove = 0l;

	if (pGameInstance->Get_DIKeyPress(VK_LBUTTON))
	{
		if (mouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::MMS_X)) // (y축을 회전하면 x축이 움직이기에.)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -(mouseMove * 0.01f * _fTimeDelta));
	}

	Safe_Release(pGameInstance);
}

void CLoadingObj_Skeever::LateTick(_float _fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CLoadingObj_Skeever::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	// 메시 몇개
	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_MaterialTexture(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_MaterialTexture(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Render_Font(TEXT("Font_Bold"), m_strText,
		_float2(800.f, 600.f), XMVectorSet(1.f, 1.f, 1.f, 1.f)
		, 0.f, _float2(20.f, 20.f), 0.5f);

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLoadingObj_Skeever::Ready_Component(void* pArg)
{
	if (FAILED(__super::Add_CloneComponent(LEVEL_LOADING, TEXT("ProtoType_Component_Model_LoadingObj_Skeever"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_LOADING, TEXT("ProtoType_Component_Shader_VtxNonAnimMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingObj_Skeever::Bind_ShaderResource()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	// 뷰, 투영 행렬을 던져준다.
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::D3DTS_VIEW)))
		return E_FAIL;
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLoadingObj_Skeever* CLoadingObj_Skeever::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLoadingObj_Skeever* pInstance = new CLoadingObj_Skeever(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CLoadingObj_Skeever");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoadingObj_Skeever::Clone(void* _pArg)
{
	CLoadingObj_Skeever* pInstance = new CLoadingObj_Skeever(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CLoadingObj_Skeever");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoadingObj_Skeever::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
