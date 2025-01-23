#pragma once

#include "Component.h"

// 화면에 그려질 객체들을 그리는 순서대로 그룹별로 관리하면서 랜더콜 수행.

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RG_PRIORITY, RG_NONLIGHT, RG_NONBLEND, RG_BLEND, RG_UI_0, RG_UI_1, RG_UI_2, RG_END };

private:
	CRenderer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CRenderer(const CRenderer& rhs) = delete;
	virtual ~CRenderer() = default;

public:
	virtual HRESULT Initialize_ProtoType() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	HRESULT Add_RenderGroup(RENDERGROUP _eGroup, class CGameObject* _pCloneObject);
	HRESULT Draw_RenderObjects();

#ifdef _DEBUG
public:
	HRESULT Add_Debug(class CComponent* _pDebug)
	{
		m_ltRenderDebug.push_back(_pDebug);
		Safe_AddRef(_pDebug);
		return S_OK;
	}
#endif

private:
	list<class CGameObject*> m_ltRenderObj[RG_END];

	class CTarget_Manager* m_pTarget_Manager	= nullptr;
	class CLight_Manager* m_pLight_Manager		= nullptr;

	class CVIBuffer_Rect* m_pVIBuffer = nullptr;
	class CShader* m_pShader = nullptr;

	_float4x4 m_matWorld, m_matView, m_matProj;

#ifdef _DEBUG
private:
	list<class CComponent*>				m_ltRenderDebug;
#endif

private:
	HRESULT Render_Priority();
	HRESULT Render_NonLight();
	HRESULT Render_NonBlend();
	HRESULT Render_LightAcc();
	HRESULT Render_Deferred();
	HRESULT Render_Blend();
	HRESULT Render_UI();

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif

public:
	static CRenderer* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

