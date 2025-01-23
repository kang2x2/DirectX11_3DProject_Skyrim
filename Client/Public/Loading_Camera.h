#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CLoading_Camera final : public CCamera
{
public:
	typedef struct tagLoading_Camera_Desc : public CCamera::CAMERA_DESC
	{
		_float			fMouseSensitive = 0.f;
	}LOADING_CAMERA_DESC;

private:
	CLoading_Camera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CLoading_Camera(const CLoading_Camera& rhs);
	virtual ~CLoading_Camera() = default;

public:
	virtual HRESULT Initialize_ProtoType() override; // 원본
	virtual HRESULT Initialize_Clone(void* pArg) override; // 사본
	virtual void	Tick(_float _fTimeDelta) override;
	virtual void	LateTick(_float _fTimeDelta) override;

public:
	static CLoading_Camera* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

