#pragma once

#include "Base.h"
#include "Bin_AIScene.h"

/*
	1. aiScene		: 뼈의 상태를 가지고 있음.
	2. aiBone		: 뼈들을 메시 기준으로 다시 재구성하여 가지고 있다.
					  이 뼈는 이 메시의 어떤 정점에게 몇 퍼센트의 영향을 주는지에 대한 정보를 가지고 있다.
	3. aiNodel		: 뼈들의 상속관계를 표현하기 위한 데이터를 가지고 있다.
					  뼈들의 상태행렬을 가지고 있다.
	4. aiAnimNode : 
*/

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

