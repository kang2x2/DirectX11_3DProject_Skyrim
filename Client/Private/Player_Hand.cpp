#include "framework.h"
#include "Player_Hand.h"

#include "GameInstance.h"
#include "Layer.h"

#include "Player.h"

CPlayer_Hand::CPlayer_Hand(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlayerPart_Base(_pDevice, _pContext)
{

}

CPlayer_Hand::CPlayer_Hand(const CPlayer_Hand& rhs)
	: CPlayerPart_Base(rhs)
{

}

HRESULT CPlayer_Hand::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CPlayer_Hand::Initialize_Clone(void* _pArg)
{
	if (FAILED(__super::Initialize_Clone(_pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_strName = TEXT("Player_Hand");

	//CGameInstance* pGameInstance = CGameInstance::GetInstance();
	//Safe_AddRef(pGameInstance);
	//
	//pGameInstance->Add_CloneObject(g_curLevel, TEXT("Temp"), TEXT("ProtoType_GameObject_GlovesM_Blades"));
	//
	//CGameObject* tempObject = pGameInstance->Find_CloneObject(g_curLevel, TEXT("Temp"), TEXT("블레이즈 건틀릿(남)"));
	//
	////m_pModelComAry[CPlayer::CAM_1ST]->SwapDesc_Armor(
	////	dynamic_cast<CModel*>(tempObject->Get_Component(TEXT("Com_1stModel"))));
	//
	//m_pModelComAry[CPlayer::CAM_3ST]->SwapDesc_Armor(
	//	dynamic_cast<CModel*>(tempObject->Get_Component(TEXT("Com_3stModel"))));
	//
	//
	//Safe_Release(pGameInstance);

	m_pModelComAry[CPlayer::CAM_1ST]->SwapDesc_Armor(m_pBasicModelAry[CPlayer::CAM_1ST]);
	m_pModelComAry[CPlayer::CAM_3ST]->SwapDesc_Armor(m_pBasicModelAry[CPlayer::CAM_3ST]);

	return S_OK;
}

void CPlayer_Hand::Tick(_float _fTimeDelta)
{
	m_pModelComAry[m_ePlayerCamMode]->Play_Animation(_fTimeDelta);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix());
}

void CPlayer_Hand::LateTick(_float _fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CPlayer_Hand::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	// 메시 몇개
	_uint		iNumMeshes = m_pModelComAry[m_ePlayerCamMode]->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelComAry[m_ePlayerCamMode]->Bind_BondMatrices(m_pShaderCom, i, "g_BoneMatrices")))
			return E_FAIL;

		if (FAILED(m_pModelComAry[m_ePlayerCamMode]->Bind_MaterialTexture(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelComAry[m_ePlayerCamMode]->Bind_MaterialTexture(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelComAry[m_ePlayerCamMode]->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

void CPlayer_Hand::SetUp_Animation(_bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex, _bool _bIsReset, _bool _bIsQuickChange)
{
	m_pModelComAry[m_ePlayerCamMode]->SetUp_Animation(_bIsLoop, _strAnimationName, _iChangeIndex, _bIsReset, _bIsQuickChange);
}

_uint CPlayer_Hand::Get_CurFrameIndex()
{
	return m_pModelComAry[m_ePlayerCamMode]->Get_CurFrameIndex();
}

_bool CPlayer_Hand::Get_CurAnimationIsLoop()
{
	return m_pModelComAry[m_ePlayerCamMode]->Get_CurAnimationIsLoop();
}

void CPlayer_Hand::Set_MeshType(CPlayer::PLAYERCAMERA _eCamType)
{
	m_ePlayerCamMode = _eCamType;
	m_pModelCom = m_pModelComAry[m_ePlayerCamMode];
}

void CPlayer_Hand::Change_Equip(CGameObject* _pItem)
{
	if (m_pModelComAry[CPlayer::CAM_1ST] == _pItem->Get_Component(TEXT("Com_1stModel")))
	{
		int i = 0;
	}
	else
	{
		//Safe_Release(m_pModelComAry[CPlayer::CAM_1ST]);
		//Safe_Release(m_pModelComAry[CPlayer::CAM_3ST]);

		m_pModelComAry[CPlayer::CAM_1ST]->SwapDesc_Armor(dynamic_cast<CModel*>(_pItem->Get_Component(TEXT("Com_1stModel"))));
		m_pModelComAry[CPlayer::CAM_3ST]->SwapDesc_Armor(dynamic_cast<CModel*>(_pItem->Get_Component(TEXT("Com_3stModel"))));

		//m_pModelComAry[CPlayer::CAM_1ST] = dynamic_cast<CModel*>(_pItem->Get_Component(TEXT("Com_1stModel")));
		//m_pModelComAry[CPlayer::CAM_3ST] = dynamic_cast<CModel*>(_pItem->Get_Component(TEXT("Com_3stModel")));
	}

}

_bool CPlayer_Hand::Get_IsAnimationFin()
{
	return m_pModelComAry[m_ePlayerCamMode]->Get_IsAnimationFin();
}

string CPlayer_Hand::Get_CurAnimationName()
{
	return m_pModelComAry[m_ePlayerCamMode]->Get_CurAnimationName().c_str();
}


HRESULT CPlayer_Hand::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_PlayerNude_Hand"),
		TEXT("Com_3stModel"), (CComponent**)&m_pModelComAry[CPlayer::CAM_3ST])))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_PlayerNude_1stHand"),
		TEXT("Com_1stModel"), (CComponent**)&m_pModelComAry[CPlayer::CAM_1ST])))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_PlayerNude_Hand"),
		TEXT("Com_3stModelBasic"), (CComponent**)&m_pBasicModelAry[CPlayer::CAM_3ST])))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Model_PlayerNude_1stHand"),
		TEXT("Com_1stModelBasic"), (CComponent**)&m_pBasicModelAry[CPlayer::CAM_1ST])))
		return E_FAIL;

	m_ePlayerCamMode = CPlayer::CAM_1ST;
	// m_ePlayerCamMode = CPlayer::CAM_3ST;
	m_pModelCom = m_pModelComAry[m_ePlayerCamMode];

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Hand::Bind_ShaderResource()
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

CPlayer_Hand* CPlayer_Hand::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CPlayer_Hand* pInstance = new CPlayer_Hand(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CPlayer_Hand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Hand::Clone(void* _pArg)
{
	CPlayer_Hand* pInstance = new CPlayer_Hand(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CPlayer_Hand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Hand::Free()
{
	__super::Free();

	// 지울 때 누수나 에러 나는지 확인.
	for (_int i = 0; i < CPlayer::CAM_END; ++i)
	{
		if (m_pBasicModelAry[i] != nullptr)
			Safe_Release(m_pBasicModelAry[i]);
	}

	for (_int i = 0; i < CPlayer::CAM_END; ++i)
	{
		if (m_pModelComAry[i] != nullptr)
			Safe_Release(m_pModelComAry[i]);
	}
}
