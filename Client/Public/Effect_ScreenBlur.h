#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRenderer;
class CTransform;
class CShader;
class CTexture;
class CVIBuffer_Rect;

END

BEGIN(Client)

class CEffect_ScreenBlur final : public CGameObject
{
private:
	CEffect_ScreenBlur(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CEffect_ScreenBlur(const CEffect_ScreenBlur& rhs);
	virtual ~CEffect_ScreenBlur() = default;

public:
	virtual HRESULT Initialize_ProtoType();
	virtual HRESULT Initialize_Clone(void* pArg);
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

	void Set_Alpha() { m_fAlpha = 1.f; }

private:
	CRenderer* m_pRendererCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransformCom = nullptr;

	_float		m_fAlpha = 0.5f;

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResource();

public:
	static CEffect_ScreenBlur* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

