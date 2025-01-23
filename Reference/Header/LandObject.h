#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLandObject abstract : public CGameObject
{
public:
	typedef struct tagLandObjectDesc
	{
		class CTransform*   pTerrainTransform = nullptr;
		class CMesh*		pTerrainMesh = nullptr;
		_bool				bIsOnObject = false;
	}LANDOBJECT_DESC;

protected:
	CLandObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CLandObject(const CLandObject& rhs);
	virtual ~CLandObject() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring & _strModelComTag, void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

protected:
	_vector SetUp_OnTerrain(_fvector _vWorldPos);

private:
	vector<LANDOBJECT_DESC> m_vecDesc;

public:
	virtual CGameObject* Clone(void* _pArg) = 0;
	virtual void Free() override;
};

END

