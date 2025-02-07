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
	strcpy_s(m_szName, _pAINode->mName.data);

	memcpy(&m_TransformationMatrix, &_pAINode->mTransformation, sizeof(_float4x4));

	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CBone::Update_CombinedTransformationMatrix(const vector<class CBone*>& _vecBone)
{
	if (m_iParentBoneIndex == -1)
	{
		m_CombinedTransformationMatrix = m_TransformationMatrix;
	}
	else
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
			XMLoadFloat4x4(&m_TransformationMatrix) *
			XMLoadFloat4x4(&_vecBone[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
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
