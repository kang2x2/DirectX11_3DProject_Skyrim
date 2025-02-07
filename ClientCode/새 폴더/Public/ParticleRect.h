#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect_Instance;

END

BEGIN(Client)

class CParticleRect final : public CGameObject
{
private:
	CParticleRect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CParticleRect(const CParticleRect& rhs);
	virtual ~CParticleRect() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg); // 사본
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	CRenderer* m_pRendererCom =					nullptr;
	CTransform* m_pTransformCom =				nullptr;
	CShader* m_pShaderCom =						nullptr;
	CTexture* m_pTextureCom =				    nullptr;
	CVIBuffer_Rect_Instance* m_pVIBufferCom =   nullptr;

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResource();

public:
	static CParticleRect* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

