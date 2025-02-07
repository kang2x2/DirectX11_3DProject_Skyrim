#include "framework.h"
#include "FalmerOH_Weapon.h"

#include "GameInstance.h"

#include "Bone.h"
#include "Player.h"
#include "Falmer_OneHand.h"

CFalmerOH_Weapon::CFalmerOH_Weapon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CFalmerOHPart_Base(_pDevice, _pContext)
{
}

CFalmerOH_Weapon::CFalmerOH_Weapon(const CFalmerOH_Weapon& rhs)
	: CFalmerOHPart_Base(rhs)
{
}

HRESULT CFalmerOH_Weapon::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CFalmerOH_Weapon::Initialize_Clone(void* _pArg)
{
	if (nullptr != _pArg)
	{
		WEAPON_DESC* pWeaponDesc = (WEAPON_DESC*)_pArg;

		m_pSocketBone = pWeaponDesc->pSocketBone;
		Safe_AddRef(m_pSocketBone);

		m_matSocketPivot = pWeaponDesc->matSocketPivot;

		if (FAILED(__super::Initialize_Clone(_pArg)))
			return E_FAIL;
	}

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(_float3(0.0128f, 0.0128f, 0.0128f));
	m_pTransformCom->Fix_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-105.0f));
	_float4 vInitPos;
	XMStoreFloat4(&vInitPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	vInitPos.x -= 0.05f;
	vInitPos.y -= 0.03f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vInitPos));

	m_strName = TEXT("FlamerOH_Weapon");

	m_fDissloveTime = 0.7f;

	return S_OK;
}

void CFalmerOH_Weapon::Tick(_float _fTimeDelta)
{	
	/* 내 행렬 * (소캣 뼈의 컴바인드 행렬 * 소캣의 행렬 * 페어런트의 월드 행렬) */
	_float4x4 matSocketCombined = m_pSocketBone->Get_CombinedTransformationMatrix();

	_matrix		WorldMatrix = XMLoadFloat4x4(&matSocketCombined) *
		XMLoadFloat4x4(&m_matSocketPivot);

	WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
	WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
	WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix() * WorldMatrix);

	m_pColliderCom->Update(XMLoadFloat4x4(&m_matWorld));

	if (g_curLevel != LEVEL_TOOL)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);
	
		CGameObject* m_pPlayer = pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Player"));
		CGameObject* m_pPlayerWeapon = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Part(CPlayer::PART_WEAPON);
	
		pGameInstance->Collision_Out(m_pColliderCom, dynamic_cast<CCollider*>(m_pPlayerWeapon->Get_Component(TEXT("Com_Collider_OBB"))));
		pGameInstance->Collision_Out(m_pColliderCom, dynamic_cast<CCollider*>(dynamic_cast<CPlayer*>(m_pPlayer)->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))));
	
		Safe_Release(pGameInstance);
	}

	if (dynamic_cast<CFalmer_OneHand*>(m_pParent)->Get_IsDissloving())
	{
		m_fCurDissloveTime += _fTimeDelta;

		if (m_fCurDissloveTime > m_fDissloveTime)
		{
			m_bReadyDead = true;
		}
	}
}

void CFalmerOH_Weapon::LateTick(_float _fTimeDelta)
{
#ifdef _DEBUG
	m_pRendererCom->Add_Debug(m_pColliderCom);
#endif
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CFalmerOH_Weapon::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_matWorld)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	// 뷰, 투영 행렬을 던져준다.
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::D3DTS_VIEW)))
		return E_FAIL;
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	if (dynamic_cast<CFalmer_OneHand*>(m_pParent)->Get_IsDissloving())
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture", 0)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fCurDissloveTime, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveTime", &m_fDissloveTime, sizeof(_float))))
			return E_FAIL;
	}

	// 메시 몇개
	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_MaterialTexture(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_MaterialTexture(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (!dynamic_cast<CFalmer_OneHand*>(m_pParent)->Get_IsDissloving())
		{
			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;
		}
		else if (dynamic_cast<CFalmer_OneHand*>(m_pParent)->Get_IsDissloving())
		{
			if (FAILED(m_pShaderCom->Begin(1)))
				return E_FAIL;
		}

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

const char* CFalmerOH_Weapon::Get_SoketBoneName()
{
	return m_pSocketBone->Get_BoneName();
}

void CFalmerOH_Weapon::Set_SoketBone(CBone* _pSocketBone)
{
	Safe_Release(m_pSocketBone);
	m_pSocketBone = _pSocketBone;
	Safe_AddRef(_pSocketBone);
}

HRESULT CFalmerOH_Weapon::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Shader_VtxNonAnimMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_Weapon_FalmerAxe"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CBounding_OBB::BOUNDING_OBB_DESC OBBDesc = {};

	OBBDesc.vExtents = _float3(7.f, 7.f, 25.f);
	OBBDesc.vDegree = _float3(0.f, 0.f, 0.f);
	OBBDesc.vCenter = _float3(0.f, 0.f, OBBDesc.vExtents.z / 2.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_OBB"),
		TEXT("Com_Collider_OBB"), (CComponent**)&m_pColliderCom, &OBBDesc)))
		return E_FAIL;

	m_pColliderCom->Set_OwnerObj(m_pParent);

	return S_OK;
}

CFalmerOH_Weapon* CFalmerOH_Weapon::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CFalmerOH_Weapon* pInstance = new CFalmerOH_Weapon(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CFalmerOH_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFalmerOH_Weapon::Clone(void* _pArg)
{
	CFalmerOH_Weapon* pInstance = new CFalmerOH_Weapon(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CFalmerOH_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFalmerOH_Weapon::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pSocketBone);
}
