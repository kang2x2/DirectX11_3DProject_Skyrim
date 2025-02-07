#pragma once
#include "Base.h"

BEGIN(Engine)

class CFrustum : public CBase
{
	DECLARE_SINGLETON(CFrustum)
private:
	CFrustum();
	virtual ~CFrustum() = default;
	
public:
	HRESULT Initialize();
	void Tick();
	void Transform_ToLocalSpace(_fmatrix _WorldMatrixInverse);
	_bool IsIn_Frustum_World(_fvector _vWorldPos, _float _fRadius);
	_bool IsIn_Frustum_Local(_fvector _vLocalPos, _float _fRadius);

	void InitOcTree(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, _float3 _vCenter, _float3 _vRootRange);

#ifdef _DEBUG
	HRESULT RenderOcTree();
#endif

private:
	class CPipeLine* m_pPipeLine = { nullptr };
	_float3			m_vOriginalPoints[8];
	_float3			m_vPoints[8];

	_float4			m_WorldPlanes[6];
	_float4			m_LocalPlanes[6];

	class COcTree* m_pOcTree;

private:
	void Make_Planes(const _float3* _pPoints, _float4* _pPlanes);

public:
	virtual void Free() override;

};

END

