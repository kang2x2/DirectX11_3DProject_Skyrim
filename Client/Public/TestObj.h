#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CVIBuffer_Rect;
class CRenderer;
class CShader;

END

BEGIN(Engine)

class CTestObj final : public CGameObject
{
private:
	CTestObj(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CTestObj(const CTestObj& rhs);
	virtual ~CTestObj() = default;

public:
	virtual HRESULT Initialize_ProtoType() override; 
	virtual HRESULT Initialize_Clone(void* pArg);
	virtual void	Tick(_float _fTimeDelta) override;
	virtual void	LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTransform*		m_pTransformCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CShader*		m_pShaderCom = nullptr;

private:
	void	Key_Input(_float _fTimeDelta);
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

public:
	static CTestObj* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
