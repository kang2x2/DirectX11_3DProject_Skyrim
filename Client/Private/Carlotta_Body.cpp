#include "framework.h"
#include "Carlotta_Body.h"

#include "GameInstance.h"
#include "Layer.h"

#include "Player.h"

CCarlotta_Body::CCarlotta_Body(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCarlottaPart_Base(_pDevice, _pContext)
{

}

CCarlotta_Body::CCarlotta_Body(const CCarlotta_Body& rhs)
	: CCarlottaPart_Base(rhs)
{

}

HRESULT CCarlotta_Body::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CCarlotta_Body::Initialize_Clone(void* _pArg)
{
	if (FAILED(__super::Initialize_Clone(_pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_strName = TEXT("Carlotta_Body");

	//CGameInstance* pGameInstance = CGameInstance::GetInstance();
	//Safe_AddRef(pGameInstance);
	//
	//pGameInstance->Add_CloneObject(g_curLevel, TEXT("Temp"), TEXT("ProtoType_GameObject_TorsoF_Merchant01"));
	//
	//m_pArmor = pGameInstance->Find_CloneObject(g_curLevel, TEXT("Temp"), TEXT("TorsoF_Merchant01"));
	//
	//m_pModelCom->SwapDesc_Armor(
	//	dynamic_cast<CModel*>(m_pArmor->Get_Component(TEXT("Com_3stModel"))));
	//
	//Safe_Release(pGameInstance);

	return S_OK;
}

void CCarlotta_Body::Tick(_float _fTimeDelta)
{
	m_pModelCom->Play_Animation(_fTimeDelta);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix());

	/* aabb오리지널 바운딩 * 행렬을 해서 실제 충돌하기위한 데이터(aabb)에게 전달한다.*/
	m_pColliderCom->Update(XMLoadFloat4x4(&m_matWorld));
}

void CCarlotta_Body::LateTick(_float _fTimeDelta)
{
#ifdef _DEBUG
	m_pRendererCom->Add_Debug(m_pColliderCom);
#endif
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	m_pColliderCom->Late_Update(_fTimeDelta);
}

HRESULT CCarlotta_Body::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	// 메시 몇개
	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_BondMatrices(m_pShaderCom, i, "g_BoneMatrices")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_MaterialTexture(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_MaterialTexture(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;


		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

void CCarlotta_Body::Set_AnimationIndex(_bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex, _bool _bIsReset, _bool _bIsQuickChange)
{
	m_pModelCom->SetUp_Animation(_bIsLoop, _strAnimationName, _iChangeIndex, _bIsReset, _bIsQuickChange);
}

_uint CCarlotta_Body::Get_CurFrameIndex()
{
	return m_pModelCom->Get_CurFrameIndex();
}

_bool CCarlotta_Body::Get_CurAnimationIsLoop()
{
	return m_pModelCom->Get_CurAnimationIsLoop();
}

_bool CCarlotta_Body::Get_IsAnimationFin()
{
	return m_pModelCom->Get_IsAnimationFin();
}

string CCarlotta_Body::Get_CurAnimationName()
{
	return m_pModelCom->Get_CurAnimationName().c_str();
}


HRESULT CCarlotta_Body::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_TorsoF_Merchant01"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC AABBDesc = {};

	AABBDesc.vExtents = _float3(0.3f, 0.7f, 0.3f);
	AABBDesc.vCenter = _float3(0.f, AABBDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_AABB"),
		TEXT("Com_Collider_AABB"), (CComponent**)&m_pColliderCom, &AABBDesc)))
		return E_FAIL;

	m_pColliderCom->Set_OwnerObj(m_pParent);

	return S_OK;
}

HRESULT CCarlotta_Body::Bind_ShaderResource()
{
	//if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_matWorld)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// 뷰, 투영 행렬과 카메라의 위치를 던져준다.
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::D3DTS_VIEW)))
		return E_FAIL;
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CCarlotta_Body* CCarlotta_Body::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CCarlotta_Body* pInstance = new CCarlotta_Body(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CCarlotta_Body");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCarlotta_Body::Clone(void* _pArg)
{
	CCarlotta_Body* pInstance = new CCarlotta_Body(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CCarlotta_Body");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCarlotta_Body::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
