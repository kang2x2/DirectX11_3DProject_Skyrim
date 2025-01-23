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

class CSkyrimUI_Letter final : public CSkyrimUI
{
private:
	CSkyrimUI_Letter(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkyrimUI_Letter(const CSkyrimUI_Letter& rhs);
	virtual ~CSkyrimUI_Letter() = default;

public:
	virtual HRESULT Initialize_ProtoType();
	virtual HRESULT Initialize_Clone(void* _pArg);
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT	Ready_Component();
	HRESULT Bind_ShaderResources();

private:
	wstring		m_strText = TEXT("");
	_float		m_fWaitTime = 0.f;

public:
	static CSkyrimUI_Letter* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CSkyrimUI* Clone(void* _pArg) override;
	virtual void Free()override;
};

END

