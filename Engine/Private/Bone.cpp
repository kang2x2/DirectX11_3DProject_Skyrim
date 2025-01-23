#include "Bone.h"

CBone::CBone()
{
}

CBone::CBone(const CBone& rhs)
	: m_TransformationMatrix(rhs.m_TransformationMatrix)
	, m_CombinedTransformationMatrix(rhs.m_CombinedTransformationMatrix)
	, m_iParentBoneIndex(rhs.m_iParentBoneIndex)
{
	strcpy_s(m_szName, rhs.m_szName);
}

HRESULT CBone::Initialize(const CBin_AIScene::DESC_NODE* _pAINode, _int _iParentBoneIndex)
{
	m_iParentBoneIndex = _iParentBoneIndex;
	// _pAINode = CHILDREN
	strcpy_s(m_szName, _pAINode->mName.data);

	memcpy(&m_TransformationMatrix, &_pAINode->mTransformation, sizeof(_float4x4));

	// 행렬이 Col행렬로 되어있기에 Raw행렬로 전치 해주어야 함.
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CBone::Update_CombinedTransformationMatrix(const vector<class CBone*>& _vecBone)
{
	// 최상위 부모일 시
	if (m_iParentBoneIndex == -1)
	{
		// 자신의 m_CombinedTransformationMatrix 행렬은 자신의 뼈 행렬이 된다.
		m_CombinedTransformationMatrix = m_TransformationMatrix;
	}
	// 최상위 부모가 아니라면.
	else
	{
		// 자신의 뼈 행렬의 원점은 부모 뼈의 m_CombinedTransformationMatrix가 되야 한다.
		// 자신의 뼈는 부모 뼈의 m_CombinedTransformationMatrix와 연산하여
		// 자신의 m_CombinedTransformationMatrix에 저장할 것이다.
		// 부모의 m_CombinedTransformationMatrix과 연산하는 이유는 부모도 자기 부모의
		// m_CombinedTransformationMatrix와 연산하여 저장하고 있기 때문.
		// 최상의 부모의 경우에는 m_CombinedTransformationMatrix에 자기 m_TransformationMatrix를
		// 대입해 주었기 때문에 전혀 문제가 없다.
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
			XMLoadFloat4x4(&m_TransformationMatrix) *
			XMLoadFloat4x4(&_vecBone[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
	
		//if (!strcmp("NPC COM[COM]", m_szName))
		//{
		//	_vector Zero = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		//	memcpy(&m_CombinedTransformationMatrix.m[3], &Zero, sizeof(_vector));
		//}
	}


	return S_OK;
}

CBone* CBone::Create(const CBin_AIScene::DESC_NODE* _pAINode, _int _iParentBoneIndex)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(_pAINode, _iParentBoneIndex)))
	{
		MSG_BOX("Fail Create : CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone* CBone::Clone()
{
	return new CBone(*this);
}

void CBone::Free()
{
	__super::Free();
}
