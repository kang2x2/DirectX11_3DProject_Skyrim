#include "framework.h"
#include "Camera_Player.h"

#include "GameInstance.h"

#include "Player.h"

CCamera_Player::CCamera_Player(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : CCamera(_pDevice, _pContext)
{
    for (_int i = 0; i < STAGE_END; ++i)
    {
        m_pNavigationCom[i] = nullptr;
    }
}

CCamera_Player::CCamera_Player(const CCamera_Player& rhs)
    : CCamera(rhs)
{
    for (_int i = 0; i < STAGE_END; ++i)
    {
        m_pNavigationCom[i] = nullptr;
    }

}

HRESULT CCamera_Player::Initialize_ProtoType()
{
    return S_OK;
}

HRESULT CCamera_Player::Initialize_Clone(void* pArg)
{
    FREE_PLAYERCAMERA_DESC* PlayerCameraDesc = (FREE_PLAYERCAMERA_DESC*)pArg;

    m_pPlayer = PlayerCameraDesc->pPlayer;
    m_pPlayerTransform = PlayerCameraDesc->pPlayerTransform;
    m_fMouseSensitive = PlayerCameraDesc->fMouseSensitive;

    if (FAILED(__super::Initialize_Clone(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_strName = TEXT("GamePlay_PlayerCamera");

    XMStoreFloat4(&m_vCamPos, m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

    m_vCamPos.y += 3.f;
    m_vCamPos.z -= 3.f;

    return S_OK;
}

void CCamera_Player::Tick_3st(_float4 _vTargetPos, _float _fTimeDelta)
{
    if (!g_bIsTalk)
        Mouse_Fix();

    Zoom(_fTimeDelta);

    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(pGameInstance);

    /* 마우스 움직임 정도를 얻는다.*/
    _long mouseMoveY = pGameInstance->Get_DIMouseMove(CInput_Device::MMS_X);
    _long mouseMoveX = pGameInstance->Get_DIMouseMove(CInput_Device::MMS_Y);

    /* 위 아래 회전 각도 제한 두기 */
    _vector vPlayerUp = m_pPlayerTransform->Get_State(CTransform::STATE_UP);
    m_fMouseXAngle = XMVectorGetX(XMVector3AngleBetweenVectors(m_pTransformCom->Get_State(CTransform::STATE_LOOK), vPlayerUp));
    m_fMouseXAngle = fabs(XMConvertToDegrees(m_fMouseXAngle));

    // cout << m_fMouseXAngle << endl;

    /* 회전 행렬 생성. */
    _matrix matRot = XMMatrixRotationY(m_RotationSpeed * mouseMoveY * _fTimeDelta);
    m_matAccumulateRotY = XMMatrixMultiply(m_matAccumulateRotY, matRot);
    if (m_fMouseXAngle > 170.f)
    {
        m_matAccumulateRotX = m_matSaveRotX;
    }
    if (m_fMouseXAngle <= 170.f)
    {
        m_matSaveRotX = m_matAccumulateRotX;
        matRot = XMMatrixRotationX(m_RotationSpeed * mouseMoveX * _fTimeDelta);
        m_matAccumulateRotX = XMMatrixMultiply(m_matAccumulateRotX, matRot);
    }


    /* 위에서 계산한 회전 행렬들을 카메라의 world 행렬과 모두 연산한다.*/
/*  1. 카메라의 위치를 설정.
    2. 위에서 구한 마우스에 따른 회전 행렬들을 곱한다.(무조건 x축이 먼저)
    3. 대상의 위치를 곱한다.
    결론. 카메라의 위치 행렬에서 타겟의 떨어져서 공전이 가능. */
    matWorld = XMMatrixTranslation(m_vCamPos.x, m_vCamPos.y, m_vCamPos.z);
    matWorld = XMMatrixMultiply(matWorld, m_matAccumulateRotX);
    matWorld = XMMatrixMultiply(matWorld, m_matAccumulateRotY);
    matWorld *= XMMatrixRotationY(XMConvertToRadians(90.f));
    matWorld = XMMatrixMultiply(matWorld, XMMatrixTranslation(_vTargetPos.x, _vTargetPos.y, _vTargetPos.z));

    m_pTransformCom->Set_WorldMatrix(matWorld);

    _vTargetPos.y += 1.5f;

    m_pTransformCom->LookAt(XMLoadFloat4(&_vTargetPos));
   
    Safe_Release(pGameInstance);

    __super::Tick(_fTimeDelta);
}

void CCamera_Player::Tick_1st(CTransform* _pParentTransform, _float4x4 _matSocket, _float _fTimeDelta)
{
    if (!g_bIsTalk)
        Mouse_Fix();

    m_pTransformCom->Set_WorldMatrix(_pParentTransform->Get_WorldMatrix());
    
    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(pGameInstance);

    _long	MouseMove = 0l;

    if (MouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::MMS_X))
        m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * m_fMouseSensitive * _fTimeDelta);
    if (MouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::MMS_Y))
        m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMove * m_fMouseSensitive * _fTimeDelta);

    if(!g_bIsTalk)
        _pParentTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());

    Safe_Release(pGameInstance);

    m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&_matSocket));

    __super::Tick(_fTimeDelta);
}

void CCamera_Player::LateTick(_float _fTimeDelta)
{
}

HRESULT CCamera_Player::Ready_Component()
{
    /* Com_Navigation */
    CNavigation::DESC_NAVIGATION		NavigationDesc;
    NavigationDesc.iCurIndex = 0;

    if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Navigation_WhiteRun"),
        TEXT("Com_Navigation_WhiteRun"), (CComponent**)&m_pNavigationCom[STAGE_WHITERUN], &NavigationDesc)))
        return E_FAIL;

    if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Navigation_Dungeon"),
        TEXT("Com_Navigation_Dungeon"), (CComponent**)&m_pNavigationCom[STAGE_DUNGEON], &NavigationDesc)))
        return E_FAIL;

    return S_OK;
}

void CCamera_Player::Mouse_Fix()
{
    if (!g_bIsPause)
    {
        POINT		pt{ g_iWinSizeX >> 1, g_iWinSizeY >> 1 };

        ClientToScreen(g_hWnd, &pt);
        SetCursorPos(pt.x, pt.y);
    }

}

void CCamera_Player::Zoom(_float _fTimeDelta)
{
    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(pGameInstance);

    _long mouseMove = 0l;
    if (mouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::MMS_WHEEL))
    {
        // in
        if (mouseMove > 0)
        {
            m_pTransformCom->Go_Foward(_fTimeDelta);
        }
        // out
        else if (mouseMove < 0)
        {
            m_pTransformCom->Go_Backward(_fTimeDelta);
        }
    }

    Safe_Release(pGameInstance);
}

CCamera_Player* CCamera_Player::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CCamera_Player* pInstance = new CCamera_Player(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_ProtoType()))
    {
        MSG_BOX("Fail Create : CCamera_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCamera_Player::Clone(void* _pArg)
{
    CCamera_Player* pInstance = new CCamera_Player(*this);

    if (FAILED(pInstance->Initialize_Clone(_pArg)))
    {
        MSG_BOX("Fail Clone : CCamera_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCamera_Player::Free()
{
    __super::Free();

    for (_int i = 0; i < STAGE_END; ++i)
    {
        Safe_Release(m_pNavigationCom[i]);
    }
}
