#pragma once

#include "Client_Defines.h"
#include "SkyrimUI.h"

BEGIN(Engine)

class CTexture;
class CRenderer;
class CShader;
class CVIBuffer_Rect;
class CTransform;

END

BEGIN(Client)

class CSkyrimUI_HpBar final : public CSkyrimUI
{
private:
	CSkyrimUI_HpBar(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkyrimUI_HpBar(const CSkyrimUI_HpBar& rhs);
	virtual ~CSkyrimUI_HpBar() = default;

public:
	virtual HRESULT Initialize_ProtoType();
	virtual HRESULT Initialize_Clone(void* _pArg);
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fHp, _float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT	Ready_Component();
	HRESULT Bind_ShaderResources();

private: /* 사용할 컴포넌트들 */
	CTransform* m_pTransformEmpty = nullptr;
	CTexture* m_pTextureEmpty = nullptr;
	CVIBuffer_Rect* m_pVIBufferEmpty = nullptr;
	_float2			m_vEmptyPos = {};
	_float2			m_vEmptySize = {};

	CTransform* m_pTransformFill = nullptr;
	CTexture* m_pTextureFill = nullptr;
	CVIBuffer_Rect* m_pVIBufferFill = nullptr;
	_float2			m_vFillPos = {};
	_float2			m_vFillSize = {};

	_float		m_fAlpha = 0.f;
	_bool		m_bIsShow = false;

public:
	static CSkyrimUI_HpBar* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CSkyrimUI* Clone(void* _pArg) override;
	virtual void Free()override;
};

END

