#pragma once

#include "GameObject.h"
#include "Transform.h"

/* 클라이언트에서 만든 카메라 들의 부모가 된다. */
/* 모든 카메라에게 필요한 기능을 공통적으로 구현하여 자식에게 상속내려주기위해서. */

/* 모든 카메라에게 필요한 기능 :  */
/* 카메라의 상태 행렬을 이용하여 뷰스페이스 변환행렬.*/
/* 투영행렬(fov, aspect, near, far)을 설정하는작업.  */

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc : public CTransform::tagTransformDesc
	{
		_float4 vEye, vAt;
		_float  fFovY, fAspect, fNear, fFar;
		_matrix matView;
	}CAMERA_DESC;

protected:
	CCamera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_ProtoType() override; // 원본
	virtual HRESULT Initialize_Clone(void* pArg) override; // 사본
	virtual void	Tick(_float _fTimeDelta) override;
	virtual void	LateTick(_float _fTimeDelta) override;

public:
	const _vector Get_CamLook();
	void	Zoom_In(_float _fZoomSpeed, _float _fTimeDelta);
	void	Zoom_Out(_float _fZoomSpeed, _float _fTimeDelta);
	
	// void Set_CamLook(const _vector& _vPlayerLook) { m_pTransformCom->Set_State(CTransform::STATE_LOOK, _vPlayerLook); }
protected:
	class CTransform* m_pTransformCom = nullptr;
	class CPipeLine*  m_pPipeLine = nullptr;

	_float4 vEye, vAt;
	_float  fFovY, fAspect, fNear, fFar;
	_matrix matView;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END

