#pragma once

#include "Client_Defines.h"
#include "CarlottaPart_Base.h"

BEGIN(Engine)

class CBone;
class CModel;
class CRenderer;
class CShader;
class CTransform;

END

BEGIN(Client)

class CCarlotta_Head final : public CCarlottaPart_Base
{
private:
	CCarlotta_Head(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CCarlotta_Head(const CCarlotta_Head& rhs);
	virtual ~CCarlotta_Head() = default;

public:
	_bool Get_IsAnimationFin();
	string Get_CurAnimationName();

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void		Set_AnimationIndex(_bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex, _bool _bIsReset, _bool _bIsQuickChange);
	_uint		Get_CurFrameIndex();
	_bool		Get_CurAnimationIsLoop();
private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResource();

public:
	static CCarlotta_Head* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

