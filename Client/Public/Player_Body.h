#pragma once

#include "Client_Defines.h"
#include "PlayerPart_Base.h"

BEGIN(Engine)

class CBone;
class CModel;
class CRenderer;
class CShader;
class CCollider;
class CTransform;

END

BEGIN(Client)

class CPlayer_Body final : public CPlayerPart_Base
{
private:
	CPlayer_Body(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CPlayer_Body(const CPlayer_Body& rhs);
	virtual ~CPlayer_Body() = default;

public:
	_bool Get_IsAnimationFin();
	string Get_CurAnimationName();
	string Get_NextAnimationName();

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void		SetUp_Animation(_bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex, _bool _bIsReset, _bool _bIsQuickChange);
	_uint		Get_CurFrameIndex();
	_bool		Get_CurAnimationIsLoop();

	void		Set_MeshType(CPlayer::PLAYERCAMERA _eCamType);
	void		Change_Equip(CGameObject* _pItem);
private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResource();

private:
	CCollider*  m_pColliderCom = nullptr;
	CModel*		m_pBasicModelAry[CPlayer::CAM_END] = { nullptr, nullptr };
	CModel*		m_pModelComAry[CPlayer::CAM_END] = { nullptr, nullptr };

	CPlayer::PLAYERCAMERA m_ePlayerCamMode = CPlayer::CAM_END;

public:
	static CPlayer_Body* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

