#pragma once

#include "Client_Defines.h"
#include "CreatureObject.h"

BEGIN(Engine)

class CModel;
class CTransform;
class CShader;
class CRenderer;
class CNavigation;

END

BEGIN(Client)

class CSkyrim_NPC abstract : public CCreatureObject
{
protected:
	CSkyrim_NPC(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkyrim_NPC(const CSkyrim_NPC& rhs);
	virtual ~CSkyrim_NPC() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg); // 사본
	virtual void	PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

protected:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResource();

public:
	void			Play_Animation(_bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex = 0, _bool _bIsReset = false, _bool _bIsQuickChange = false);

	_bool			Get_IsAnimationFin();
	string			Get_CurAnimationName();
	_uint			Get_CurFrameIndex();
	void			Set_KeyFrame(_uint _iFrame);

	_vector			Get_OriginPos() { return m_vOriginPos; }

	void			Set_AnimationStop(_bool _bIsStop);

	class CSkyrimUI_Talk* Get_TalkUI() { return m_pTalk; }
	void			Start_Talk();
	_uint			Get_CurTextID();
	_uint			Get_CurTalkID();
	
	void			Set_VisibleTalkIcon(_bool _bIsVisible);

	virtual void	Set_Idle() {};

protected:
	_matrix* pMatPivot;

	CModel* m_pModelCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CNavigation* m_pNavigationCom = nullptr;

	_vector			m_vOriginPos = {};

	class CPlayer* m_pPlayer = nullptr;
	class CSkyrimUI_Talk* m_pTalk = nullptr;
	class CSkyrimUI_TalkIcon* m_pTalkIcon = nullptr;

public:
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

