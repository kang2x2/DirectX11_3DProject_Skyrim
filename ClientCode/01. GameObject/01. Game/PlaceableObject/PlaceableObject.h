#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CPlaceableObject : public CGameObject
{
protected:
	CPlaceableObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CPlaceableObject(const CPlaceableObject& rhs);
	virtual ~CPlaceableObject() = default;

public:
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

protected:
	CModel* m_pModelCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;

	float m_fRadius = 5.f;

protected:
	HRESULT Ready_Component(_uint _iLevel);
	HRESULT Bind_ShaderResource();

public:
	virtual void Free() override;
};

END

