#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CNavigation;

END

BEGIN(Client)

class CCamera_Player final : public CCamera
{
public:
	typedef struct tagFREE_PLAYERCAMERA_DESC : public CCamera::CAMERA_DESC
	{
		class CPlayer*		pPlayer = nullptr;
		class CTransform*   pPlayerTransform = nullptr;
		_float				fMouseSensitive = 0.f;
	}FREE_PLAYERCAMERA_DESC;

private:
	CCamera_Player(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CCamera_Player(const CCamera_Player& rhs);
	virtual ~CCamera_Player() = default;

public:
	virtual HRESULT Initialize_ProtoType() override; // 원본
	virtual HRESULT Initialize_Clone(void* pArg) override; // 사본
	void	Tick_3st(_float4 _vTargetPos, _float _fTimeDelta);
	void	Tick_1st(CTransform* _pParentTransform, _float4x4 _matSocket, _float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta) override;

public:
	void	Set_PlayerCamLook(_vector _vLook) {
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, _vLook);
	}
	void	Set_PlayerCamPos(_vector _vPos)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vPos);
	}

private:
	HRESULT Ready_Component();

public:
	void Mouse_Fix();
	void Zoom(_float _fTimeDelta);

private:
	CNavigation* m_pNavigationCom[STAGE_END];

	class CPlayer*			m_pPlayer = nullptr;
	class CTransform*		m_pPlayerTransform = nullptr;

	_matrix			m_matAccumulateRotX = XMMatrixIdentity();
	_matrix			m_matAccumulateRotY = XMMatrixIdentity();
	_matrix			matTempRotX = XMMatrixIdentity();
	_matrix			matWorld = XMMatrixIdentity();

	_float4			m_vCamPos = {};

	_float			m_fMouseSensitive = 0.f;
	_float			m_RotationSpeed = 0.1f; // 회전 속도 (마우스 입력에 따라 조절 가능)
	_float			m_fMouseXAngle = 0.f; // 마우스 위 아래 회전 제한 두기 위한 변수.

	_bool			m_bIsChangeCamMode = false;
	_matrix			m_matSaveRotX = XMMatrixIdentity();
	_float4			vDestCamPos = {};
	_float4			vResultCamPos = {};
	_vector			vFinalCamPos = {};
public:
	static CCamera_Player* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};
END

