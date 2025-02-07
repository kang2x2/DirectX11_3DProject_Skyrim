#pragma once

#include "Base.h"
#include "Bin_AIScene.h"

BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	CBone(const CBone& rhs);
	virtual ~CBone() = default;

public:
	HRESULT Initialize(const CBin_AIScene::DESC_NODE* _pAINode, _int _iParentBoneIndex);
	HRESULT Update_CombinedTransformationMatrix(const vector<class CBone*>& _vecBone);

public:
	const char* Get_BoneName() const { return m_szName; }
	_float4x4 Get_CombinedTransformationMatrix() const { return m_CombinedTransformationMatrix; }
	void Set_TransformationMatrix(_fmatrix _matTransformation) { XMStoreFloat4x4(&m_TransformationMatrix, _matTransformation); }

private:
	char	m_szName[MAX_PATH] = ""; // 뼈의 이름

	_float4x4		m_TransformationMatrix; // 자기 자신만의 뼈 행렬(부모를 기준으로)
	_float4x4		m_CombinedTransformationMatrix; // 부모의 뼈 행렬과 곱한 뼈 행렬.

	_int			m_iParentBoneIndex = -1; // 부모 뼈의 인덱스(없으면 -1)

public:
	static CBone* Create(const CBin_AIScene::DESC_NODE* _pAINode, _int _iParentBoneIndex);
	CBone* Clone();
	virtual void Free() override;
};

END

