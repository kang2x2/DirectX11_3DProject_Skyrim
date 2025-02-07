#pragma once

#include "Client_Defines.h"
#include "SkyrimUI.h"

BEGIN(Engine)

class CGameObject;
class CTexture;
class CRenderer;
class CShader;
class CVIBuffer_Rect;
class CTransform;

END

BEGIN(Client)

class CSkyrimUI_Talk final : public CSkyrimUI
{
public:
	typedef struct tagPlayerTalkDesc
	{
		_uint		m_iShowTextID = 0;
		wstring		m_strPlayerTalk = TEXT("");
	}PLAYERTALK_DESC;

	typedef struct tagTalkDesc
	{
		CGameObject* pOwner = nullptr;
		_uint       m_iShowDialogIndex = 0;
		wstring		m_strOwnerName = TEXT("");
		map<int, vector<wstring>>	  m_mapNpcTalkData = {};
		vector<PLAYERTALK_DESC>		  m_vecPlayerTalkDesc = {};
	}TALK_DESC;

private:
	CSkyrimUI_Talk(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkyrimUI_Talk(const CSkyrimUI_Talk& rhs);
	virtual ~CSkyrimUI_Talk() = default;

public:
	virtual HRESULT Initialize_ProtoType();
	virtual HRESULT Initialize_Clone(void* _pArg);
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void			Start_Talk() {
		m_bIsTalk = true;
		g_bIsTalk = true;
		m_iCurTextIndex = 0;
		m_iCurPlayerTextIndex = 0;
	}
	_uint	Get_CurTextID() { return m_iCurTextIndex; }

private:
	HRESULT	Ready_Component();
	HRESULT Bind_ShaderResources();

private: /* 사용할 컴포넌트들 */
	CTransform*		m_pTransformBlack = nullptr;
	CTexture*		m_pTextureBlack = nullptr;
	CVIBuffer_Rect* m_pVIBufferBlack = nullptr;
	_float2			m_vBlackPos = {};
	_float2			m_vBlackSize = {};

	CTransform*		m_pTransformLine = nullptr;
	CTexture*		m_pTextureLine = nullptr;
	CVIBuffer_Rect* m_pVIBufferLine = nullptr;
	_float2			m_vLinePos = {};
	_float2			m_vLineSize = {};

	_float			m_fAlpha = 0.f;
	_bool			m_bIsTalk = false;

	TALK_DESC		m_tTalkDesc = {};
	_uint			m_iCurTextIndex = 0;
	_uint			m_iCurPlayerTextIndex = 0;

	wstring			m_strPlayerTalk = TEXT("");
	RECT			m_Rc = {};
	_vector			m_vPlayerTextColor = {};

public:
	static CSkyrimUI_Talk* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CSkyrimUI* Clone(void* _pArg) override;
	virtual void Free()override;
};

END

