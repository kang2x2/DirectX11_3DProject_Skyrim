#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNavigationDesc
	{
		_int iCurIndex = -1;
	}DESC_NAVIGATION;
private:
	CNavigation(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	vector<class CCell*> Get_VecCell() { return m_vecCell; }

public:
	virtual HRESULT Initialize_ProtoType(const wstring& _strNaviMeshPath);
	virtual HRESULT Initialize_Clone(void* _pArg);
	void  Update();
	_bool IsMove(_fvector _vPoint);

	HRESULT Init_Cell(const wstring& _strNaviMeshPath);
	HRESULT Add_Cell(_float3 _vMeshPos[3]);
	HRESULT Cell_PopBack();
	HRESULT SetUp_Neighbors();
	HRESULT Set_CurCell(_fvector vPos);
	_vector Set_OnCell(_fvector _vWorldPos);
	_bool	isMove(_fvector vPoint);


public:

#ifdef _DEBUG
public:
	HRESULT Render();
#endif

private:
	static _float4x4				m_matWorld;
	_int							m_iCurIndex = -1;
	vector<class CCell*>			m_vecCell;

#ifdef _DEBUG
private:
	class CShader*			m_pShaderCom = nullptr;
#endif

public:
	static CNavigation* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext, const wstring& _strNaviMeshPath = TEXT(""));
	virtual CComponent* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

