#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CLogo_Camera final : public CCamera
{
public:
	typedef struct tagLogo_Camera_Desc : public CCamera::CAMERA_DESC
	{

	}LOGO_CAMERA_DESC;

private:
	CLogo_Camera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CLogo_Camera(const CLogo_Camera& rhs);
	virtual ~CLogo_Camera() = default;

public:
	virtual HRESULT Initialize_ProtoType() override; // 원본
	virtual HRESULT Initialize_Clone(void* pArg) override; // 사본
	virtual void	Tick(_float _fTimeDelta) override;
	virtual void	LateTick(_float _fTimeDelta) override;

public:
	static CLogo_Camera* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

