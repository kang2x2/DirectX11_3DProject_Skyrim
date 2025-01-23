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

class CSkyrimUI_BossHpBar final : public CSkyrimUI
{
private:
	CSkyrimUI_BossHpBar(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkyrimUI_BossHpBar(const CSkyrimUI_BossHpBar& rhs);
	virtual ~CSkyrimUI_BossHpBar() = default;

public:
	virtual HRESULT Initialize_ProtoType();
	virtual HRESULT Initialize_Clone(void* _pArg);
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fSp, _float _fTimeDelta);
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
	wstring		m_TextName = TEXT("");
public:
	void Set_BarShow() { m_bIsShow = true; }

public:
	static CSkyrimUI_BossHpBar* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CSkyrimUI* Clone(void* _pArg) override;
	virtual void Free()override;
};

END

