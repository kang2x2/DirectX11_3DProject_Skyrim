#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CNormal_Camera final : public CCamera
{
private:
	CNormal_Camera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CNormal_Camera(const CNormal_Camera& rhs);
	virtual ~CNormal_Camera() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);

private:
	CTransform* m_pTransformCom;

private:
	HRESULT Ready_Components();

public:
	static CNormal_Camera* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CCamera* Clone(void* pArg) override;
	virtual void Free() override;
};

END

