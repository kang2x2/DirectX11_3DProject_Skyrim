#include "framework.h"
#include "Player_CameraPart.h"

#include "GameInstance.h"
#include "PipeLine.h"

#include "Bone.h"

#include "Player.h"
#include "Player_Body.h"
#include "Camera_Player.h"

CPlayer_CameraPart::CPlayer_CameraPart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlayerPart_Base(_pDevice, _pContext)
{
}

CPlayer_CameraPart::CPlayer_CameraPart(const CPlayer_CameraPart& rhs)
	: CPlayerPart_Base(rhs)
{
}

HRESULT CPlayer_CameraPart::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CPlayer_CameraPart::Initialize_Clone(void* _pArg)
{
	/* 카메라 파츠 세팅 */
	if (nullptr != _pArg)
	{
		PLAYER_CAMERAPART_DESC* pCameraPartDesc = (PLAYER_CAMERAPART_DESC*)_pArg;

		m_pBodyTransform = pCameraPartDesc->pBodyTransform;
		m_pSocketBone = pCameraPartDesc->pSocketBone;
		Safe_AddRef(m_pSocketBone);

		m_matSocketPivot = pCameraPartDesc->matSocketPivot;

		if (FAILED(__super::Initialize_Clone(_pArg)))
			return E_FAIL;
	}

	if (FAILED(Ready_Component()))
		return E_FAIL;

	/* 카메라 세팅 */
	if (FAILED(Ready_Camera(_pArg)))
		return E_FAIL;

	m_strName = TEXT("PlayerCamera");

	m_pTransformCom->Fix_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.0f));

	/* 블레이즈 아머 꼈을 때 이상해서 임시로 설정해 둠. */
	_float4 vPos;
	XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	vPos.z -= 0.8f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));

	return S_OK;

}

void CPlayer_CameraPart::Tick(_float _fTimeDelta)
{
	/* 내 행렬 * (소캣 뼈의 컴바인드 행렬 * 소캣의 행렬 * 페어런트의 월드 행렬) */
	_float4x4 matSocketCombined = m_pSocketBone->Get_CombinedTransformationMatrix();

	_matrix		WorldMatrix = XMLoadFloat4x4(&matSocketCombined) *
		XMLoadFloat4x4(&m_matSocketPivot);

	// WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
	// WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
	// WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix() * WorldMatrix);

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, m_pParentTransform->Get_State(CTransform::STATE_POSITION));

	if (dynamic_cast<CPlayer*>(m_pParent)->Get_CamMode() == CPlayer::CAM_3ST)
	{
		m_pPlayerCamera->Tick_3st(vTargetPos, _fTimeDelta);
	}
	else if (dynamic_cast<CPlayer*>(m_pParent)->Get_CamMode() == CPlayer::CAM_1ST)
	{
		m_pPlayerCamera->Tick_1st(m_pParentTransform, m_matWorld, _fTimeDelta);
	}

}

void CPlayer_CameraPart::LateTick(_float _fTimeDelta)
{
}

HRESULT CPlayer_CameraPart::Render()
{
	return S_OK;
}

void CPlayer_CameraPart::Set_SoketBone(CBone* _pBone)
{
	Safe_Release(m_pSocketBone);
	m_pSocketBone = _pBone;
	Safe_AddRef(_pBone);
}

const char* CPlayer_CameraPart::Get_SoketBoneName()
{
	return m_pSocketBone->Get_BoneName();
}

void CPlayer_CameraPart::Set_PlayerCamLook(_vector _vLook)
{
	m_pPlayerCamera->Set_PlayerCamLook(_vLook);
}

HRESULT CPlayer_CameraPart::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_CameraPart::Ready_Camera(void* _pArg)
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	/* 카메라 세팅 */
	CCamera_Player::FREE_PLAYERCAMERA_DESC PlayerCameraDesc;

	PlayerCameraDesc.pPlayer = dynamic_cast<CPlayer*>(m_pParent);
	PlayerCameraDesc.pPlayerTransform = m_pTransformCom;
	PlayerCameraDesc.fMouseSensitive = 0.1f;
	// XMStoreFloat4(&PlayerCameraDesc.vEye, m_pParentTransform->Get_State(CTransform::STATE_POSITION));
	PlayerCameraDesc.vEye = _float4(0.f, 0.f, 0.f, 0.f);
	PlayerCameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	PlayerCameraDesc.fFovY = XMConvertToRadians(60.f);
	PlayerCameraDesc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	PlayerCameraDesc.fNear = 0.3f;
	PlayerCameraDesc.fFar = 1000.f;
	PlayerCameraDesc.fSpeedPerSec = 0.001f;
	PlayerCameraDesc.fRotationRadianPerSec = XMConvertToRadians(90.0f);

	if (FAILED(pGameInstance->Add_CloneObject(g_curLevel, TEXT("Layer_PlayerCamera"), TEXT("ProtoType_GameObject_Camera_Player"), &PlayerCameraDesc)))
		return E_FAIL;

	m_pPlayerCamera = dynamic_cast<CCamera_Player*>(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_PlayerCamera"), TEXT("GamePlay_PlayerCamera")));

	Safe_Release(pGameInstance);

	return S_OK;
}

CPlayer_CameraPart* CPlayer_CameraPart::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CPlayer_CameraPart* pInstance = new CPlayer_CameraPart(_pDevice, _pContext);
	
	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CPlayer_CameraPart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_CameraPart::Clone(void* _pArg)
{
	CPlayer_CameraPart* pInstance = new CPlayer_CameraPart(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CPlayer_CameraPart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_CameraPart::Free()
{
	__super::Free();

	Safe_Release(m_pSocketBone);
	Safe_Release(m_pPlayerCamera);
}
