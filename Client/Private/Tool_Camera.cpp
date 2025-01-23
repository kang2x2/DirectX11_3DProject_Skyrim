#include "framework.h"
#include "Tool_Camera.h"

#include "GameInstance.h"

CTool_Camera::CTool_Camera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCamera(_pDevice, _pContext)
{
}

CTool_Camera::CTool_Camera(const CTool_Camera& rhs)
	: CCamera(rhs)
{
}

HRESULT CTool_Camera::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CTool_Camera::Initialize_Clone(void* pArg)
{
	TOOL_CAMERA_DESC* FreeCameraDesc = (TOOL_CAMERA_DESC*)pArg;

	m_fMouseSensitive = FreeCameraDesc->fMouseSensitive;

	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	m_bHasMesh = false;
	m_strName = TEXT("Tool_Camera");

	return S_OK;
}

void CTool_Camera::Tick(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyDown('P'))
	{
		m_bIsStop = !m_bIsStop;
	}

	if (!m_bIsStop)
	{
#pragma region KeyBoard

		// 키 입력
		if (pGameInstance->Get_DIKeyPress('W'))
		{
			m_pTransformCom->Go_Foward(_fTimeDelta);
		}
		if (pGameInstance->Get_DIKeyPress('S'))
		{
			m_pTransformCom->Go_Backward(_fTimeDelta);
		}
		if (pGameInstance->Get_DIKeyPress('A'))
		{
			m_pTransformCom->Go_Left(_fTimeDelta);
		}
		if (pGameInstance->Get_DIKeyPress('D'))
		{
			m_pTransformCom->Go_Right(_fTimeDelta);
		}

#pragma endregion

#pragma region Mouse

		// 휠 클릭
		if (pGameInstance->Get_DIMouseState(CInput_Device::MKS_WHEELBUTTON) & 0x80)
		{
			// 마우스 입력
			_long mouseMove = 0l;

			// Shift 안 눌리고 휠만 : 회전
			if (!pGameInstance->Get_DIKeyPress(VK_LSHIFT))
			{
				// 일반적인 y축으로 회전 할 것. 
				if (mouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::MMS_X)) // (y축을 회전하면 x축이 움직이기에.)
					m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), mouseMove * m_fMouseSensitive * _fTimeDelta);

				// 카메라 월드 행렬의 right 축을 기준으로 회전할 것.
				if (mouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::MMS_Y))
					m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), mouseMove * m_fMouseSensitive * _fTimeDelta);
			}

			// Shift + MouseWheel = 카메라 이동.
			if (pGameInstance->Get_DIKeyPress(VK_LSHIFT))
			{
				_long mouseMove = 0l;

				// down = -1, up = 1
				if (mouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::MMS_Y))
				{
					if (mouseMove > 0)
						m_pTransformCom->Go_Up(_fTimeDelta);
					else if (mouseMove < 0)
						m_pTransformCom->Go_Down(_fTimeDelta);
				}

				if (mouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::MMS_X))
				{
					if (mouseMove > 0)
						m_pTransformCom->Go_Left(_fTimeDelta);
					else if (mouseMove < 0)
						m_pTransformCom->Go_Right(_fTimeDelta);
				}
			}
		}
		// 휠 클릭이 아닌 굴리기
		else
		{
			_long mouseMove = 0l;

			if (mouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::MMS_WHEEL))
			{
				_float4 vPos;
				XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

				if (mouseMove < 0 && pGameInstance->Get_DIKeyPress(VK_LCONTROL))
				{
					Zoom_Out(300.f, _fTimeDelta);
				}
				else if (mouseMove > 0 && pGameInstance->Get_DIKeyPress(VK_LCONTROL))
				{
					Zoom_In(300.f, _fTimeDelta);
				}
			}

		}
	}

	matView = m_pTransformCom->Get_WorldMatrix_Inverse();

	Safe_Release(pGameInstance);

#pragma endregion

	__super::Tick(_fTimeDelta);
}

void CTool_Camera::LateTick(_float _fTimeDelta)
{
}

CTool_Camera* CTool_Camera::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CTool_Camera* pInstance = new CTool_Camera(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CTool_Camera ProtoType");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTool_Camera::Clone(void* pArg)
{
	CTool_Camera* pInstance = new CTool_Camera(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Fail Clone : CTool_Camera Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool_Camera::Free()
{
	__super::Free();
}
