#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Terrain;
class CShader;
class CTransform;
END

BEGIN(Client)

class CTerrain final : public CGameObject
{
public:
	enum TEXTUER { TEX_DIFFUSE, TEX_MASK, TEX_BRUSH, TEX_END };
private:
	CTerrain(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_ProtoType();
	virtual HRESULT Initialize_Clone(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom[TEX_END] = { nullptr };
	CVIBuffer_Terrain*	m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CTerrain* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

