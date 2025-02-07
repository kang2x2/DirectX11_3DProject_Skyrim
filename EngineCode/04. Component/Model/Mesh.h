#pragma once

#include "VIBuffer.h"
#include "Model.h"
#include "Bin_AIScene.h"

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const
	{
		return m_iMaterialIndex;
	}

	const _float3* Get_VtxPos() { return m_pPos; }
	vector<_ulong> Get_Indices() { return m_vecIndex; }

public:
	void Update_VI(const _fmatrix& _matPivot);

public:
	virtual HRESULT Initialize_ProtoType(const CModel* _pModel, const CBin_AIScene::DESC_MESH* _pAIMesh, _fmatrix _matPivot, CModel::MODEL_TYPE _eType, vector<class CBone*>* _pVecBone);
	virtual HRESULT Initialize_Clone(void* _pArg) override;

public:
	HRESULT Bind_BondMatrices(class CShader* _pShader, vector<class CBone*>& _vecBone, const char* _strConstantName, _fmatrix PivotMatrix);

private:
	_char m_szName[MAX_PATH] = "";
	_uint m_iMaterialIndex = 0;
	_uint m_iNumBones = 0; // 이 메시를 구성하는 정점들이 몇 개 뼈의 영향을 받는지
	/* 이 메시는 어떤 뼈들의 영향을 받는지. 뼈들의 인덱스 == 모델클래스가 들고 있는 뼈배열의 인덱스  */
	vector<_uint> m_vecBoneIndex;
	vector<_float4x4> m_OffsetMatrices;

	// 위치 저장용.
	_float3* m_pPos = nullptr;
	vector<_ulong> m_vecIndex;

private:
	HRESULT Ready_VertexBuffer_For_NonAnim(const CBin_AIScene::DESC_MESH* _pAIMesh, _fmatrix _matPivot);
	HRESULT Ready_VertexBuffer_For_Anim(const CModel* _pModel, const CBin_AIScene::DESC_MESH* _pAIMesh, vector<class CBone*>* _pVecBone);

public:
	static CMesh* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const CModel* _pModel, const CBin_AIScene::DESC_MESH* _pAIMesh, _fmatrix _matPivot, CModel::MODEL_TYPE _eType, vector<class CBone*>* _pVecBone);
	virtual CComponent* Clone(void* _pArg) override;
	virtual void Free() override;
};

END

