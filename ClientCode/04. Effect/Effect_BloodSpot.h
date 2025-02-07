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

class CEffect_BloodSpot final : public CGameObject
{
private:
	CEffect_BloodSpot(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CEffect_BloodSpot(const CEffect_BloodSpot& rhs);
	virtual ~CEffect_BloodSpot() = default;

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

	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

	CTexture* m_pTextureCom2 = nullptr;
	CTransform* m_pTransformCom2 = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom2 = nullptr;

	_float		m_fAlpha = 1.f;
	_float		m_fAlpha2 = 1.f;
	_float		m_fReleaseTime = 0.f;
	_float		m_fReleaseTime2 = 0.f;
	_bool		m_bIsRelease = false;
	_bool		m_bIsRelease2 = false;

	_bool		m_bIsDelete = false;
	_bool		m_bIsDelete2 = false;

	static _uint	m_iBloodSpotIdx;

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResource();

public:
	static CEffect_BloodSpot* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

