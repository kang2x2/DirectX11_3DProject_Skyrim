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

class CEffect_IntroFade final : public CGameObject
{
private:
	CEffect_IntroFade(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CEffect_IntroFade(const CEffect_IntroFade& rhs);
	virtual ~CEffect_IntroFade() = default;

public:
	virtual HRESULT Initialize_ProtoType();
	virtual HRESULT Initialize_Clone(void* pArg);
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	CRenderer* m_pRendererCom = nullptr;
	CShader* m_pShaderCom = nullptr;

	CVIBuffer_Rect* m_pVIBufferBG = nullptr;
	CTexture* m_pTextureBG = nullptr;
	CTransform* m_pTransformBG = nullptr;

	CVIBuffer_Rect* m_pVIBufferTitle = nullptr;
	CTexture* m_pTextureTitle = nullptr;
	CTransform* m_pTransformTitle = nullptr;

	_float		m_fAlphaBG = 1.f;
	_float		m_fAlphaTitle = 0.f;

	_float		m_fWaitTime = 0.f;
	_float		m_fWaitTimeTitle = 0.f;

	_uint		m_iEventLevel = 0;
private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResource();

	void	Fade_In(_float _fTimeDelta);
	void	Fade_Out(_float _fTimeDelta);

public:
	static CEffect_IntroFade* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

