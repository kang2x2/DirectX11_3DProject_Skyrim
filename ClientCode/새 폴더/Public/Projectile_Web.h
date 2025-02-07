#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CModel;
class CRenderer;
class CShader;
class CTransform;
class CTexture;
class CCollider;

END

BEGIN(Client)

class CProjectile_Web final : public CGameObject
{
private:
	CProjectile_Web(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CProjectile_Web(const CProjectile_Web& rhs);
	virtual ~CProjectile_Web() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResource();

private:
	CModel* m_pModelCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CCollider* m_pColliderCom = nullptr;

	_float		m_fLifeTime = 0.f;
	_float      m_fTimeDelta = 0.f;

public:
	static CProjectile_Web* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

