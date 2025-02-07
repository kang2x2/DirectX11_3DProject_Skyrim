#pragma once

#include "Client_Defines.h"
#include "PlayerPart_Base.h"
#include "Camera_Player.h"

BEGIN(Engine)

class CTransform;
class CBone;

END

BEGIN(Client)

class CPlayer_CameraPart final : public CPlayerPart_Base
{
public:
	typedef struct tagPartDesc : public CPlayerPart_Base::PART_DESC
	{
		CBone*					pSocketBone = nullptr;
		CTransform*				pBodyTransform = nullptr;
		_float4x4				matSocketPivot;
	}PLAYER_CAMERAPART_DESC;

private:
	CPlayer_CameraPart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CPlayer_CameraPart(const CPlayer_CameraPart& rhs);
	virtual ~CPlayer_CameraPart() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* _pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void		Set_PivotMatrix(_float4x4 _matPivot) { m_matSocketPivot = _matPivot; }
	void		Set_SoketBone(CBone* _pBone);
	const char* Get_SoketBoneName();

	_vector		Get_PlayerCamLook() { return m_pPlayerCamera->Get_CamLook(); }
	void		Set_PlayerCamLook(_vector _vLook);
	void		Set_PlayerCamPos(_vector _vPos) { m_pPlayerCamera->Set_PlayerCamPos(_vPos); }

public:
	HRESULT Ready_Component();
	HRESULT Ready_Camera(void* _pArg);

private:
	CBone*					m_pSocketBone = nullptr;
	_float4x4				m_matSocketPivot;
	CTransform*				m_pBodyTransform = nullptr;

	CCamera_Player*			m_pPlayerCamera = nullptr;

	_matrix			m_matAccumulateRotX = XMMatrixIdentity();
	_matrix			m_matAccumulateRotY = XMMatrixIdentity();
	_float			m_fMouseSensitive = 0.2f;
	_float			rotationSpeed = 0.1f; // 회전 속도 (마우스 입력에 따라 조절 가능)


public:
	static  CPlayer_CameraPart* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

