#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBuffer_Terrain;
class CTransform;
class CShader;
END

BEGIN(Client)

class CTerrain_Grid final : public CGameObject
{
private:
	CTerrain_Grid(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CTerrain_Grid(const CTerrain_Grid& rhs);
	virtual ~CTerrain_Grid() = default;

public:
	virtual HRESULT Initialize_ProtoType();
	virtual HRESULT Initialize_Clone(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Terrain*  m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

public:
	static CTerrain_Grid* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

