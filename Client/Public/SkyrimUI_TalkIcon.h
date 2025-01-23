#pragma once

#include "SkyrimUI.h"

BEGIN(Engine)

class CRenderer;
class CShader;
class CTransform;
class CTexture;
class CVIBuffer_Rect;

END

BEGIN(Client)

class CSkyrimUI_TalkIcon final : public CSkyrimUI
{
public:
	typedef struct tagTalkIconDesc
	{
		wstring strTargetName = TEXT("");
	}TALKICON_DESC;

private:
	CSkyrimUI_TalkIcon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkyrimUI_TalkIcon(const CSkyrimUI_TalkIcon& rhs);
	virtual ~CSkyrimUI_TalkIcon() = default;

public:
	virtual HRESULT Initialize_ProtoType();
	virtual HRESULT Initialize_Clone(void* pArg);
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void	Set_VisibleTalkIcon(_bool _bIsVisible) { m_bIsVisible = _bIsVisible; }

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

private:
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

	TALKICON_DESC	m_tIconDesc = {};

	_bool		m_bIsVisible = false;

public:
	static CSkyrimUI_TalkIcon* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CSkyrimUI* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

