#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRenderer;
class CShader;
class CVIBuffer_Rect;
class CTransform;

END

BEGIN(Client)

class CInventory_PartBase abstract : public CGameObject
{
public:
	typedef struct tagInvenPartDesc
	{
		_float2   vBlackPos = {};
		_float2   vBlackSize = {};

		_float2   vLine1Pos = {};
		_float2   vLine1Size = {};

		_float2   vLine2Pos = {};
		_float2   vLine2Size = {};
	}INVENPART_DESC;

protected:
	CInventory_PartBase(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CInventory_PartBase(const CInventory_PartBase& rhs);
	virtual ~CInventory_PartBase() = default;

public:
	virtual HRESULT Initialize_ProtoType(); 
	virtual HRESULT Initialize_Clone(void* _pArg); 
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

protected:
	HRESULT	Ready_Component();
	HRESULT Bind_ShaderResources();

protected: /* 사용할 컴포넌트들 */
	CRenderer*		m_pRendererCom = nullptr;
	CShader*		m_pShaderCom = nullptr;

	CTransform*		m_pTransformBlack = nullptr;
	CTexture*		m_pTextureBlack = nullptr;
	CVIBuffer_Rect* m_pVIBufferBlack = nullptr;
	_float2			m_vBlackPos = {};
	_float2			m_vBlackSize = {};

	CTransform*		m_pTransformLine1 = nullptr;
	CTexture*		m_pTextureLine1 = nullptr;
	CVIBuffer_Rect* m_pVIBufferLine1 = nullptr;
	_float2			m_vLine1Pos = {};
	_float2			m_vLine1Size = {};

	CTransform*		m_pTransformLine2 = nullptr;
	CTexture*		m_pTextureLine2 = nullptr;
	CVIBuffer_Rect* m_pVIBufferLine2 = nullptr;
	_float2			m_vLine2Pos = {};
	_float2			m_vLine2Size = {};

	_float			m_fAlpha = 0.f;

public:
	virtual CGameObject* Clone(void* _pArg) = 0;
	virtual void Free()override;
};

END

