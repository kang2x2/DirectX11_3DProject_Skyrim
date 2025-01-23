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

class CEffect_FadeBlack final : public CGameObject
{
public:
	typedef struct tagFadeDesc
	{
		_bool  bIsFadeIn = false;
		_float fFadeTime = 0.f;
		STAGEID eDestStage = STAGE_END;
	}FADE_DESC;

private:
	CEffect_FadeBlack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CEffect_FadeBlack(const CEffect_FadeBlack& rhs);
	virtual ~CEffect_FadeBlack() = default;

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

	_float		m_fAlpha = 0.f;

	FADE_DESC m_tFadeDesc = {};

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResource();

	void	Fade_In(_float _fTimeDelta);
	void	Fade_Out(_float _fTimeDelta);

public:
	// 0이면 인, 1이면 아웃
	static CEffect_FadeBlack* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

