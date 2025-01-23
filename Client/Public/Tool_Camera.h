#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CTool_Camera final : public CCamera
{
public:
	typedef struct tagTool_Camera_Desc : public CCamera::CAMERA_DESC
	{
		_float			fMouseSensitive = 0.f;
	}TOOL_CAMERA_DESC;

private:
	CTool_Camera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CTool_Camera(const CTool_Camera& rhs);
	virtual ~CTool_Camera() = default;

public:
	virtual HRESULT Initialize_ProtoType() override; // 원본
	virtual HRESULT Initialize_Clone(void* pArg) override; // 사본
	virtual void	Tick(_float _fTimeDelta) override;
	virtual void	LateTick(_float _fTimeDelta) override;

private:
	_float			m_fMouseSensitive = 0.f;

	_bool			m_bIsStop = false;
public:
	static CTool_Camera* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

